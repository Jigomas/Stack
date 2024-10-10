#ifndef PUSHPOP
#define PUSHPOP

#include "header.h"

int StkPush(struct stk_t *stk, stk_elem_t new_element);

int StkPop(struct stk_t *stk);

void StkFreeMem(struct stk_t **stk);

void StkAddMem(struct stk_t **stk);

#endif // PUSHPOP
