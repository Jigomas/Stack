#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "reader.h"



stk_elem_t Reader() {
    stk_elem_t received_num = 0;
    int had_read_sth = 0;

    printf("\nPls write a number\n");

    while (!had_read_sth) {
        printf(">> ");
        had_read_sth = scanf(" %lf", &received_num);
        if (!had_read_sth) {
            printf("Try again\n");
            break;
        }
    }

    return received_num;
}
