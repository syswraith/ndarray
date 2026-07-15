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

    printf("Original array:\n");
    printer(arr);

    printf("\n");

    printf("sum     : %" PRId64 "\n", reduce_sum(arr));
    printf("mean    : %.2f\n", reduce_mean(arr));
    printf("min     : %" PRId64 "\n", reduce_min(arr));
    printf("max     : %" PRId64 "\n", reduce_max(arr));
    printf("argmin  : %zu\n", reduce_argmin(arr));
    printf("argmax  : %zu\n", reduce_argmax(arr));

    printf("\nConverting to FLOAT64...\n\n");

    Ndarray *float_arr = astype(arr, FLOAT64);

    printer(float_arr);

    printf("\n");

    printf("sum     : %" PRId64 "\n", reduce_sum(float_arr));
    printf("mean    : %.2f\n", reduce_mean(float_arr));
    printf("min     : %" PRId64 "\n", reduce_min(float_arr));
    printf("max     : %" PRId64 "\n", reduce_max(float_arr));
    printf("argmin  : %zu\n", reduce_argmin(float_arr));
    printf("argmax  : %zu\n", reduce_argmax(float_arr));

    delete_ndarray(arr);
    delete_ndarray(float_arr);

    return 0;
}
