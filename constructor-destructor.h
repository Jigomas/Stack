#ifndef CTORDTOR
#define CTORDTOR

#include "header.h"

int StkCtor(struct stk_t *stk, unsigned int capacity, poison_elem_t poison_elem, DEBUG(canary_t canary));

int StkDtor(struct stk_t *stk);

#endif //CTORDTOR
