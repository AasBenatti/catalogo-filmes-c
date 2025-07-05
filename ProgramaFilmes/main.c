#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para isdigit
#include <limits.h> // Para INT_MIN e INT_MAX

#define MAX_FILMES 500 // Limite de filmes (ajustável se necessário)
#define MAX_STR 100 // Tamanho máximo para strings como título, diretor, gênero

// Estrutura para representar um filme
typedef struct {
    int id; // ID do filme
    char titulo[MAX_STR]; // Título
    char diretor[MAX_STR]; // Diretor
    int ano; // Ano
    char genero[MAX_STR]; // Gênero
    float avaliacao; // Avaliação (0-10)
} Filme;

Filme **filmes; // Vetor global de ponteiros para structs de filmes
int numFilmes = 0; // Contador de filmes

// Protótipos das funções
void inicializa(); // Inicializa o sistema carregando dados do CSV
void salvarDados(); // Salva os dados no arquivo CSV
void adicionarFilme(); // Adiciona um novo filme
void buscarPorDiretor(); // Busca filmes por diretor
void filtrarPorGenero(); // Filtra filmes por gênero
void atualizarAvaliacao(); // Atualiza a avaliação de um filme
void listarFilmes(); // Lista filmes por ano/título
void limparMemoria(); // Função para liberar a memória alocada dinamicamente

// Funções de validação de entrada
int lerInteiro(const char *prompt);
float lerFloat(const char *prompt);
void lerString(const char *prompt, char *buffer, int tamanho);

