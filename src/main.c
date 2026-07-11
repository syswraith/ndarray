#include "../include/ndarray.h"

int main(void) {

    uint8_t data[] = {
        1,  2,  3,  4,
        5,  6,  7,  8,
        9, 10, 11, 12
    };

    size_t shape[] = {3, 4};

    Ndarray *nigga = create_ndarray(data, 2, shape, sizeof(int));
    printer(nigga);
    delete_ndarray(nigga);

    return 0;
}
