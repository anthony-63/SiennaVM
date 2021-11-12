#ifndef STACK_H
#define STACK_H

typedef struct {
    int max_size;
    int top;
    int* items;
} sienna_stack_t;

void sienna_stack_init(sienna_stack_t** stack, int capacity);
void sienna_stack_push(sienna_stack_t* stack, int value);
int sienna_stack_pop(sienna_stack_t* stack);
int sienna_stack_peek(sienna_stack_t* stack);

#endif