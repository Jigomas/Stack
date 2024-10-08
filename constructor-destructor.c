#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ver-tor.h"
#include "constructor-destructor.h"


int StkCtor(struct stk_t *stk, unsigned int capacity, poison_elem_t poison_elem,
                            DEBUG( canary_t canary)) {   // gonna produce stk
    DEBUG(
        stk->stk_l_canary = canary;
        stk->stk_r_canary = canary;
        stk->canary       = canary;

        stk->hash         = 5381;
        stk->hash_after   = 5381;
    )

    stk->size = 0;
    stk->capacity = capacity;

    stk->data = (stk_elem_t *)realloc(stk->data, capacity * sizeof(stk_elem_t) +
                                                            DEBUG(2 * sizeof(canary_t)));


    DEBUG(*(stk->data) = canary;)
    DEBUG(*(stk->data + stk->capacity * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = canary;)

    for (int i = 0; i < capacity; i++)
        *(stk->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = poison_elem;

    DEBUG(stk->stk_hash = StkStructCountHash(stk, stk->canary);)

    return(StkVerifier(stk));
}


int StkDtor(struct stk_t *stk) {
    int exit_code = StkVerifier(stk);

    for (int i = 0; i < (stk->size); i++)
        *(stk->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = 0;

    stk->capacity = 0;
    stk->size = 0;

    free(stk->data);
    return exit_code;
}

