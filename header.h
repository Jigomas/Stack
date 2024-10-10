#ifndef HEADER
#define HEADER

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
    DEBUG(canary_t stk_r_canary;)

    stk_elem_t *data;
    unsigned int size;
    unsigned int capacity;
    DEBUG(poison_elem_t poison_elem;)

    DEBUG(stk_elem_t hash;)
    DEBUG(stk_elem_t hash_after;)
    DEBUG(stk_elem_t stk_hash;)

    DEBUG(canary_t canary;)
    DEBUG(canary_t stk_l_canary;)
};

enum exit_codes {
    ALL_GOOD = 1,
    ERROR = 0,
};

enum all_numbers_commands {
    PUSH = 1 ,
    ADD  = 2 ,
    SUB  = 3 ,
    MULT = 4 ,
    DIV  = 5 ,
    OUT  = 6 ,
    IN   = 7 ,
    SQRT = 8 ,
    SIN  = 9 ,
    COS  = 10,
    DUMP = 11,
    HALT = 0 ,
};

#endif // HEADER
