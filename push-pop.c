#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ver-tor.h"
#include "push-pop.h"
#include "header.h"

int StkPush(struct stk_t *stk, stk_elem_t new_element, poison_elem_t poison_elem, DEBUG(canary_t canary)) {
    int exit_code = StkVerifier(stk);

    if (!exit_code) return exit_code;

    StkAddMem(&stk, poison_elem, DEBUG(canary) );
    DEBUG(printf("new_element %lf\n", new_element);)

    *(stk->data + stk->size * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = new_element;
    stk->size ++;

    //printf("size after %d\n", stk->size);

    DEBUG(stk->hash_after = (stk->hash + 31) + new_element;)
    DEBUG(stk->hash       = StkCountHash(stk, stk->canary);)

    DEBUG(stk->stk_hash   = StkStructCountHash(stk, canary);)

    exit_code *= StkVerifier(stk);

    return exit_code;
}



void StkAddMem(struct stk_t **stk, poison_elem_t poison_elem, DEBUG(canary_t canary) ) {
    int exit_code = StkVerifier(*stk);

    if ((*stk)->size == (*stk)->capacity) {
        size_t new_capacity = (*stk)->capacity ? (*stk)->capacity * 2 : 1; // Start with 1 if capacity is 0

        (*stk)->data =
                    (stk_elem_t *)realloc((*stk)->data, new_capacity * sizeof(stk_elem_t));

        for (int i = (*stk)->size; i < new_capacity; i++)
              *((*stk)->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = poison_elem;

        if ((*stk)->data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        (*stk)->capacity = new_capacity; // Update capacity

        DEBUG(*((*stk)->data + (*stk)->capacity * sizeof(stk_elem_t) + sizeof(canary_t) ) = canary;)
    }
}



int StkPop(struct stk_t *stk, poison_elem_t poison_elem, DEBUG(canary_t canary)) {
    int exit_code = StkVerifier(stk);

    if (!exit_code || stk->size <= 0) return exit_code;
    stk->size --;

    DEBUG(stk->hash_after = (stk->hash - 31) - *(stk->data + stk->size * sizeof(stk_elem_t) + sizeof(canary_t));)
    DEBUG(stk->hash      = StkCountHash(stk, stk->canary);)
    DEBUG(stk->stk_hash   = StkStructCountHash(stk, canary);)

    StkFreeMem(&stk, DEBUG(canary));

    printf("Pop elemenent = %lf\n", *(stk->data + stk->size * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ));
    *(stk->data + stk->size * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = poison_elem;


    return exit_code;
}



void StkFreeMem(struct stk_t **stk, DEBUG(canary_t canary)) {
    int exit_code = StkVerifier(*stk);

    if ((*stk)->size <= 0.25 * (*stk)->capacity) {
        size_t new_capacity = (*stk)->capacity / 2;

        stk_elem_t *new_data =
                    (stk_elem_t *)realloc((*stk)->data, new_capacity * sizeof(stk_elem_t) DEBUG(+ 2 * sizeof(canary_t)));

        if (new_data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        (*stk)->data = new_data;
        (*stk)->capacity = new_capacity; // Update capacity

        DEBUG(*((*stk)->data + (*stk)->capacity * sizeof(stk_elem_t) + sizeof(canary_t) ) = canary;)
    }
}
