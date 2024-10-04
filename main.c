#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#

typedef double stk_elem_t;
typedef stk_elem_t poison_elem_t; //first is size not bigger than  stk_elem_t
typedef stk_elem_t canary_t;      //first is size

/*
#define DEBUG_MODE 1
*/


#define DEBUG_MODE 1

#if DEBUG_MODE == 1
    #define DEBUG(a) a
#else
    #define DEBUG(a)
#endif



struct stk_t {
    DEBUG(canary_t stk_r_canary);

    stk_elem_t *data;
    unsigned int size;
    unsigned int capacity;

    DEBUG(stk_elem_t hash);
    DEBUG(stk_elem_t hash_after);

    DEBUG(canary_t canary);

    DEBUG(canary_t stk_l_canary);
};

enum exit_codeS {
    ALL_GOOD = 1,
    ERROR = 0,
};



int StkCtor(struct stk_t *stk, unsigned int capacity, poison_elem_t poison_elem, DEBUG(canary_t canary));

int StkDtor(struct stk_t *stk);

void StkDumper(struct stk_t *stk, const char* file, double line, poison_elem_t poison_elem);

int StkVerifier(struct stk_t *stk);

int StkPush(struct stk_t *stk, stk_elem_t new_element, poison_elem_t poison_elem, DEBUG(canary_t canary));

int StkPop(struct stk_t *stk, poison_elem_t poison_elem, DEBUG(canary_t canary));

void StkFreeMem(struct stk_t **stk, DEBUG(canary_t canary));

void StkAddMem(struct stk_t **stk, poison_elem_t poison_elem, DEBUG(canary_t canary));

stk_elem_t StkCountHash(struct stk_t *stk, DEBUG(canary_t canary));

stk_elem_t Reader();

void StkAssertionFunc(struct stk_t *stk, int exit_code);



int main() {
    int exit_code = ALL_GOOD;
    int amount_to_push = 5;
    int amount_to_pop =  3;

    DEBUG(canary_t canary = 123456789;)

    poison_elem_t poison_elem = 0; // not bigger than  stk_elem_t

    DEBUG(
            switch (sizeof(poison_elem) ) {
            case sizeof(double): {
                poison_elem = -6666;
                break;
            }

            case sizeof(int): {
                poison_elem = -666;
                break;
            }

            case sizeof(short): {
                poison_elem = -66;
                break;
            }

            case sizeof(char): {
                poison_elem = -6;
                break;
            }

            default: {
                poison_elem = -0;
                break;
            }
        }
    )

    DEBUG(printf ("Poison Element = %lf\n", poison_elem);)
    struct stk_t stk = {};

    unsigned int capacity = 5;    // this is gonna be starting capacity

    exit_code *= StkCtor(&stk, capacity, poison_elem, DEBUG(canary));
    DEBUG(exit_code *= StkVerifier(&stk);)



    for (int i = 0; i < amount_to_push/*How many do u want to Push*/; i++) {
        DEBUG(exit_code *= StkVerifier(&stk);)
        stk_elem_t new_element = Reader();

        exit_code *= StkPush(&stk,  new_element, poison_elem, DEBUG(canary));
    }

    printf("Programm started with stack of %d elements\n", stk.size);
    DEBUG(StkDumper(&stk, __FILE__, __LINE__, poison_elem);)

    // Using pop
    for (int i = 0; i < amount_to_pop/*How many do u want to pop*/; i++) {
        DEBUG(exit_code *= StkVerifier(&stk);)

        exit_code *= StkPop(&stk, poison_elem, DEBUG(canary));

        DEBUG(
            StkDumper(&stk, __FILE__, __LINE__, poison_elem);
        )
    }
    printf("****************************************\n");
    printf("Programm ended with stack of %d elements\n", stk.size);

    DEBUG(StkDumper(&stk, __FILE__, __LINE__, poison_elem);)



    exit_code *= StkDtor(&stk);
    DEBUG(printf("Programm ended with Exit Code %d", exit_code);)
}



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
    return(StkVerifier(stk));
}



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
    DEBUG(stk->hash         = StkCountHash(stk, stk->canary);)

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



int StkDtor(struct stk_t *stk) {
    int exit_code = StkVerifier(stk);

    for (int i = 0; i < (stk->size); i++)
        *(stk->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) = 0;

    stk->capacity = 0;
    stk->size = 0;

    free(stk->data);
    return exit_code;
}



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
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->size < 0) {
            exit_code = ERROR;

            printf("size is less than 0\n");
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->capacity < 0) {
            exit_code = ERROR;

            printf("capacity is less than 0\n");
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->size > stk->capacity) {
            exit_code = ERROR;

            printf("size is bigger than capacity\n");
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if ( (*(stk->data + stk->capacity * sizeof(stk_elem_t) +
                    sizeof(canary_t)))  != *(stk->data) || *(stk->data) != stk->canary ) {
            exit_code = ERROR;

            printf("canary of data is damaged\n");
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

        if (stk->stk_l_canary != stk->stk_r_canary || stk->stk_l_canary != stk->canary) {
            exit_code = ERROR;

            printf("canary of struct is damaged\n");
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }


        if (stk->hash != stk->hash_after) {
            exit_code = ERROR;

            printf("damaged hash\n");
            StkAssertionFunc(stk, exit_code);

            return exit_code;
        }

    )
    return ALL_GOOD;
}



void StkAssertionFunc(struct stk_t *stk, int exit_code) {
    if (!exit_code) {
        abort(); // and there write the line with error
    }
}



void StkDumper(struct stk_t *stk, const char* line, double file, poison_elem_t poison_elem) {
    printf("\n****************************************\n");
    printf("File: %d Line: %d\n", file, line);
    printf("capacity = %d\n",       stk->capacity);
    printf("size     = %d\n",       stk->size);
    printf("\n");

    DEBUG(printf("Hash            = %lf\n", stk->hash) ;)
    DEBUG(printf("Hash after      = %lf\n", stk->hash_after) ;)


    DEBUG(printf("Canary          = %lf\n", stk->canary) ;)
    DEBUG(printf("LCanary of stk  = %lf\n", stk->stk_l_canary) ;)
    DEBUG(printf("RCanary of stk  = %lf\n", stk->stk_r_canary) ;)
    DEBUG(printf("LCanary of data = %lf\n", *(stk)->data) ;)
    DEBUG(printf("RCanary of data = %lf\n", *(stk->data + stk->capacity * sizeof(stk_elem_t)
                                                        + sizeof(canary_t) )) ;)
    printf("\n");

    for (int i = 0; i < stk->capacity; i++) {
        if (*(stk->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ) != poison_elem)
            printf("data[%d] = %lf\n", i + 1, *(stk->data + i * sizeof(stk_elem_t) DEBUG(+ sizeof(canary_t)) ));
    }
    printf("\n");

}


stk_elem_t StkCountHash(struct stk_t *stk, DEBUG(canary_t canary)) {
    // dunno how to verify it           StkVerifier(stk);
    stk_elem_t hash_summ = 5381;
    DEBUG(
        for (int i = 0; i < stk->size; i++) {
            hash_summ = (hash_summ + 31)    +     *(stk->data + i * sizeof(stk_elem_t) + sizeof(canary_t));
            // xor must be here
        }
    )
    return hash_summ;
}


stk_elem_t Reader()
{
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
