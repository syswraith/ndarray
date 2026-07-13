#include "../include/ndarray.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void square_int32(void *element) {
    int32_t *val = (int32_t *)element;
    *val = (*val) * (*val); 
}

int main(void) {

    int32_t data[] = {
        1,  2,  3, 
        4, 5,  6,  
        7,  8, 9, 
        10, 11, 12
    };

    size_t shape[] = {4, 3};
    Ndarray *nigga = create_ndarray(data, 2, shape, sizeof(int32_t));
    printer(nigga);
    printf("\n");
    transpose(nigga);
    printer(nigga);
    delete_ndarray(nigga);

    return 0;
}
