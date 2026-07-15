#include <stdint.h>
#include <stdlib.h>

typedef enum {
    UINT8,
    UINT16,
    UINT32,
    UINT64,

    INT8,
    INT16,
    INT32,
    INT64,

    FLOAT32,
    FLOAT64
} DType;

typedef struct ndarray {
    // to get from user
    uint8_t *data; // malloc(length * itemsize) at the end
    size_t ndim; // length of shape[] which is dynamically allocated
                 // like length of shape
    size_t *shape; // actual input
    DType dtype; // enum name
    size_t itemsize; // actual size
    // calculable
    size_t *strides;
    size_t length;

    // methods

} Ndarray;


int check_equal(Ndarray *n1, Ndarray *n2);

Ndarray *create_ndarray(
    void *data,
    size_t ndim,
    size_t *shape,
    DType dtype
);

void delete_ndarray(Ndarray *n);
void printer(Ndarray *n);
void *index_at(Ndarray *n, size_t indices_size, size_t *indices);
void reshape(Ndarray *n, size_t *new_shape);
Ndarray *element_wise_operation(Ndarray *n, void (*operation)(void *element));
void transpose(Ndarray *n);
Ndarray *flatten(Ndarray *n);

int64_t reduce_sum(Ndarray *n);
double  reduce_mean(Ndarray *n);
int64_t reduce_min(Ndarray *n);
int64_t reduce_max(Ndarray *n);
size_t  reduce_argmin(Ndarray *n);
size_t  reduce_argmax(Ndarray *n);

int64_t get_as_int64(Ndarray *n, size_t index);
size_t  dtype_size(DType dtype);
