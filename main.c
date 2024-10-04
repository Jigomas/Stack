#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>



typedef double stk_elem_t;
typedef stk_elem_t poison_elem_t;

struct stk_t {
    stk_elem_t *data;
    unsigned int size;
    unsigned int capacity;
};

enum exit_codeS {
    ALL_GOOD = 1,
    ERROR = 0,
};



int StkCtor(struct stk_t *stk, unsigned int capacity, poison_elem_t poison_elem);

int StkDtor(struct stk_t *stk);

void StkDumper(struct stk_t *stk, const char* file, double line, poison_elem_t poison_elem);

int StkVerifier(struct stk_t *stk);

int StkPush(struct stk_t *stk, stk_elem_t new_element, poison_elem_t poison_elem);

int StkPop(struct stk_t *stk, poison_elem_t poison_elem);

void StkFreeMem(struct stk_t **stk);

void StkAddMem(struct stk_t **stk, poison_elem_t poison_elem);

stk_elem_t Reader();

void StkAssertionFunc(struct stk_t *stk, int exit_code);



int main() {
    int exit_code = ALL_GOOD;
    int amount_to_push = 5;
    int amount_to_pop =  5;

    poison_elem_t poison_elem = 6;

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
    printf ("Poison Element = %lf\n", poison_elem);
    struct stk_t stk = {};

    unsigned int capacity = 5;    // this is gonna be starting capacity



    exit_code *= StkCtor(&stk, capacity, poison_elem);
    exit_code *= StkVerifier(&stk);

    for (int i = 0; i < amount_to_push/*How many do u want to Push*/; i++) {
        stk_elem_t new_element = Reader();

        exit_code *= StkPush(&stk,  new_element, poison_elem);
        exit_code *= StkVerifier(&stk);
    }





    for (int i = 0; i < stk.capacity; i++)
        printf("%lf ", stk.data[i]);
    printf("\n");


    // Using pop
    for (int i = 0; i < amount_to_pop/*How many do u want to pop*/; i++) {
        exit_code *= StkPop(&stk, poison_elem);
        exit_code *= StkVerifier(&stk);


        //for (int j = 0; j < stk.capacity; j++)
        //    printf("%lf ", stk.data[j]);
        StkDumper(&stk, __FILE__, __LINE__, poison_elem);
        printf("\n");
    }
    exit_code *= StkDtor(&stk);

    printf("Programm ended with Exit Code %d", exit_code);

}



int StkCtor(struct stk_t *stk, unsigned int capacity, poison_elem_t poison_elem) {   // gonna produce stk
    stk->size = 0;
    stk->capacity = capacity;

    stk->data = (stk_elem_t *)calloc(stk->capacity, sizeof(stk_elem_t));
    for (int i = 0; i < capacity; i++)
        stk->data[i] = poison_elem;
    return(StkVerifier(stk));
}



int StkPush(struct stk_t *stk, stk_elem_t new_element, poison_elem_t poison_elem) {
    int exit_code = StkVerifier(stk);

    if (!exit_code) return exit_code;

    StkAddMem(&stk, poison_elem);
    printf("new_element %lf\n", new_element);
    //printf("size b4 %d\n", stk->size);
    //printf("%d, %d\n", stk->size, stk->capacity);

    stk->data [stk->size] = new_element;
    stk->size ++;

    //printf("size after %d\n", stk->size);

    return exit_code;
}



void StkAddMem(struct stk_t **stk, poison_elem_t poison_elem) {
    int exit_code = StkVerifier(*stk);

    if ((*stk)->size == (*stk)->capacity) {
        size_t new_capacity = (*stk)->capacity ? (*stk)->capacity * 2 : 1; // Start with 1 if capacity is 0
        stk_elem_t *new_data =
                    (stk_elem_t *)realloc((*stk)->data, new_capacity * sizeof(stk_elem_t));

        for (int i = (*stk)->size; i < new_capacity; i++)
              (*stk)->data [i] = poison_elem;

        if (new_data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        (*stk)->data = new_data;
        (*stk)->capacity = new_capacity; // Update capacity
    }
}



int StkPop(struct stk_t *stk, poison_elem_t poison_elem) {
    int exit_code = StkVerifier(stk);

    if (!exit_code || stk->size <= 0) return exit_code;

    stk->size --;
    StkFreeMem(&stk);

    printf("Pop elemenent = %lf\n", stk->data [stk->size]);
    stk->data [stk->size] = poison_elem;

    return exit_code;
}



void StkFreeMem(struct stk_t **stk) {
    int exit_code = StkVerifier(*stk);

    if ((*stk)->size <= 0.25 * (*stk)->capacity) {
        size_t new_capacity = (*stk)->capacity / 2;

        stk_elem_t *new_data =
                    (stk_elem_t *)realloc((*stk)->data, new_capacity * sizeof(stk_elem_t));

        if (new_data == NULL) {
            printf("Memory allocation failed\n");
            return;
        }

        (*stk)->data = new_data;
        (*stk)->capacity = new_capacity; // Update capacity
    }
}



int StkDtor(struct stk_t *stk) {
    int exit_code = StkVerifier(stk);

    for (int i = 0; i < (stk->size); i++)
        stk->data [i] = 0;

    stk->capacity = 0;
    stk->size = 0;

    free(stk->data);
    return exit_code;
}



int StkVerifier(struct stk_t *stk) {
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

    return ALL_GOOD;
}

/*
struct stk_t {
    stk_elem_t *data;
    unsigned int size;
    unsigned int capacity;
};
*/
void StkAssertionFunc(struct stk_t *stk, int exit_code) {
    if (!exit_code) {
        abort(); // and there write the line with error
    }
}



void StkDumper(struct stk_t *stk, const char* line, double file, poison_elem_t poison_elem) {

    printf("\nFile: %d Line: %d\n", file, line);
    printf("capacity = %d\n",     stk->capacity);
    printf("size     = %d\n",      stk->size);

    for (int i = 0; i < stk->capacity; i++) {
        if (stk->data [i] != poison_elem)
            printf("data[%d] = %lf\n", i, stk->data [i]);
    }
}



stk_elem_t Reader()
{
    stk_elem_t received_num = 0;
    int had_read_sth = 0;

    printf("Pls write a number\n");

    while (!had_read_sth) {

        had_read_sth = scanf(" %lf", &received_num);
        if (!had_read_sth) {
            printf("Try again\n");
            break;
        }
    }

    return received_num;
}
