#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h> // Necessário para uint32_t, etc.

// 1. Estrutura de dados base
typedef struct {
    char code[12];
    char cnpj[20];
    uint32_t peso;
    uint32_t original_index;
} container_record_t;

// 2. Estrutura de controle principal
typedef struct {
    uint32_t container_size;
    uint32_t container_selected;
    container_record_t *info;
    container_record_t *search;
} container_t;

// 3. Estruturas da Tabela Hash
typedef struct hash_node{
    container_record_t data;
    struct hash_node *next;
} hash_node_t;

typedef struct hash_table{
    hash_node_t **table;
    uint32_t size;
} hash_table_t;

// 4. Estruturas da Lógica de Fiscalização
typedef enum {
    CAUSA_CNPJ = 0,
    CAUSA_PESO = 1,
    CAUSA_NENHUMA = 2
} fiscal_causa_t;

typedef struct {
    fiscal_causa_t causa;       
    uint32_t original_index;    
    container_record_t data_cadastrado;
    container_record_t data_busca;
} fiscal_item_t;

#endif // STRUCTS_H