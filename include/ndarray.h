#include <stdint.h>
#include <stdlib.h>

typedef struct ndarray {
    // to get from user
    uint8_t *data; // malloc(length * itemsize) at the end
    size_t ndim; // length of shape[] which is dynamically allocated
                 // like length of shape
    size_t *shape; // actual input
    uint16_t dtype; // enum name
    size_t itemsize; // actual size
    // calculatable
    size_t *strides;
    size_t length;

    // methods

} Ndarray;


int check_equal(Ndarray *n1, Ndarray *n2);

Ndarray *create_ndarray(
    void *data,
    size_t ndim,
    size_t *shape,
//    uint16_t dtype,
    size_t itemsize
);

void delete_ndarray(Ndarray *n);
void printer(Ndarray *n);
void *index_at(Ndarray *n, size_t indices_size, size_t *indices);
void reshape(Ndarray *n, size_t *new_shape);
Ndarray *element_wise_operation(Ndarray *n, void (*operation)(void *element));
void transpose(Ndarray *n);
Ndarray *flatten(Ndarray *n);


int64_t reduce_sum(Ndarray *n);
int64_t reduce_mean(Ndarray *n);
