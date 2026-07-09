#include "../include/ndarray.h"

Ndarray *create_ndarray(
    uint8_t *data, size_t data_size,
    size_t ndim,
    size_t *shape,
    uint16_t dtype,
    size_t itemsize
)
{
     Ndarray     *n;
     n->ndim     = ndim;
     n->shape    = shape;
     n->dtype    = dtype;
     n->itemsize = itemsize;

     n->length = 1;

     for (int i = 0; i < ndim; i++)
         n->length *= shape[i];

     n->strides = malloc(ndim);
     // now calculate strides
     // strides is basically shape[n] * itemsize
     
     n->strides[ndim] = itemsize;


    return n;
}

