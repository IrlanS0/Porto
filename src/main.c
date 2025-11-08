#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Incluindo nossos módulos
#include "structs.h"
#include "hash_table.h"
#include "mergesort.h"

#define HASH_TABLE_SIZE 10007 // Numero grande primo 

/*
    @brief: lê o arquivo e cria as structs
*/
container_t loading_memory(FILE *input) {
    // (Implementação do loading_memory... exatamente como você escreveu)
    container_t container;
    
    if(fscanf(input, "%u", &container.container_size) != 1){
        printf("Erro ao ler a linha: 1\n");
        container.container_size = 0;
        container.info = NULL;
        exit(1);
    };
    fgetc(input);
    
    container.info = malloc(container.container_size * sizeof(container_record_t));
    if(!container.info) {
        printf("Erro ao alocar memoria para: info\n");
        exit(1);
    };
    
    uint32_t tmp = 0;
    while(tmp < container.container_size) {
        if(fscanf(input, "%s %s %u", container.info[tmp].code, container.info[tmp].cnpj, &container.info[tmp].peso) != 3) {
            printf("Erro ao ler a linha: %u\n", tmp + 1);
            break;
        }
        container.info[tmp].original_index = tmp;
        tmp++;
    }
    
    if(fscanf(input, "%u", &container.container_selected) != 1){
        printf("Erro ao ler a linha: %u\n", container.container_size + 1);
        container.container_selected = 0;
        exit(1);
    };
    fgetc(input);
    
    container.search = malloc(container.container_selected * sizeof(container_record_t));
    if(!container.search) {
        printf("Erro ao alocar memoria para: search\n");
        free(container.info);
        exit(1);
    };
    
    tmp = 0;
    while(tmp < container.container_selected) {
        if(fscanf(input, "%s %s %u", container.search[tmp].code, container.search[tmp].cnpj, &container.search[tmp].peso) != 3) {
            printf("Erro ao ler a linha: %u\n", container.container_selected + tmp + 1);
            break;
        }
        tmp++;
    }

    return container;
}

/*
    @brief: verifica se peso do container é válido
*/
int fiscalizar_peso(uint32_t peso_cadastrado, uint32_t peso_selecionado){
    double a = (double)peso_cadastrado;
    double b = (double)peso_selecionado;
    double limite = a * 1.1;
    if(b > limite){
        return 1;
    }
    return 0;
}

/*
    @brief: Compara dois itens de fiscalização (para o Merge Sort)
*/
int compare_fiscal_item(const void *a, const void *b) {
    const fiscal_item_t *itemA = (const fiscal_item_t *)a;
    const fiscal_item_t *itemB = (const fiscal_item_t *)b;

    if (itemA->causa != itemB->causa) {
        return (itemA->causa - itemB->causa);
    }
    return (itemA->original_index - itemB->original_index);
}

/*
    @brief: função principal
*/
int main(int argc, char *argv[]) {
    // 1. Abertura de Arquivos
    if(argc < 3) {
        printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    };
    FILE *input = fopen(argv[1], "r");
    if(input == NULL) { /* ... erro */ };
    FILE *output = fopen(argv[2], "w");
    if(output == NULL) { /* ... erro */ };

    // 2. Carregar Dados
    container_t loaded_data = loading_memory(input);

    // 3. Criar e Popular a Tabela Hash (Usando o módulo)
    hash_table_t *ht = create_hash_table(HASH_TABLE_SIZE);
    if (!ht) { exit(1); };
    for (int i = 0; i < loaded_data.container_size; i++) {
        insert_hash(ht, loaded_data.info[i]);
    };

    // 4. Etapa de Coleta (Lógica de Negócio)
    fiscal_item_t *inspection_list = malloc(loaded_data.container_selected * sizeof(fiscal_item_t));
    if (!inspection_list) { exit(1); }
    uint32_t items_to_inspect_count = 0;

    for (uint32_t i = 0; i < loaded_data.container_selected; i++){
        container_record_t *item_busca = &loaded_data.search[i];
        container_record_t *item_cadastrado = search_hash(ht, item_busca->code); // Usa o módulo
        fiscal_causa_t causa_encontrada = CAUSA_NENHUMA;

        if (item_cadastrado != NULL) {
            if (strcmp(item_cadastrado->cnpj, item_busca->cnpj) != 0) {
                causa_encontrada = CAUSA_CNPJ;
            }
            else if (fiscalizar_peso(item_cadastrado->peso, item_busca->peso)) {
                causa_encontrada = CAUSA_PESO;
            }
        }
        
        if (causa_encontrada != CAUSA_NENHUMA) {
            inspection_list[items_to_inspect_count].causa = causa_encontrada;
            inspection_list[items_to_inspect_count].original_index = item_cadastrado->original_index;
            inspection_list[items_to_inspect_count].data_cadastrado = *item_cadastrado;
            inspection_list[items_to_inspect_count].data_busca = *item_busca;         
            items_to_inspect_count++;
        }
    }

    // 5. Etapa de Ordenação (Usando o módulo)
    mymerge_sort(inspection_list,
                 items_to_inspect_count,
                 sizeof(fiscal_item_t),
                 compare_fiscal_item);

    // 6. Etapa de Impressão
    for (uint32_t i = 0; i < items_to_inspect_count; i++) {
        fiscal_item_t *item = &inspection_list[i];
        if (item->causa == CAUSA_CNPJ) {
            fprintf(output, "%s:%s<->%s\n", 
                    item->data_busca.code, 
                    item->data_cadastrado.cnpj,
                    item->data_busca.cnpj);
        }
        else if (item->causa == CAUSA_PESO) {
            uint32_t diff_peso = item->data_busca.peso - item->data_cadastrado.peso;
            double perc = ((double)diff_peso / (double)item->data_cadastrado.peso) * 100.0;
            fprintf(output, "%s:%ukg(%.0f%%)\n", 
                    item->data_busca.code, 
                    diff_peso,
                    perc);
        }
    }

    // 7. Liberação de Memória
    free(inspection_list);
    free_hash_table(ht); // Usa o módulo
    ht = NULL;
    if(loaded_data.info) free(loaded_data.info);
    if(loaded_data.search) free(loaded_data.search);
    
    fclose(input);
    fclose(output);
    return 0;
}