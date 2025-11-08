#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#define HASH_TABLE_SIZE 10007 // Numero grande primo 
/*
    @brief: cria structs para o programa
*/
typedef struct {
    char code[12];
    char cnpj[20];
    uint32_t peso;
} container_record_t;

typedef struct {
    uint32_t container_size;
    uint32_t container_selected;
    container_record_t *info;
    container_record_t *search;
} container_t;
// Nó da lista ligada(Guarda o nó e o ponteiro para o próximo nó)
typedef struct hash_node{
    container_record_t data; // dado completo do container_record_t
    struct hash_node *next; // Ponteiro para o próximo nó(tratamento da colisão)
}hash_node_t;
// Estrutura da Tabela Hash
typedef struct hash_table{
    hash_node_t **table; // Array de ponteiros para tabela hash_node_t
    uint32_t size; // Tamanho do array
}hash_table_t;

/*
    @brief: lê o arquivo e cria as structs
*/
container_t loading_memory(FILE *input) {
    container_t container;
    
    if(fscanf(input, "%u", &container.container_size) != 1){
        printf("Erro ao ler a linha: 1\n");
        container.container_size = 0;
        container.info = NULL;
        exit(1);
    };
    fgetc(input);
    // printf("[DEBUG] container_size: %u\n", container.container_size);
    
    //alocando memoria para container.info 
    container.info = malloc(container.container_size * sizeof(container_record_t));
    if(!container.info) {
        printf("Erro ao alocar memoria para: info\n");
        exit(1);
    };
    
    //lendo as informacoes originais
    uint32_t tmp = 0;
    while(tmp < container.container_size) {
        if(fscanf(input, "%s %s %u", container.info[tmp].code, container.info[tmp].cnpj, &container.info[tmp].peso) != 3) {
            printf("Erro ao ler a linha: %u\n", tmp + 1);
            break;
        }
        // printf("[DEBUG] container info: \nCode: %s\nCNPJ: %s\nPeso: %u\n", container.info[tmp].code, container.info[tmp].cnpj, container.info[tmp].peso);
        tmp++;
    }
    
    //lendo o numero de containers selecionados
    if(fscanf(input, "%u", &container.container_selected) != 1){
        printf("Erro ao ler a linha: %u\n", container.container_size + 1);
        container.container_selected = 0;
        exit(1);
    };
    fgetc(input);
    // printf("[DEBUG] container_selected: %u\n", container.container_selected);
    
    //alocando memoria para container.search
    container.search = malloc(container.container_selected * sizeof(container_record_t));
    if(!container.search) {
        printf("Erro ao alocar memoria para: search\n");
        free(container.info);
        exit(1);
    };
    
    //lendo as informacoes dos containers selecionados
    tmp = 0;
    while(tmp < container.container_selected) {
        if(fscanf(input, "%s %s %u", container.search[tmp].code, container.search[tmp].cnpj, &container.search[tmp].peso) != 3) {
            printf("Erro ao ler a linha: %u\n", container.container_selected + tmp + 1);
            break;
        }
        // printf("[DEBUG] container search: \nCode: %s\nCNPJ: %s\nPeso: %u\n", container.search[tmp].code, container.search[tmp].cnpj, container.search[tmp].peso);
        tmp++;
    }

    return container;
};

/*
    @brief: função que faz operação de comparação
*/
int compare(const void *key, const void *item) {
    const container_record_t *search_key = (container_record_t *)key;
    const container_record_t *search_item = (container_record_t *)item;
    return strcmp(search_key->code, search_item->code);
};

/*
    @brief: função auxiliar para merge sort(intercala)
*/
void merge(void *arr, uint32_t init, uint32_t mid, uint32_t end, size_t element_size, int (*compare)(const void*, const void*)) {
    char *tmp = malloc((end - init + 1) * (element_size));
    if(!tmp){
        printf("Erro ao alocar memoria para: tmp\n");
        exit(1);
    };

    uint32_t i = init, j = mid + 1, k = 0;
    while(i <= mid && j <= end) 
    {
        // O endereço de qualquer elemento i em um array genérico(tipo void*) é: ponteiro_base + (indice * tamanho_do_elemento)
        // em C isso vira: (char *)arr + (i * element_size)
        if((compare((char *)arr + (i * element_size), (char *)arr + (j * element_size)) <= 0)) {
            memcpy(tmp + k * element_size, (char *)arr + (i * element_size), element_size);
            i++;
            k++;
        }else{
            memcpy(tmp + k * element_size, (char *)arr + (j * element_size), element_size);
            j++;
            k++;
        }
    }

    // se ainda houver elementos na primeira metade
    while(i <= mid)
    {
        memcpy(tmp + k * element_size, (char *)arr + (i * element_size), element_size);
        i++;
        k++;
    }
    // se ainda houver elementos na segunda metade
    while (j <= end)
    {
        memcpy(tmp + k * element_size, (char *)arr + (j * element_size), element_size);
        j++;
        k++;
    }
    // copiando os elementos para o array
    uint32_t num_elements = end - init + 1;
    for(k = 0; k < num_elements; k++) {
        memcpy((char *)arr + (init + k) * element_size, tmp + k * element_size, element_size);
    }

    free(tmp);
    tmp = NULL;
    return;
};

