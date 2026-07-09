#include <stdint.h>
#include <stdlib.h>

typedef struct {
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
} Ndarray;



Ndarray *create_ndarray(
    uint8_t *data, size_t data_size,
    size_t ndim,
    size_t *shape,
    uint16_t dtype,
    size_t itemsize
);

void delete_ndarray(Ndarray *n);
void printer(Ndarray *n);

