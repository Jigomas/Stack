#ifndef VERIFICATOR
#define VERIFICATOR

#include "header.h"

int StkVerifier(struct stk_t *stk);

void StkAssertionFunc(struct stk_t *stk, int exit_code);

void StkDumper(struct stk_t *stk, const char* line, double file);

stk_elem_t StkCountHash(struct stk_t *stk);

stk_elem_t StkStructCountHash(struct stk_t *stk);

#endif // VERIFICATOR
