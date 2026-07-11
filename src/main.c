#include "../include/ndarray.h"
#include <stdio.h>

int main(void) {

    uint8_t data[] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12
    };

    uint8_t data2[] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 12, 12
    };

    size_t shape[] = {3, 4};
    size_t shape2[] = {4, 3};

    Ndarray *nigga1 = create_ndarray(data, 2, shape, sizeof(int));
    Ndarray *nigga2 = create_ndarray(data2, 2, shape2, sizeof(int));

    printf(check_equal(nigga1, nigga2)?"True\n":"False\n");
    delete_ndarray(nigga1);
    delete_ndarray(nigga2);

    return 0;
}
