#include "../include/ndarray.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {

    int32_t data[] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12
    };

    size_t shape[] = {3, 4};

    Ndarray *nigga = create_ndarray(data, 2, shape, sizeof(int));

    size_t indices[] = {2,2};

    int32_t *index = index_at(nigga, 2, indices);

    printf("%d\n", *index);

    delete_ndarray(nigga);

    return 0;
}
