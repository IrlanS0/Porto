<div align="center">

# 🚢 Projeto de Fiscalização Portuária (PAA)

**Uma solução de alta performance em C para triagem de contêineres usando Tabelas Hash e MergeSort.**

</div>

<p align="center">
  <img src="https://img.shields.io/badge/Linguagem-C-blue.svg" alt="Linguagem C">
  <img src="https://img.shields.io/badge/Padrão-C99-blue.svg" alt="Padrão C99">
  <img src="https://img.shields.io/badge/Build-Makefile-brightgreen.svg" alt="Build com Makefile">
  <img src="https://img.shields.io/badge/Status-Completo-brightgreen.svg" alt="Status Completo">
  <img src="https://img.shields.io/badge/Licença-MIT-yellow.svg" alt="Licença MIT">
</p>

> Este projeto implementa um sistema de fiscalização para a empresa fictícia Poxim Tech, capaz de processar e auditar milhares de manifestos de contêineres, priorizando-os para fiscalização baseada em regras de negócio complexas.

<br>

## 📜 Sumário

* [O Problema](#-o-problema)
* [A Solução Técnica](#-a-solução-técnica)
* [Stack Tecnológica](#-stack-tecnológica)
* [Como Compilar e Rodar](#-como-compilar-e-rodar)
    * [Pré-requisitos](#pré-requisitos)
    * [Comandos do Makefile](#comandos-do-makefile)
* [Formato de I/O](#-formato-de-io)

<br>

## 🎯 O Problema

A empresa Poxim Tech precisa automatizar a fiscalização de contêineres no Porto de Sergipe. O sistema deve comparar os dados cadastrados (`n` contêineres) com os dados lidos na triagem (`m` contêineres) e criar uma fila de fiscalização baseada em duas prioridades:

1.  **Prioridade 1:** Divergência de CNPJ.
2.  **Prioridade 2:** Diferença percentual de peso líquido superior a 10%.

A fila de Prioridade 2 deve ser ordenada da maior diferença percentual para a menor.

## 💡 A Solução Técnica

Para garantir performance e eficiência, a solução foi dividida em duas etapas principais, utilizando estruturas de dados clássicas de PAA:

### 1. Tabela Hash (Armazenamento e Busca)

Os `n` contêineres do cadastro são armazenados em uma **Tabela Hash** (Hashing de Endereçamento Aberto ou Fechado).
* **Chave:** O `código` do contêiner.
* **Resultado:** Isso permite que, durante a triagem, os dados de cadastro de qualquer um dos `m` contêineres sejam encontrados em tempo **O(1)** (em média).

### 2. MergeSort (Ordenação e Priorização)

Os `k` contêineres que apresentam discrepâncias são adicionados a um vetor. Este vetor é então ordenado usando **MergeSort**.
* **Por quê?** O MergeSort garante complexidade **O(k log k)** no pior caso e é um algoritmo de ordenação estável.
* **Função de Comparação:** A complexidade real está na função `compare_fiscal_item()`, que implementa a regra de negócio de múltiplas prioridades para criar a fila de fiscalização:
1. ***Nível de prioridade***: Contêineres com Causa 1 (Divergência de CNPJ) sempre vêm antes de contêineres com Causa 2 (Diferença de Peso).
2. ***Ordenação Causa 1***: Se dois contêineres são ambos da Causa 1, eles são exibidos por ordem de entrada (First-in-First-out). 
3. ***Ordenação da Causa 2***: Se dois contêineres são ambos da Causa 2, eles são ordenados pela maior diferença percentual de peso, em ordem decrescente. Caso a diferença percentual seja a mesma, são ordenados por ordem de entrada.
<br>

## 🛠️ Stack Tecnológica

* **Linguagem:** `C` (Padrão C99)
* **Compilação:** `GCC`
* **Build System:** `Make` (com um Makefile portátil para Windows/Linux)
* **Controle de Versão:** `Git` e `GitHub`

<br>

## 🚀 Como Compilar e Rodar

Este projeto utiliza um `Makefile` robusto que detecta automaticamente o sistema operacional (Windows ou Linux/macOS) para usar os comandos de compilação e limpeza corretos.

### Pré-requisitos

* `gcc` (Compilador C)
* `make` (no Linux/macOS) ou `mingw32-make` (no Windows)

### Comandos do Makefile

Abra o terminal na raiz do projeto e execute:

| Comando | Ação |
| :--- | :--- |
| `make all` (ou `mingw32-make all`) | **Compila** o executável `main` (ou `.exe`). |
| `make test` (ou `mingw32-make test`) | **Roda todos os testes.** Encontra todos os `*.txt` na pasta `tests/`, executa o programa e salva os resultados em `build/`. |
| `make clean` (ou `mingw32-make clean`) | **Limpa** o projeto, apagando o executável e a pasta `build/`. |

<br>
