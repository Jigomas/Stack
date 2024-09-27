#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>



typedef double Stk_Elem_t;

struct Stk_t {
    Stk_Elem_t *Data;
    unsigned int Size;
    unsigned int Capacity;
};

enum EXIT_CODES {
    SUCCESS = 2,
    ALL_GOOD = 1,
    ERROR = 0,
};



int Stk_Ctor(struct Stk_t *Stk, unsigned int Capacity);

int Stk_Dtor(struct Stk_t *Stk);

int Stk_Dumper();

int Stk_Verifier(struct Stk_t *Stk);

int Stk_Push(struct Stk_t *Stk, Stk_Elem_t New_Element);

int Stk_Pop(struct Stk_t *Stk);

void Stk_Free_Mem(struct Stk_t **Stk);

void Stk_Add_Mem(struct Stk_t **Stk);

Stk_Elem_t Reader();

void Stk_Assertion_Func(struct Stk_t *Stk, int Exit_Code);



int main() {
    int Exit_Code = ALL_GOOD;

    while (Exit_Code == ALL_GOOD) {
        struct Stk_t Stk = {};

        unsigned int Capacity = 5;    // this is gonna be starting Capacity



        Exit_Code *= Stk_Ctor(&Stk, Capacity);
        Stk_Assertion_Func(&Stk, Exit_Code);


        for (int i = 0; i < 10/*How many do u want to pop*/; i++) {
            Stk_Elem_t New_Element = Reader();
            Exit_Code *= Stk_Push(&Stk,  New_Element);
            Stk_Assertion_Func(&Stk, Exit_Code);
        }





        for (int i = 0; i < Stk.Capacity; i++)
            printf("%lf ", Stk.Data[i]);
        printf("\n");


        // Using pop
        for (int i = 0; i < 10/*How many do u want to pop*/; i++) {
            Exit_Code *= Stk_Pop(&Stk);
            Stk_Assertion_Func(&Stk, Exit_Code);


            for (int i = 0; i < Stk.Capacity; i++)
                printf("%lf ", Stk.Data[i]);

            printf("\n");
        }
        Exit_Code *= Stk_Dtor(&Stk);

        Exit_Code = SUCCESS;
        printf("%d", Exit_Code);
    }
}



int Stk_Ctor(struct Stk_t *Stk, unsigned int Capacity) {   // gonna produce Stk
    Stk->Size = 0;
    Stk->Capacity = Capacity;
    Stk->Data = (Stk_Elem_t *)calloc(Stk->Capacity, sizeof(Stk_Elem_t));

    return(Stk_Verifier(Stk));
}



int Stk_Push(struct Stk_t *Stk, Stk_Elem_t New_Element) {
    int Exit_Code = Stk_Verifier(Stk);

    if (!Exit_Code) return Exit_Code;

    Stk_Add_Mem(&Stk);
    printf("New_Element %lf\n", New_Element);
    //printf("Size b4 %d\n", Stk->Size);
    //printf("%d, %d\n", Stk->Size, Stk->Capacity);

    Stk->Data [Stk->Size] = New_Element;
    Stk->Size ++;

    //printf("Size after %d\n", Stk->Size);

    return Exit_Code;
}



void Stk_Add_Mem(struct Stk_t **Stk) {
    if ((*Stk)->Size == (*Stk)->Capacity) {
        size_t New_Capacity = (*Stk)->Capacity ? (*Stk)->Capacity * 2 : 1; // Start with 1 if capacity is 0
        Stk_Elem_t *New_Data = (Stk_Elem_t *)realloc((*Stk)->Data, New_Capacity * sizeof(Stk_Elem_t));

        for (int i = (*Stk)->Size; i < New_Capacity; i++)
              (*Stk)->Data [i] = 0;

        if (New_Data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        (*Stk)->Data = New_Data;
        (*Stk)->Capacity = New_Capacity; // Update capacity
    }
}



int Stk_Pop(struct Stk_t *Stk) {
    int Exit_Code = Stk_Verifier(Stk);

    if (!Exit_Code || Stk->Size <= 0) return Exit_Code;

    Stk->Size --;
    Stk_Free_Mem(&Stk);

    printf("POP %lf\n", Stk->Data [Stk->Size]);
    Stk->Data [Stk->Size] = 0;

    return Exit_Code;
}



void Stk_Free_Mem(struct Stk_t **Stk) {
    if ((*Stk)->Size <= 0.25 * (*Stk)->Capacity) {
        size_t New_Capacity = (*Stk)->Capacity / 2;

        Stk_Elem_t *New_Data = (Stk_Elem_t *)realloc((*Stk)->Data, New_Capacity * sizeof(Stk_Elem_t));

        if (New_Data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        (*Stk)->Data = New_Data;
        (*Stk)->Capacity = New_Capacity; // Update capacity
    }
}



int Stk_Dtor(struct Stk_t *Stk) {
    for (int i = 0; i < (Stk->Size); i++)
        Stk->Data [i] = 0;

    Stk->Capacity = 0;
    Stk->Size = 0;

    free(Stk->Data);
}



int Stk_Verifier(struct Stk_t *Stk) {
    int Exit_Code = ALL_GOOD;

    if (Stk->Data == NULL) {
        Exit_Code = ERROR;
        printf("DATA is equal to NULL\n");
        Stk_Assertion_Func(Stk, Exit_Code);

        return Exit_Code;
    }

    if (Stk->Size < 0) {
        Exit_Code = ERROR;
        printf("Size is less than 0\n");
        Stk_Assertion_Func(Stk, Exit_Code);

        return Exit_Code;
    }

    if (Stk->Capacity < 0) {
        Exit_Code = ERROR;
        printf("Capacity is less than 0\n");
        Stk_Assertion_Func(Stk, Exit_Code);

        return Exit_Code;
    }

    if (Stk->Size > Stk->Capacity) {
        Exit_Code = ERROR;
        printf("Size is bigger than Capacity\n");
        Stk_Assertion_Func(Stk, Exit_Code);

        return Exit_Code;
    }

    return ALL_GOOD;
}



void Stk_Assertion_Func(struct Stk_t *Stk, int Exit_Code) {
    if (!Exit_Code) {
        assert(Exit_Code); // and there write the line with error
    }
}



Stk_Elem_t Reader()
{
    Stk_Elem_t Received_Num = 0;
    int Had_Read_Sth = 0;

    printf("Pls write a number\n");

    while (!Had_Read_Sth) {

        Had_Read_Sth = scanf(" %lf", &Received_Num);
        if (!Had_Read_Sth) {
            printf("Try again\n");
            break;
        }
    }

    return Received_Num;
}
