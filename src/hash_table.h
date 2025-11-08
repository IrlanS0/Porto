#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "structs.h" // Precisa conhecer as structs que utiliza

// Protótipos das funções públicas
hash_table_t *create_hash_table(uint32_t size);
void insert_hash(hash_table_t *ht, container_record_t data);
container_record_t *search_hash(hash_table_t *ht, char *code);
void free_hash_table(hash_table_t *ht);

#endif // HASH_TABLE_H