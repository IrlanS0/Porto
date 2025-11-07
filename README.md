# 🚢 Projeto PAA: Fiscalização Portuária (Poxim Tech)

Este projeto, desenvolvido para a disciplina de Projeto e Análise de Algoritmos (PAA), simula um sistema de triagem de contêineres para fiscalização aduaneira no Porto de Sergipe, conforme as especificações da empresa fictícia Poxim Tech.

## 🎯 O Problema

O objetivo é automatizar a fiscalização. Todos os contêineres possuem dados de cadastro (código, CNPJ, peso) e dados da triagem (o que foi lido no porto). A fiscalização ocorre se houver discrepância.

A regra de negócio principal é a **ordem de prioridade** para a fila de fiscalização:
1.  **Divergência de CNPJ:** Contêineres onde o CNPJ lido é diferente do CNPJ cadastrado (Prioridade 1).
2.  **Maior Diferença Percentual de Peso:** Contêineres onde a diferença de peso é maior que 10% (Prioridade 2).

Crucialmente, a fila de Prioridade 2 deve ser ordenada pela maior diferença percentual primeiro (ordem decrescente).

## 💡 A Solução

Para resolver o problema de forma eficiente, a seguinte abordagem foi implementada em C:

1.  **Armazenamento (Tabela Hash):** Os `n` contêineres do cadastro são lidos e armazenados em uma **Tabela Hash**. O `código` do contêiner é usado como chave. Isso nos permite buscar os dados de cadastro de qualquer contêiner em tempo O(1), em média.

2.  **Comparação (Triagem):** Os `m` contêineres da triagem são lidos um a um. Para cada um:
    * Buscamos seu `código` na Tabela Hash para obter os dados de cadastro.
    * Comparamos os dados lidos (`_m`) com os dados cadastrados (`_n`).
    * Se `cnpj_m != cnpj_n`, o contêiner é marcado como **Prioridade 1**.
    * Se `cnpj_m == cnpj_n` mas a `abs(peso_m - peso_n) / peso_n > 0.10`, o contêiner é marcado como **Prioridade 2**, e sua diferença percentual é calculada e armazenada.

3.  **Ordenação (MergeSort):** Todos os contêineres marcados para fiscalização (Prioridade 1 ou 2) são adicionados a um vetor. Este vetor é então ordenado usando **MergeSort** com uma função de comparação customizada que segue as regras de negócio:
    * `a.prioridade < b.prioridade` (Prioridade 1 sempre vem antes da 2).
    * Se `a.prioridade == 2 && b.prioridade == 2`, então `a.percentual > b.percentual` (maior percentual vem primeiro).
    * (Opcional) Como critério de desempate, a ordem de entrada na triagem é mantida (o MergeSort é um algoritmo de ordenação estável).

## 🛠️ Estruturas de Dados e Algoritmos

* **Tabela Hash:** Usada para armazenar os `n` contêineres.
    * **Complexidade de Inserção:** O(1) em média.
    * **Complexidade de Busca:** O(1) em média.
* **MergeSort:** Usado para ordenar os `k` contêineres fiscalizados.
    * **Complexidade de Ordenação:** O(k log k) em todos os casos.

## 🚀 Como Compilar e Rodar

O projeto utiliza um `Makefile` portátil que automatiza a compilação e os testes em ambientes Windows (com MinGW) e Linux/macOS.

### Requisitos

* `gcc` (ou um compilador C)
* `make` (ou `mingw32-make` no Windows)

### Comandos

O `Makefile` detecta automaticamente o sistema operacional para usar os comandos corretos (`del`/`rm`, `.\`/`./`, `.exe`/etc.).

1.  **Compilar o programa:**
    ```bash
    # No Linux/macOS
    make all

    # No Windows (com MinGW)
    mingw32-make all
    ```
    Isso gera o executável `meu_programa` (ou `meu_programa.exe`).

2.  **Rodar todos os testes (Recomendado):**
    Este comando compila (se necessário) e roda o programa para **todos** os arquivos `*.txt` encontrados na pasta `testes/`, salvando os resultados em `saida_testes/`.

    ```bash
    # No Linux/macOS
    make test

    # No Windows (com MinGW)
    mingw32-make test
    ```

3.  **Limpar o projeto:**
    Apaga o executável e a pasta `saida_testes/` gerada.
    ```bash
    # No Linux/macOS
    make clean

    # No Windows (com MinGW)
    mingw32-make clean
    ```

## 📦 Formato de I/O

### Entrada (`testes/exemplo.txt`)