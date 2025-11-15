#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h" // Inclui as declarações que estamos implementando
#include "structs.h"    // Inclui as definições das structs

/*
    @brief: função para hash(DJB2) - Privada para este módulo
*/
static uint64_t string_hash(const char *str, unsigned int N)
{
    unsigned int hash_value = 5381;
    int c;
    while ((c = *str++))
    {
        hash_value = ((hash_value << 5) + hash_value) + c;
    }
    return hash_value % N;
}

/* @brief: cria tabela hash
 */
hash_table_t *create_hash_table(uint32_t size)
{
    hash_table_t *ht = (hash_table_t *)malloc(sizeof(hash_table_t));
    if (!ht)
    {
        printf("Erro ao alocar memoria para: table\n");
        return NULL;
    };

    ht->size = size;
    ht->table = (hash_node_t **)calloc(size, sizeof(hash_node_t *));
    if (!ht->table)
    {
        printf("Erro ao alocar memoria para: table->table\n");
        free(ht);
        return NULL;
    };
    return ht;
}

/*
    @brief: insere na tabela hash
*/
void insert_hash(hash_table_t *ht, container_record_t data)
{
    hash_node_t *new_node = (hash_node_t *)malloc(sizeof(hash_node_t));
    if (!new_node)
    {
        printf("Erro ao alocar memoria para: novo no\n");
        exit(1);
    };

    new_node->data = data;
    uint64_t index = string_hash(data.code, ht->size);
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
}

/*
    @brief: procura na tabela hash
*/
container_record_t *search_hash(hash_table_t *ht, char *code)
{
    uint64_t index = string_hash(code, ht->size);
    hash_node_t *current = ht->table[index];

    while (current)
    {
        if (strcmp(current->data.code, code) == 0)
        {
            return &current->data;
        };
        current = current->next;
    }
    return NULL;
}

/*
    @brief: libera memoria alocada para tabela hash
*/
void free_hash_table(hash_table_t *ht)
{
    for (int i = 0; i < ht->size; i++)
    {
        hash_node_t *current = ht->table[i];
        while (current)
        {
            hash_node_t *next = current->next;
            free(current);
            current = next;
        };
    }
    free(ht->table);
    free(ht);
}