#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ver-tor.h"
#include "push-pop.h"
#include "header.h"

int StkPush(struct stk_t *stk, stk_elem_t new_element) {
    int exit_code = StkVerifier(stk);

    if (!exit_code) return exit_code;

    StkAddMem(&stk);
    DEBUG(printf("new_element %lf\n", new_element);)

    *(stk_elem_t *)((char *)stk->data + stk->size * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = new_element;
    stk->size ++;


    DEBUG(stk->hash_after = (stk->hash + 31) + new_element;)
    DEBUG(stk->hash       = StkCountHash(stk);)
    DEBUG(stk->stk_hash   = StkStructCountHash(stk);)

    exit_code *= StkVerifier(stk);
    StkDumper(stk, __FILE__, __LINE__);
    return exit_code;
}



void StkAddMem(struct stk_t **stk) {
    int exit_code = StkVerifier(*stk);

    stk_elem_t * Temp = (stk_elem_t *)malloc((*stk)->capacity * sizeof(stk_elem_t));

    for (int i = 0; i < (*stk)->size; i++)
              Temp[i] = *(stk_elem_t *)((char *)(*stk)->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) );

    if ((*stk)->size == (*stk)->capacity) {

        unsigned int new_capacity = (*stk)->capacity ? (*stk)->capacity * 2 : 1; // Start with 1 if capacity is 0

        (*stk)->data =
                    (stk_elem_t *)realloc((*stk)->data, new_capacity * sizeof(stk_elem_t) DEBUG(+ 2 * sizeof(canary_t)));

        for (int i = (*stk)->size; i < new_capacity; i++)
              *(stk_elem_t *)((char *)(*stk)->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = (*stk)->poison_elem;

        if ((*stk)->data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        for (int i = 0; i < (*stk)->size; i++)
            *(stk_elem_t *)((char *)(*stk)->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = Temp[i];

        (*stk)->capacity = new_capacity; // Update capacity


        printf("realloc finished \n\n\n\n");
        StkDumper(*stk, __FILE__, __LINE__);


        DEBUG(*(stk_elem_t *)((char *)(*stk)->data + (*stk)->capacity * sizeof(stk_elem_t) + sizeof(canary_t) ) = (*stk)->canary;)
    }
}



int StkPop(struct stk_t *stk) {
    int exit_code = StkVerifier(stk);

    if (!exit_code || stk->size <= 0) return exit_code;
    stk->size --;

    DEBUG(stk->hash_after = (stk->hash - 31) - *(stk_elem_t *)((char *)stk->data + stk->size * sizeof(stk_elem_t) + sizeof(canary_t));)
    DEBUG(stk->hash      = StkCountHash(stk);)
    DEBUG(stk->stk_hash   = StkStructCountHash(stk);)

    StkFreeMem(&stk);

    printf("Pop elemenent = %lf\n", *(stk->data + stk->size * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ));
    *(stk_elem_t *)((char *)stk->data + stk->size * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = stk->poison_elem;


    return exit_code;
}



void StkFreeMem(struct stk_t **stk) {
    int exit_code = StkVerifier(*stk);

    if ((*stk)->size <= 0.25 * (*stk)->capacity) {
        stk_elem_t * Temp = (stk_elem_t *)malloc((*stk)->capacity * sizeof(stk_elem_t));
        for (int i = 0; i < (*stk)->size; i++)
            Temp[i] = *(stk_elem_t *)((char *)(*stk)->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) );

        size_t new_capacity = (*stk)->capacity / 2;
        stk_elem_t *new_data =
                    (stk_elem_t *)realloc((*stk)->data, new_capacity * sizeof(stk_elem_t) DEBUG(+ 2 * sizeof(canary_t)));

        if (new_data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        (*stk)->data = new_data;
        (*stk)->capacity = new_capacity; // Update capacity

        for (int i = 0; i < (*stk)->size; i++)
            *(stk_elem_t *)((char *)(*stk)->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = Temp[i];

        DEBUG(*(stk_elem_t *)((char *)(*stk)->data + (*stk)->capacity * sizeof(stk_elem_t) + sizeof(canary_t) ) = (*stk)->canary;)
    }
}
