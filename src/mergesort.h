#ifndef MERGESORT_H
#define MERGESORT_H

#include <stdlib.h>

void mymerge_sort(void *arr, size_t elements_size, size_t size, int (*compare)(const void *, const void *));

#endif // MERGESORT_H