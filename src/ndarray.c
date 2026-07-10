#include "../include/ndarray.h"
#include <stdio.h>

Ndarray *create_ndarray(
        uint8_t *data,
        size_t ndim,
        size_t *shape,
        size_t itemsize
        )
{
    Ndarray   *n =         malloc(sizeof(Ndarray));
    n->data     = data;
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

    // for (int i = 0; i < ndim; i++)
    //       printf("%zu ", n->strides[i]);

    //    printf("\n");

    return n;
}

void delete_ndarray(Ndarray *n) {
    free(n->strides);
    free(n);
}

