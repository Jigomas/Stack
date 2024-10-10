#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ver-tor.h"
#include "push-pop.h"
#include "constructor-destructor.h"
#include "reader.h"

void EndingFunctionOfProgram(struct stk_t *stk, int exit_code);

int RunOfProcessor(struct stk_t *stk, int exit_code);



int main() {

    /**********************************************************************************************/
    //Starting
    int exit_code = ALL_GOOD;
    int amount_to_push = 5;
    int amount_to_pop =  3;
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
    //EndOfStarting part
    /**********************************************************************************************/

    RunOfProcessor(&stk, exit_code);





    EndingFunctionOfProgram(&stk, exit_code);
}



void EndingFunctionOfProgram(struct stk_t *stk, int exit_code) {
    StkVerifier(stk);
    printf("****************************************\n");
    printf("Programm ended with stack of %d elements\n", stk->size);

    DEBUG(StkDumper(stk, __FILE__, __LINE__);)

    exit_code = StkDtor(stk);
    DEBUG(printf("Programm ended with Exit Code %d", exit_code);)
}



int RunOfProcessor(struct stk_t *stk, int exit_code) {
    int instr_pnt = 0;
    stk_elem_t all_instructions[0] = {};//todo receiver
    while (1) {
        switch ( *(int*)() )  {
            case PUSH: {
                StkPush(stk, )
            }

            case ADD: {

            }

            case SUB: {

            }

            case MULT: {

            }

            case DIV: {

            }

            case OUT: {

            }

            case IN: {

            }

            case SQRT: {

            }

            case SIN: {

            }

            case COS: {

            }

            case DUMP: {

            }

            case HALT: {

            }
        }
    }
}
