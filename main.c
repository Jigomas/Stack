#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ver-tor.h"
#include "push-pop.h"
#include "constructor-destructor.h"
#include "reader.h"


int main() {
    int exit_code = ALL_GOOD;
    int amount_to_push = 10;
    int amount_to_pop =  8;
    unsigned int capacity = 5;    // this is gonna be starting capacity


    poison_elem_t poison_elem = 0; // not bigger than  stk_elem_t
    DEBUG(canary_t canary = 0;)
    DEBUG(
            switch (sizeof(poison_elem) ) {
            case sizeof(double): {
                poison_elem = -6666;
                canary = 123456789;
                break;
            }

            case sizeof(int): {
                poison_elem = -666;
                canary = 12345678;

                break;
            }

            case sizeof(short): {
                poison_elem = -66;
                canary = 1234567;

                break;
            }

            case sizeof(char): {
                poison_elem = -6;
                canary = 1;

                break;
            }

            default: {
                poison_elem = -0;
                canary = 1;

                break;
            }
        }
    )

    DEBUG(printf ("Poison Element = %lf\n", poison_elem);)
    struct stk_t stk = {};


    exit_code *= StkCtor(&stk, capacity, poison_elem, DEBUG(canary));
    DEBUG(exit_code *= StkVerifier(&stk);)
    //Start


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

