# Compilador C-- 

Este repositório contém a implementação das etapas de **Análise Léxica** e **Análise Sintática** de um compilador para a linguagem **C--**. Projeto desenvolvido em C++ como trabalho prático da disciplina de Compiladores I.

## ⚙️ Funcionalidades Implementadas

- **Analisador Léxico (Scanner):** Lê o código-fonte `.cmm`, ignora espaços e comentários (em linha e bloco), e converte cadeias de caracteres em *Tokens* válidos da gramática.
- **Analisador Sintático (Parser):** Implementação de um analisador preditivo descendente recursivo (Top-Down) que valida a estrutura do código com base nas regras gramaticais de C--.
- **Recuperação de Erros (Modo Pânico):** Implementação de *Panic Mode* para recuperação de erros sintáticos. O compilador não aborta no primeiro erro encontrado; ele reporta a linha e o contexto exatos, ressincroniza a análise em delimitadores seguros (como `;` ou chaves) e continua o processo para listar todos os erros estruturais.

## 📂 Estrutura do Repositório

```text
COMPILADORES/
├── Analise_Lexica/         # Arquivos referentes apenas à Etapa 1 (Scanner isolado)
├── Analise_Sintatica/      # Código fonte final integrado (Scanner + Parser)
│   ├── parser.cpp / .h
│   ├── scanner.cpp / .h
│   ├── token.h
│   └── principal.cpp
├── testes/                 # Bateria de testes (.cmm) para o compilador
├── Makefile                # Script de automação de build e execução de testes
└── README.md
```

## 🚀 Como Compilar e Executar

**Pré-requisitos:** O projeto foi desenvolvido para ser executado em ambientes Linux/Unix (ou WSL no Windows). É necessário ter o compilador `g++` e a ferramenta `make` instalados.

### 1. Compilar o Projeto
Abra o terminal na raiz do repositório (onde o ficheiro `Makefile` se encontra) e execute:
```bash
make
```
*Isto irá compilar os ficheiros fonte localizados na pasta `Analise_Sintatica/` e gerar o executável chamado `cmm` na raiz do projeto.*

### 2. Executar o Compilador (Manualmente)
Para executar o analisador num ficheiro de código específico, passe o caminho do ficheiro como argumento ao executável. Por exemplo:
```bash
./cmm testes/teste1.cmm
```
- **Caso de Sucesso:** O terminal exibirá a mensagem `Compilação encerrada com sucesso!`.
- **Caso de Erro:** O compilador irá listar as linhas e as descrições dos erros sintáticos encontrados, exibindo no final a mensagem `Compilação encerrada com erros estruturais`.

### 3. Executar a Bateria de Testes
O projeto possui um script que varre a pasta `testes/` e processa todos os códigos `.cmm` automaticamente. Para rodar todos os testes de uma só vez, execute:
```bash
make test
```

### 4. Limpar a Build
Para remover o ficheiro executável e deixar a diretoria limpa, basta executar:
```bash
make clean
```
