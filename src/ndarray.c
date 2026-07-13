#include "../include/ndarray.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

Ndarray *create_ndarray(
        void *data,
        size_t ndim,
        size_t *shape,
        size_t itemsize
        )
{
    Ndarray   *n =         malloc(sizeof(Ndarray));

    n->ndim     = ndim;       // 3
    n->shape = malloc(n->ndim * sizeof(size_t));
    
    for (size_t i = 0; i < n->ndim; i++)
        n->shape[i] = shape[i];

    n->itemsize = itemsize;   // 4  bytes
    n->length   = 1;
    n->strides  = malloc(ndim *  sizeof(size_t));

    for (size_t i = 0; i < ndim; i++)
        n->length *= shape[i];

    n->data = malloc(n->length * n->itemsize);
    memcpy(n->data, data, (n->length * n->itemsize));

    size_t current_stride = itemsize;

    for (size_t i = ndim; i-- > 0;) {
        n->strides[i] = current_stride;
        current_stride *= shape[i];
    }

    return n;
}

void delete_ndarray(Ndarray *n)
{
    free(n->data);
    free(n->strides);
    free(n->shape);
    free(n);
}

void print_ndim_recursive(uint8_t *data_ptr, size_t *shape, size_t *strides, size_t ndim, size_t current_dim) 
{
    // Base Case: We are at the final dimension, print the actual row of numbers
    if (current_dim == ndim - 1) {
        printf("[");
        for (size_t i = 0; i < shape[current_dim]; i++) {
            // Calculate the exact byte offset for this specific element
            int32_t *val = (int32_t *)(data_ptr + (i * strides[current_dim]));
            
            printf("%d", *val);
            if (i < shape[current_dim] - 1) printf(", ");
        }
        printf("]");
        return;
    }

    // Recursive Case: We have higher dimensions, print brackets and drop down a level
    printf("[");
    for (size_t i = 0; i < shape[current_dim]; i++) {
        // Shift the data pointer forward by the current dimension's stride multiplier
        uint8_t *next_dimension_ptr = data_ptr + (i * strides[current_dim]);
        
        print_ndim_recursive(next_dimension_ptr, shape, strides, ndim, current_dim + 1);
        
        // Formatting: don't print a trailing comma on the last element of this block
        if (i < shape[current_dim] - 1) {
            printf(",\n");
            // Indent the next row slightly based on how deep we are for clean alignment
            for (size_t space = 0; space <= current_dim; space++) printf(" ");
        }
    }
    printf("]");
}

// wrapper
void print_ndarray_data(Ndarray *n) 
{
    if (!n || !n->data) {
        printf("[]\n");
        return;
    }
    print_ndim_recursive((uint8_t *)n->data, n->shape, n->strides, n->ndim, 0);
    printf("\n");
}



void printer(Ndarray *n) 
{
    printf("ndim: %zu\n", n->ndim);
    printf("length: %zu\n", n->length);
    printf("itemsize: %zu\n", n->itemsize);

    printf("shape: ");
    for (size_t i = 0; i < n->ndim; i++)
        printf("%zu ", n->shape[i]);
    printf("\n");


    printf("strides: ");
    for (size_t i = 0; i < n->ndim; i++)
        printf("%zu ", n->strides[i]);
    printf("\n");

    print_ndarray_data(n);

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

void reshape(Ndarray *n, size_t *new_shape)
{
    // set shape change and then redo stride calculation
    n->shape = new_shape;
    
    size_t current_stride = n->itemsize;

    for (size_t i = n->ndim; i-- > 0;) {
        n->strides[i] = current_stride;
        current_stride *= n->shape[i];
    }

}

Ndarray *element_wise_operation(Ndarray *n, void (*operation)(void *element)) 
{

    Ndarray *new_n = malloc(sizeof(Ndarray));
    memcpy(new_n, n, sizeof(Ndarray));

    new_n->data = malloc(n->length * n->itemsize);
    memcpy(new_n->data, n->data, (n->length * n->itemsize));

    new_n->shape = malloc(n->ndim * sizeof(size_t));
    new_n->strides = malloc(n->ndim * sizeof(size_t));

    for (size_t i = 0; i < new_n->ndim; i++) {
        new_n->shape[i] = n->shape[i];
        new_n->strides[i] = n->strides[i];
    }


    for (size_t i = 0; i < new_n->length; i++) {
        // call this operation on each element
        uint8_t *element = new_n->data + (i * new_n->itemsize);
        operation(element);
    }

    return new_n;
}

void transpose(Ndarray *n)
{

    for (size_t i = 0; i < n->ndim / 2; i++) {
        size_t temp_shape = n->shape[i];
        n->shape[i] = n->shape[n->ndim - i - 1];
        n->shape[n->ndim - i - 1] = temp_shape;

        size_t temp_stride = n->strides[i];
        n->strides[i] = n->strides[n->ndim - i - 1];
        n->strides[n->ndim - i - 1] = temp_stride;
    }

}

Ndarray *flatten(Ndarray *n)
{
    Ndarray *new_n = malloc(sizeof(Ndarray));
    memcpy(new_n, n, sizeof(Ndarray));

    new_n->data = malloc(n->length * n->itemsize);
    memcpy(new_n->data, n->data, (n->length * n->itemsize));

    new_n->ndim = 1;

    new_n->shape = malloc(new_n->ndim * sizeof(size_t));
    new_n->strides = malloc(new_n->ndim * sizeof(size_t));

    new_n->shape[0] = n->length;
    new_n->strides[0] = n->itemsize;


    return new_n;
}


int64_t reduce_sum(Ndarray *n)
{
    int64_t sum = 0;
    for (size_t i = 0; i < n->length; i++) {
        size_t index = i * n->itemsize;
        sum += n->data[index];
    }

    return sum;
}

int64_t reduce_mean(Ndarray *n)
{
    int64_t sum = 0;
    for (size_t i = 0; i < n->length; i++) {
        size_t index = i * n->itemsize;
        sum += n->data[index];
    }

    int64_t mean = sum / n->length;

    return mean;
}

