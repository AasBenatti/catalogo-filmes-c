# catalogo-filmes-c
Sistema de gerenciamento de catálogo pessoal de filmes em linguagem C
Este repositório contém um programa em C que adiciona, busca, filtra, atualiza e lista filmes.

Para compilar e executar o sistema, são necessários os seguintes componentes:

Compilador C compatível com a norma ANSI C (exemplo: GCC)
Terminal de linha de comando (Git Bash, CMD, PowerShell, Terminal Linux ou terminal integrado à IDE)
Sistema operacional compatível, como Windows, Linux ou macOS
(Opcional) Ambiente de desenvolvimento integrado (IDE), como o CLion, para facilitar a edição e execução do código

Faça o download do arquivo "ProgramaFilmes.zip" (Clion)
Coloque-o na área de trabalho.
Clique com o botão direito sobre o arquivo e extraia.
Abra o Clion, nos 4 riscos no canto superior esquerdo procure por "Open".
Procure por "C:\Users(Nome do usuário atual)\Desktop\ProgramaFilmes" e selecione o arquivo.
Espere carregar e rode o programa pressionando SHIFT + F10.

Faça o download do arquivo ProgramaFilmes.zip. (Terminal CMD)
Mova o arquivo para a área de trabalho (Desktop). 
Extraia o conteúdo: no Windows, clique com o botão direito sobre o arquivo e selecione "Extrair tudo...". 
No Linux ou macOS, você pode usar o terminal com o comando unzip ~/Desktop/ProgramaFilmes.zip -d ~/Desktop/. 
Abra o terminal: no Windows, pressione Win + R, digite cmd e pressione Enter. 
No Linux ou macOS, abra o Terminal normalmente.
Navegue até a pasta extraída com o comando cd ~/Desktop/ProgramaFilmes. 
No Windows (Prompt de Comando), use cd %USERPROFILE%\Desktop\ProgramaFilmes. 
Compile o programa: se for um projeto com um único arquivo main.cpp, use g++ -o ProgramaFilmes main.cpp. 
Se houver múltiplos arquivos .cpp, compile todos com g++ -o ProgramaFilmes *.cpp. 
Execute o programa: no Linux ou macOS, use ./ProgramaFilmes.
No Windows, use ProgramaFilmes.exe.


Como testar o código
Use o menu interativo Ao executar, o terminal mostrará um menu como este:

Adicionar Filme
Buscar Filmes por Diretor
Filtrar Filmes por Gênero
Atualizar Avaliação de Filme
Listar Filmes
Sair Digite o número da opção desejada e siga as instruções na tela.
Exemplos de uso + Adicionar Filme (opção 1) O sistema pedirá:

ID (número único)
Título
Diretor
Ano (ex: 1999)
Gênero (ex: ação, comédia...)
Avaliação (ex: 8.5)

O sistema impede duplicações (mesmo título + diretor).

 Buscar por Diretor (opção 2) Digite parte do nome do diretor, como:
"Chris"
E o sistema exibirá todos os filmes dirigidos por ele.

Filtrar por Gênero (opção 3) Digite o gênero exatamente como "ação", "drama", "comédia", etc.
O sistema não diferencia letras maiúsculas e minúsculas.

Atualizar Avaliação (opção 4) Digite o ID do filme e depois a nova nota.

Listar todos os filmes (opção 5) Exibe todos os filmes ordenados por:
Ano de lançamento (do mais antigo ao mais recente)
Título (ordem alfabética, caso os anos sejam iguais)

Sair (opção 0) Salva automaticamente os dados no arquivo filmes.csv
Libera a memória usada

Observação Os dados ficam armazenados no arquivo filmes.csv, que é lido e atualizado automaticamente sempre que o programa é executado.

Se o arquivo não existir, ele será criado ao adicionar o primeiro filme.

Se quiser apagar todos os filmes cadastrados, você pode excluir manualmente o arquivo filmes.csv e rodar o programa novamente.
