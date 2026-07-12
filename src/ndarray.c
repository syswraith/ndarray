#include "../include/ndarray.h"
#include <stdint.h>
#include <stdio.h>


Ndarray *create_ndarray(
        void *data,
        size_t ndim,
        size_t *shape,
        size_t itemsize
        )
{
    Ndarray   *n =         malloc(sizeof(Ndarray));
    n->data     = (uint8_t *) data;
    n->ndim     = ndim;       // 3
    n->shape    = shape;      // 5, 3, 3
    n->itemsize = itemsize;   // 4  bytes
    n->length   = 1;
    n->strides  = malloc(ndim *  sizeof(size_t));

    for (size_t i = 0; i < ndim; i++)
        n->length *= shape[i];


    size_t current_stride = itemsize;

    for (size_t i = ndim; i-- > 0;) {
        n->strides[i] = current_stride;
        current_stride *= shape[i];
    }

    return n;
}

void delete_ndarray(Ndarray *n)
{
    free(n->strides);
    free(n);
}

void printer(Ndarray *n) 
{
    printf("ndim: %zu\n", n->ndim);
    printf("length: %zu\n", n->length);
    printf("shape: %zu\n", n->itemsize);

    printf("shape:");
    for (size_t i = 0; i < n->ndim; i++)
        printf("%zu ", n->shape[i]);
    printf("\n");


    printf("strides:");
    for (size_t i = 0; i < n->ndim; i++)
        printf("%zu ", n->strides[i]);
    printf("\n");
}

int check_equal(Ndarray *n1, Ndarray *n2)
{
    int ne = 0;
    for (size_t i = 0; i < n1->ndim; i++)
        (n1->shape[i] != n2->shape[i] || n1->strides[i] != n2->strides[i]) && ne++;

    return (
            n1->ndim == n2->ndim
            &&
            n1->itemsize == n2->itemsize
            &&
            n1->length == n2->length
            &&
            !ne
           );
}


void *index_at(Ndarray *n, size_t indices_size, size_t *indices)
{
    if (n->ndim != indices_size) return NULL;

    size_t offset = 0;
    for (size_t i = n->ndim; i-- > 0;) 
        offset += (n->strides[i] * indices[i]);

    return (uint8_t *) n->data + offset;
}
