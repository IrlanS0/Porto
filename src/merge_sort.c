#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mergesort.h" 

/*
    @brief: função auxiliar para merge sort(intercala) - Privada
*/
static void merge(void *arr, uint32_t init, uint32_t mid, uint32_t end, size_t element_size, int (*compare)(const void*, const void*)) {
    char *tmp = malloc((end - init + 1) * (element_size));
    if(!tmp){
        printf("Erro ao alocar memoria para: tmp\n");
        exit(1);
    };

    uint32_t i = init, j = mid + 1, k = 0;
    while(i <= mid && j <= end) 
    {
        if((compare((char *)arr + (i * element_size), (char *)arr + (j * element_size)) <= 0)) {
            memcpy(tmp + k * element_size, (char *)arr + (i * element_size), element_size);
            i++; k++;
        }else{
            memcpy(tmp + k * element_size, (char *)arr + (j * element_size), element_size);
            j++; k++;
        }
    }
    
    while(i <= mid) {
        memcpy(tmp + k * element_size, (char *)arr + (i * element_size), element_size);
        i++; k++;
    }
    while (j <= end) {
        memcpy(tmp + k * element_size, (char *)arr + (j * element_size), element_size);
        j++; k++;
    }
    
    uint32_t num_elements = end - init + 1;
    for(k = 0; k < num_elements; k++) {
        memcpy((char *)arr + (init + k) * element_size, tmp + k * element_size, element_size);
    }

    free(tmp);
}

/*
    @brief: criando função merge sort - Privada
*/
static void mergeSort_recursive(void *arr, uint32_t init, uint32_t end, size_t element_size, int (*compare)(const void*, const void*)) {
    if(init >= end) return;
    uint32_t mid = init + (end - init) / 2;
    mergeSort_recursive(arr, init, mid, element_size, compare);
    mergeSort_recursive(arr, mid + 1, end, element_size, compare);
    merge(arr, init, mid, end, element_size, compare);
}

/*
    @brief: função para chamar merge sort com mais facilidade - Pública
*/
void mymerge_sort(void *arr, size_t elements_size, size_t size, int (*compare)(const void*, const void*)) {
    if(elements_size < 2)return; //ja ordenado
    mergeSort_recursive(arr, 0, elements_size - 1, size, compare);
}