/*
    @brief: criando função merge sort
*/
void mergeSort_recursive(void *arr, uint32_t init, uint32_t end, size_t element_size, int (*compare)(const void*, const void*)) {
    if(init >= end) return;
    //dividir
    uint32_t mid = init + (end - init) / 2;
    //conquistar 
    mergeSort_recursive(arr, init, mid, element_size, compare);
    mergeSort_recursive(arr, mid + 1, end, element_size, compare);
    //combinar
    merge(arr, init, mid, end, element_size, compare);
}

/*
    @brief: função para chamar merge sort com mais facilidade
*/
void mymerge_sort(void *arr, size_t elements_size, size_t size, int (*compare)(const void*, const void*)) {
    if(elements_size < 2)return; //ja ordenado

    mergeSort_recursive(arr, 0, elements_size - 1, size, compare);
}

/*
    @brief: função para hash(DJB2)
*/
uint64_t string_hash(const char *str, unsigned int N){
    unsigned int hash_value = 5381;
    int c;

    while((c = *str++)){
        // hash = hash * 33 + c
        hash_value = ((hash_value << 5) + hash_value) + c; 
    }

    return hash_value % N;
}

/*  
    @brief: cria tabela hash
*/
hash_table_t *create_hash_table(uint32_t size){
    hash_table_t *ht = (hash_table_t *)malloc(sizeof(hash_table_t));
    if(!ht){
        printf("Erro ao alocar memoria para: table\n");
        return NULL;
    };
    
    ht->size = size;
    ht->table = (hash_node_t **)calloc(size, sizeof(hash_node_t*));
    if(!ht->table){
        printf("Erro ao alocar memoria para: table->table\n");
        free(ht);
        return NULL;
    };
    return ht;
};

/*
    @brief: insere na tabela hash
*/
void insert_hash(hash_table_t *ht, container_record_t data){
    hash_node_t *new_node = (hash_node_t *)malloc(sizeof(hash_node_t));
    if (!new_node){
        printf("Erro ao alocar memoria para: novo no\n");
        exit(1);
    };
    // Preencher os nos com os dados
    new_node->data = data;
    
    // Calcular o indice onde o no deve ir 
    uint64_t index = string_hash(data.code, ht->size);

    // Logica de inserção (Encadeamento O(1))
    // O next do novo no aponta para o que estava no inicio da lista
    new_node->next = ht->table[index];

    // A cabeça da lista(o ponteiro no array) agora aponta para o novo nó
    ht->table[index] = new_node;
};

/*
    @brief: procura na tabela hash
*/
container_record_t *search_hash(hash_table_t *ht, char *code){
    
};

/*
    @brief: libera memoria alocada para tabela hash
*/
void free_hash_table(hash_table_t *ht){

};

/*
    @brief: função principal
*/
int main(int argc, char *argv[]) {
    // abrindo arquivos
    if(argc < 3) {
    printf("Uso: %s <arquivo_entrada> <arquivo_saida>\n", argv[0]);
    return 1;
    };
    FILE *input = fopen(argv[1], "r");
    if(input == NULL) {
        printf("Erro ao abrir arquivo de entrada\n");
        return 1;
    };
    FILE *output = fopen(argv[2], "w");
    if(output == NULL) {
        printf("Erro ao abrir arquivo de saida\n");
        return 1;
    };

    // carregando memoria
    container_t loaded_data = loading_memory(input);

    // Criar tabela hash
    hash_table_t *ht = create_hash_table(HASH_TABLE_SIZE);
    if (!ht)
    {
        printf("Erro ao criar tabela hash\n");
        exit(1);
    };

    // Preencher tabela hash com os dados de 'container_info'
    for (int i = 0; i < loaded_data.container_size; i++)
    {
        insert_hash(ht, loaded_data.info[i]);
    };
    
    //liberando memoria, fechando arquivos
    if(loaded_data.info) {
        free(loaded_data.info);
        loaded_data.info = NULL;
    };
    if(loaded_data.search) {
        free(loaded_data.search);
        loaded_data.search = NULL;
    };
    fclose(input);
    fclose(output);
    return 0;
}