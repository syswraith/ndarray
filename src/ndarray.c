#include "../include/ndarray.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

size_t dtype_size(DType dtype)
{
    switch (dtype) {
        case UINT8:   return sizeof(uint8_t);
        case UINT16:  return sizeof(uint16_t);
        case UINT32:  return sizeof(uint32_t);
        case UINT64:  return sizeof(uint64_t);

        case INT8:    return sizeof(int8_t);
        case INT16:   return sizeof(int16_t);
        case INT32:   return sizeof(int32_t);
        case INT64:   return sizeof(int64_t);

        case FLOAT32: return sizeof(float);
        case FLOAT64: return sizeof(double);

        default:      return 0;
    }
}

Ndarray *create_ndarray(
        void *data,
        size_t ndim,
        size_t *shape,
        DType dtype
        )
{
    Ndarray   *n =         malloc(sizeof(Ndarray));

    n->ndim     = ndim;       // 3
    n->shape = malloc(n->ndim * sizeof(size_t));

    for (size_t i = 0; i < n->ndim; i++)
        n->shape[i] = shape[i];

    n->length   = 1;
    n->strides  = malloc(ndim *  sizeof(size_t));

    for (size_t i = 0; i < ndim; i++)
        n->length *= shape[i];

    n->dtype = dtype;

    n->itemsize = dtype_size(dtype);
    n->data = malloc(n->length * n->itemsize);
    memcpy(n->data, data, (n->length * n->itemsize));

    size_t current_stride = n->itemsize;

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
    size_t new_length = 1;

    for (size_t i = 0; i < n->ndim; i++)
        new_length *= new_shape[i];

    if (new_length != n->length)
        return;

    for (size_t i = 0; i < n->ndim; i++)
        n->shape[i] = new_shape[i];

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

int64_t get_as_int64(Ndarray *n, size_t index)
{
    switch (n->dtype) {

        case UINT8:
            return ((uint8_t *)n->data)[index];

        case UINT16:
            return ((uint16_t *)n->data)[index];

        case UINT32:
            return ((uint32_t *)n->data)[index];

        case UINT64:
            return ((uint64_t *)n->data)[index];

        case INT8:
            return ((int8_t *)n->data)[index];

        case INT16:
            return ((int16_t *)n->data)[index];

        case INT32:
            return ((int32_t *)n->data)[index];

        case INT64:
            return ((int64_t *)n->data)[index];

        case FLOAT32:
            return (int64_t)((float *)n->data)[index];

        case FLOAT64:
            return (int64_t)((double *)n->data)[index];

        default:
            return 0;
    }
}

int64_t reduce_sum(Ndarray *n)
{
    int64_t sum = 0;

    for (size_t i = 0; i < n->length; i++)
        sum += get_as_int64(n, i);

    return sum;
}

double reduce_mean(Ndarray *n)
{
    if (n->length == 0)
        return 0.0;

    return (double)reduce_sum(n) / n->length;
}

int64_t reduce_min(Ndarray *n)
{
    int64_t min = get_as_int64(n, 0);

    for (size_t i = 1; i < n->length; i++) {
        int64_t value = get_as_int64(n, i);

        if (value < min)
            min = value;
    }

    return min;
}

int64_t reduce_max(Ndarray *n)
{
    int64_t max = get_as_int64(n, 0);

    for (size_t i = 1; i < n->length; i++) {
        int64_t value = get_as_int64(n, i);

        if (value > max)
            max = value;
    }

    return max;
}

size_t reduce_argmin(Ndarray *n)
{
    size_t argmin = 0;
    int64_t min = get_as_int64(n, 0);

    for (size_t i = 1; i < n->length; i++) {
        int64_t value = get_as_int64(n, i);

        if (value < min) {
            min = value;
            argmin = i;
        }
    }

    return argmin;
}

size_t reduce_argmax(Ndarray *n)
{
    size_t argmax = 0;
    int64_t max = get_as_int64(n, 0);

    for (size_t i = 1; i < n->length; i++) {
        int64_t value = get_as_int64(n, i);

        if (value > max) {
            max = value;
            argmax = i;
        }
    }

    return argmax;
}

Ndarray *astype(Ndarray *n, DType new_dtype)
{
    Ndarray *new_n = malloc(sizeof(Ndarray));

    new_n->ndim = n->ndim;
    new_n->length = n->length;
    new_n->dtype = new_dtype;
    new_n->itemsize = dtype_size(new_dtype);

    new_n->shape = malloc(new_n->ndim * sizeof(size_t));
    new_n->strides = malloc(new_n->ndim * sizeof(size_t));

    for (size_t i = 0; i < new_n->ndim; i++)
        new_n->shape[i] = n->shape[i];

    size_t current_stride = new_n->itemsize;

    for (size_t i = new_n->ndim; i-- > 0;) {
        new_n->strides[i] = current_stride;
        current_stride *= new_n->shape[i];
    }

    new_n->data = malloc(new_n->length * new_n->itemsize);

    for (size_t i = 0; i < new_n->length; i++) {
        int64_t value = get_as_int64(n, i);

        switch (new_dtype) {
            case UINT8:
                ((uint8_t *)new_n->data)[i] = (uint8_t)value;
                break;
            case UINT16:
                ((uint16_t *)new_n->data)[i] = (uint16_t)value;
                break;
            case UINT32:
                ((uint32_t *)new_n->data)[i] = (uint32_t)value;
                break;
            case UINT64:
                ((uint64_t *)new_n->data)[i] = (uint64_t)value;
                break;
            case INT8:
                ((int8_t *)new_n->data)[i] = (int8_t)value;
                break;
            case INT16:
                ((int16_t *)new_n->data)[i] = (int16_t)value;
                break;
            case INT32:
                ((int32_t *)new_n->data)[i] = (int32_t)value;
                break;
            case INT64:
                ((int64_t *)new_n->data)[i] = (int64_t)value;
                break;
            case FLOAT32:
                ((float *)new_n->data)[i] = (float)value;
                break;
            case FLOAT64:
                ((double *)new_n->data)[i] = (double)value;
                break;
        }
    }

    return new_n;
}
