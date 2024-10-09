#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ver-tor.h"



int StkVerifier(struct stk_t *stk) {
    DEBUG(
        int exit_code = ALL_GOOD;

        if (stk == NULL) {
            exit_code = ERROR;

            printf("stk is equal to NULL\n");
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->data == NULL) {
            exit_code = ERROR;

            printf("data is equal to NULL\n");
            StkDumper(stk, __FILE__, __LINE__, 0);
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->size < 0) {
            exit_code = ERROR;

            printf("size is less than 0\n");
            StkDumper(stk, __FILE__, __LINE__, 0);
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->capacity < 0) {
            exit_code = ERROR;

            printf("capacity is less than 0\n");
            StkDumper(stk, __FILE__, __LINE__, 0);
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->size > stk->capacity) {
            exit_code = ERROR;

            printf("size is bigger than capacity\n");
            StkDumper(stk, __FILE__, __LINE__, 0);
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if ( (*(stk->data + stk->capacity * sizeof(stk_elem_t) +
                    sizeof(canary_t)))  != *(stk->data) || *(stk->data) != stk->canary ) {
            exit_code = ERROR;

            printf("canary of data is damaged\n");
            StkDumper(stk, __FILE__, __LINE__, 0);
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->stk_l_canary != stk->stk_r_canary || stk->stk_l_canary != stk->canary) {
            exit_code = ERROR;

            printf("canary of struct is damaged\n");
            StkDumper(stk, __FILE__, __LINE__, 0);
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }


        if (stk->hash != stk->hash_after) {
            exit_code = ERROR;

            printf("damaged hash of data\n");
            StkDumper(stk, __FILE__, __LINE__, 0);
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }


        //if (stk->stk_hash != StkStructCountHash(stk, stk->canary)) {
        //    exit_code = ERROR;

        //    printf("damaged hash of struct\n");
        //    StkDumper(stk, __FILE__, __LINE__, 0);
        //    StkAssertionFunc(stk, exit_code);

        //    return exit_code;
        //}
    )
    return ALL_GOOD;
}



void StkAssertionFunc(struct stk_t *stk, int exit_code) {
    if (!exit_code) {
        abort(); // and there write the line with error
    }
}



void StkDumper(struct stk_t *stk, const char* line, double file, poison_elem_t poison_elem) {
    //StkVerifier(stk);
    printf("\n****************************************\n");
    printf("File: %d Line: %d\n", file, line);
    printf("capacity = %d\n",       stk->capacity);
    printf("size     = %d\n",       stk->size);
    printf("\n");

    DEBUG(printf("Hash            = %lf\n", stk->hash) ;)
    DEBUG(printf("Hash after      = %lf\n", stk->hash_after) ;)

    DEBUG(printf("Struct Hash     = %lf\n", stk->stk_hash) ;)

    DEBUG(printf("Canary          = %lf\n", stk->canary) ;)
    DEBUG(printf("LCanary of stk  = %lf\n", stk->stk_l_canary) ;)
    DEBUG(printf("RCanary of stk  = %lf\n", stk->stk_r_canary) ;)
    DEBUG(printf("LCanary of data = %lf\n", *(stk)->data) ;)
    DEBUG(printf("RCanary of data = %lf\n", *(stk->data + stk->capacity * sizeof(stk_elem_t)
                                                        + sizeof(canary_t) )) ;)
    printf("\n");

    for (int i = 0; i < stk->capacity; i++) {
        //if (*(stk->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) != poison_elem)
            printf("data[%d] = %lf\n", i + 1, *(stk->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ));
    }
    printf("\n");

}


stk_elem_t StkCountHash(struct stk_t *stk, DEBUG(canary_t canary)) { // make some functions static i
    //StkVerifier(stk);
    stk_elem_t hash_summ = 5381;
    DEBUG(
        for (int i = 0; i < stk->size; i++) {
            hash_summ = (hash_summ + 31) + *(stk->data + i * sizeof(stk_elem_t) + sizeof(canary_t));
            // xor must be here
        }
    )
    return hash_summ;
}

stk_elem_t StkStructCountHash(struct stk_t *stk, DEBUG(canary_t canary)) {
    //StkVerifier(stk);
    DEBUG(
    stk_elem_t hash_summ = 5381 + stk->canary + stk->size + stk->capacity + stk->hash + stk->hash_after
                                + stk->stk_l_canary + stk->stk_r_canary;
    return hash_summ;
    )
}
