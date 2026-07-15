#include "../include/ndarray.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

int main(void)
{
    int32_t data[] = {
        1,  2,  3,
        4,  5,  6,
        7,  8,  9,
        10, 11, 12
    };

    size_t shape[] = {4, 3};

    Ndarray *arr = create_ndarray(data, 2, shape, INT32);

    printer(arr);

    printf("\n");
    printf("sum     : %" PRId64 "\n", reduce_sum(arr));
    printf("mean    : %.2f\n", reduce_mean(arr));
    printf("min     : %" PRId64 "\n", reduce_min(arr));
    printf("max     : %" PRId64 "\n", reduce_max(arr));
    printf("argmin  : %zu\n", reduce_argmin(arr));
    printf("argmax  : %zu\n", reduce_argmax(arr));    delete_ndarray(arr);

    return 0;
}