int main() {
    inicializa(); // Carrega os dados do CSV ao iniciar o programa

    int opcao;
    do {
        printf("\n--- Catalogo de Filmes Pessoais ---\n");
        printf("1. Adicionar Filme\n");
        printf("2. Buscar Filmes por Diretor\n");
        printf("3. Filtrar Filmes por Genero\n");
        printf("4. Atualizar Avaliacao de Filme\n");
        printf("5. Listar Filmes (por Ano/Titulo)\n");
        printf("0. Sair\n");
        opcao = lerInteiro("Escolha uma opcao: "); // Usa a função de leitura validada

        switch (opcao) {
            case 1:
                adicionarFilme();
                break;
            case 2:
                buscarPorDiretor();
                break;
            case 3:
                filtrarPorGenero();
                break;
            case 4:
                atualizarAvaliacao();
                break;
            case 5:
                listarFilmes();
                break;
            case 0:
                printf("Saindo do programa. Salvando dados...\n");
                salvarDados(); // Salva as alterações ao encerrar
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    } while (opcao != 0);

    limparMemoria(); // Libera a memória antes de sair
    return 0;
}

// Inicializa o sistema carregando dados do arquivo CSV
void inicializa() {
    FILE *arquivo = fopen("filmes.csv", "r");
    if (!arquivo) {
        printf("Arquivo 'filmes.csv' não encontrado. Um novo será criado ao salvar.\n"); // Tratamento de erro de abertura
        filmes = (Filme**)malloc(MAX_FILMES * sizeof(Filme*)); // Aloca o vetor de ponteiros
        if (filmes == NULL) {
            printf("Erro de alocacao de memória inicial!\n");
            exit(1); // Encerra o programa se não conseguir alocar memória essencial
        }
        return;
    }

    filmes = (Filme**)malloc(MAX_FILMES * sizeof(Filme*)); // Aloca o vetor de ponteiros para structs
    if (filmes == NULL) {
        printf("Erro de alocacao de memória inicial!\n");
        fclose(arquivo);
        exit(1);
    }

    char linha[512];
    fgets(linha, 512, arquivo); // Ignora o cabeçalho

    while (fgets(linha, 512, arquivo)) {
        if (numFilmes >= MAX_FILMES) {
            printf("Limite maximo de filmes atingido. Dados não carregados completamente.\n");
            break;
        }

        filmes[numFilmes] = (Filme*)malloc(sizeof(Filme)); // Aloca dinamicamente cada registro
        if (filmes[numFilmes] == NULL) {
            printf("Erro de alocacao de memória para filme %d! Parando o carregamento.\n", numFilmes);
            // Libera a memória já alocada antes de sair em caso de erro grave
            for (int i = 0; i < numFilmes; i++) {
                free(filmes[i]);
            }
            free(filmes);
            fclose(arquivo);
            exit(1);
        }

        // Usar sscanf para ler os dados.
        // %[^,]: Lê até encontrar uma vírgula. Isso ajuda a ler strings com espaços.
        // Nota: Para campos com vírgulas internas que precisam ser tratadas como parte do campo,
        // o ideal seria uma lógica de parsing mais robusta que lide com aspas duplas (e.g., "Arroz, Tipo 1").
        // Para este exemplo, assumimos que campos como Título, Diretor, Gênero não terão vírgulas internas sem aspas.
        int result = sscanf(linha, "%d,%[^,],%[^,],%d,%[^,],%f",
                            &filmes[numFilmes]->id,
                            filmes[numFilmes]->titulo,
                            filmes[numFilmes]->diretor,
                            &filmes[numFilmes]->ano,
                            filmes[numFilmes]->genero,
                            &filmes[numFilmes]->avaliacao);

        if (result == 6) { // Verifica se todos os 6 campos foram lidos com sucesso
            numFilmes++;
        } else {
            printf("Erro ao ler linha do CSV: %s (Ignorando linha)\n", linha);
            free(filmes[numFilmes]); // Libera a memória do filme mal formatado
        }
    }
    fclose(arquivo);
    printf("Dados carregados com sucesso de 'filmes.csv'. Total de filmes: %d\n", numFilmes);
}

// Salva os dados no arquivo CSV
void salvarDados() {
    FILE *arquivo = fopen("filmes.csv", "w"); // Abre em modo de escrita, sobrescrevendo o arquivo
    if (!arquivo) {
        printf("Erro ao salvar arquivo 'filmes.csv'!\n"); // Tratamento de erro de abertura/escrita
        return;
    }

    fprintf(arquivo, "ID,Titulo,Diretor,Ano,Genero,Avaliacao\n"); // Escreve o cabeçalho
    for (int i = 0; i < numFilmes; i++) {
        // Usa aspas duplas para campos que podem conter vírgulas ou serem ambíguos.

        fprintf(arquivo, "%d,%s,%s,%d,%s,%.1f\n",
                filmes[i]->id,
                filmes[i]->titulo,
                filmes[i]->diretor,
                filmes[i]->ano,
                filmes[i]->genero,
                filmes[i]->avaliacao);
    }
    fclose(arquivo);
    printf("Dados salvos com sucesso em 'filmes.csv'.\n");
}

// Adiciona um novo filme
void adicionarFilme() {
    if (numFilmes >= MAX_FILMES) {
        printf("Limite maximo de filmes atingido. Nao e possível adicionar mais.\n");
        return;
    }

    Filme *novoFilme = (Filme*)malloc(sizeof(Filme));
    if (novoFilme == NULL) {
        printf("Erro de alocacao de memoria para novo filme!\n");
        return;
    }

    printf("\n--- Adicionar Novo Filme ---\n");

    // Validação de ID: deve ser positivo e único
    int idValido = 0;
    while (!idValido) {
        novoFilme->id = lerInteiro("ID do Filme: ");
        if (novoFilme->id <= 0) {
            printf("O ID deve ser um numero inteiro positivo.\n");
        } else {
            // Verifica se o ID já existe
            int idExiste = 0;
            for (int i = 0; i < numFilmes; i++) {
                if (filmes[i]->id == novoFilme->id) {
                    idExiste = 1;
                    break;
                }
            }
            if (idExiste) {
                printf("Este ID ja esta em uso. Por favor, escolha um ID diferente.\n");
            } else {
                idValido = 1;
            }
        }
    }

    lerString("Titulo: ", novoFilme->titulo, MAX_STR);
    lerString("Diretor: ", novoFilme->diretor, MAX_STR);

    // Validação de Ano: deve ser um ano razoável (1888, ano do primeiro filme registrado)
    int anoValido = 0;
    while (!anoValido) {
        novoFilme->ano = lerInteiro("Ano (ex: 2023): ");
        if (novoFilme->ano > 1888 && novoFilme->ano <= 2025) { // Considerando ano atual 2025
            anoValido = 1;
        } else {
            printf("Ano invalido. Por favor, insira um ano valido (e.g., entre 1888 e 2025).\n");
        }
    }

    lerString("Genero: ", novoFilme->genero, MAX_STR);

    // Validação de Avaliação: deve ser entre 0 e 10
    int avaliacaoValida = 0;
    while (!avaliacaoValida) {
        novoFilme->avaliacao = lerFloat("Avaliação (0-10): ");
        if (novoFilme->avaliacao >= 0.0 && novoFilme->avaliacao <= 10.0) {
            avaliacaoValida = 1;
        } else {
            printf("Avaliacao invalida. Deve ser entre 0 e 10.\n");
        }
    }

    filmes[numFilmes] = novoFilme;
    numFilmes++;
    printf("Filme '%s' adicionado com sucesso!\n", novoFilme->titulo);
    salvarDados(); // Salva imediatamente após a alteração
}

// Busca filmes por diretor
void buscarPorDiretor() {
    char diretorBusca[MAX_STR];
    printf("\n--- Buscar Filmes por Diretor ---\n");
    lerString("Digite o nome do diretor para buscar: ", diretorBusca, MAX_STR);

    int encontrados = 0;
    for (int i = 0; i < numFilmes; i++) {
        // strcasecmp para comparação sem considerar maiúsculas/minúsculas (se disponível no seu compilador)
        // ou strstr para busca parcial, case-sensitive
        if (strstr(filmes[i]->diretor, diretorBusca) != NULL) {
            printf("ID: %d, Titulo: %s, Diretor: %s, Ano: %d, Genero: %s, Avaliacao: %.1f\n",
                   filmes[i]->id, filmes[i]->titulo, filmes[i]->diretor,
                   filmes[i]->ano, filmes[i]->genero, filmes[i]->avaliacao);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("Nenhum filme encontrado para o diretor '%s'.\n", diretorBusca);
    }
}

// Filtra filmes por gênero
void filtrarPorGenero() {
    char generoBusca[MAX_STR];
    printf("\n--- Filtrar Filmes por Gênero ---\n");
    lerString("Digite o genero para filtrar: ", generoBusca, MAX_STR);

    int encontrados = 0;
    for (int i = 0; i < numFilmes; i++) {
        // strcasecmp para comparação sem considerar maiúsculas/minúsculas
        if (strcasecmp(filmes[i]->genero, generoBusca) == 0) {
            printf("ID: %d, Titulo: %s, Diretor: %s, Ano: %d, Genero: %s, Avaliacao: %.1f\n",
                   filmes[i]->id, filmes[i]->titulo, filmes[i]->diretor,
                   filmes[i]->ano, filmes[i]->genero, filmes[i]->avaliacao);
            encontrados++;
        }
    }
    if (encontrados == 0) {
        printf("Nenhum filme encontrado para o genero '%s'.\n", generoBusca);
    }
}

// Atualiza a avaliação de um filme
void atualizarAvaliacao() {
    int idBusca;
    printf("\n--- Atualizar Avaliacao de Filme ---\n");
    idBusca = lerInteiro("Digite o ID do filme para atualizar: ");

    int encontrado = 0;
    for (int i = 0; i < numFilmes; i++) {
        if (filmes[i]->id == idBusca) {
            printf("Filme encontrado: %s (Avaliacao atual: %.1f)\n", filmes[i]->titulo, filmes[i]->avaliacao);

            // Validação da nova avaliação
            int avaliacaoValida = 0;
            float novaAvaliacao;
            while (!avaliacaoValida) {
                novaAvaliacao = lerFloat("Digite a nova avaliacao (0-10): ");
                if (novaAvaliacao >= 0.0 && novaAvaliacao <= 10.0) {
                    filmes[i]->avaliacao = novaAvaliacao;
                    printf("Avaliacao do filme '%s' atualizada para %.1f.\n", filmes[i]->titulo, novaAvaliacao);
                    salvarDados(); // Salva imediatamente após a alteração [cite: 40, 167]
                    avaliacaoValida = 1;
                } else {
                    printf("Avaliacao invalida. Deve ser entre 0 e 10.\n");
                }
            }
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Filme com ID %d não encontrado.\n", idBusca);
    }
}

// Função de comparação para qsort (ordena por ano, e depois por título)
int compararFilmes(const void *a, const void *b) {
    Filme *filmeA = *(Filme**)a;
    Filme *filmeB = *(Filme**)b;

    if (filmeA->ano != filmeB->ano) {
        return filmeA->ano - filmeB->ano;
    }
    return strcmp(filmeA->titulo, filmeB->titulo);
}

// Lista filmes ordenados por ano/título
void listarFilmes() {
    if (numFilmes == 0) {
        printf("Nenhum filme cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Filmes (Ordenados por Ano e Titulo) ---\n");

    // Ordena o vetor global de ponteiros diretamente.
    qsort(filmes, numFilmes, sizeof(Filme*), compararFilmes);

    for (int i = 0; i < numFilmes; i++) {
        printf("ID: %d, Titulo: %s, Diretor: %s, Ano: %d, Genero: %s, Avaliacao: %.1f\n",
               filmes[i]->id, filmes[i]->titulo, filmes[i]->diretor,
               filmes[i]->ano, filmes[i]->genero, filmes[i]->avaliacao);
    }
}

// Libera a memória alocada dinamicamente
void limparMemoria() {
    for (int i = 0; i < numFilmes; i++) {
        free(filmes[i]); // Libera cada struct Filme
    }
    free(filmes); // Libera o vetor de ponteiros
    printf("Memoria liberada.\n");
}

// Funções de Validação de Entrada

// Função para ler um inteiro com validação
int lerInteiro(const char *prompt) {
    int valor;
    char buffer[MAX_STR];
    while (1) {
        printf("%s", prompt);
        fgets(buffer, MAX_STR, stdin);
        // Remove o '\n' do final
        buffer[strcspn(buffer, "\n")] = 0;

        // Tenta converter para inteiro
        char *endptr;
        long numL = strtol(buffer, &endptr, 10);

        // Verifica se a string estava vazia ou se contém caracteres não numéricos
        if (buffer[0] == '\0' || *endptr != '\0') {
            printf("Entrada invalida. Por favor, digite um numero inteiro.\n");
        } else {
            // Verifica se o número está dentro dos limites de um int
            if (numL < INT_MIN || numL > INT_MAX) { // INT_MIN e INT_MAX são do <limits.h>
                printf("Numero fora dos limites para um inteiro. Tente um valor menor/maior.\n");
            } else {
                valor = (int)numL;
                return valor;
            }
        }
    }
}

// Função para ler um float com validação
float lerFloat(const char *prompt) {
    float valor;
    char buffer[MAX_STR];
    while (1) {
        printf("%s", prompt);
        fgets(buffer, MAX_STR, stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        char *endptr;
        float numF = strtof(buffer, &endptr);

        if (buffer[0] == '\0' || *endptr != '\0') {
            printf("Entrada invalida. Por favor, digite um numero.\n");
        } else {
            valor = numF;
            return valor;
        }
    }
}

// Função para ler uma string com tratamento de espaços e limite de tamanho
void lerString(const char *prompt, char *buffer, int tamanho) {
    printf("%s", prompt);
    fgets(buffer, tamanho, stdin);
    // Remover o newline se presente
    buffer[strcspn(buffer, "\n")] = 0;

    // Limpar o buffer de entrada caso o usuário digite mais caracteres do que o tamanho
    if ((int)strlen(buffer) == tamanho - 1 && buffer[tamanho - 2] != '\n') {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
}