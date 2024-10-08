#ifndef PUSHPOP
#define PUSHPOP

#include "header.h"

int StkPush(struct stk_t *stk, stk_elem_t new_element, poison_elem_t poison_elem, DEBUG(canary_t canary));

int StkPop(struct stk_t *stk, poison_elem_t poison_elem, DEBUG(canary_t canary));

void StkFreeMem(struct stk_t **stk, DEBUG(canary_t canary));

void StkAddMem(struct stk_t **stk, poison_elem_t poison_elem, DEBUG(canary_t canary));

#endif // PUSHPOP
