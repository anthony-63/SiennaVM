#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

// Helper functions
int get_size(sienna_stack_t* stack){
    return stack->top + 1;
}

int is_empty(sienna_stack_t* stack){
    return stack->top == -1;
}

int is_full(sienna_stack_t* stack){
    return stack->top == stack->max_size - 1;
}

void sienna_stack_init(sienna_stack_t* stack, int capacity){
    stack = (sienna_stack_t*)malloc(sizeof(sienna_stack_t));
    stack->max_size = capacity;
    stack->top = -1;
    stack->items = (int*)malloc(sizeof(int) * capacity);
}

void sienna_stack_push(sienna_stack_t* stack, int value){
    if(is_full(stack)){
        printf("FATAL: Stack overflow!\n");
        exit(-1);
    }
    stack->items[++stack->top] = value;
}

int sienna_stack_pop(sienna_stack_t* stack){
    if(is_empty(stack)){
        printf("FATAL: Stack underflow!\n");
        exit(-1);
    }
    return stack->items[stack->top--];
}

int sienna_stack_peek(sienna_stack_t* stack){
    if(!is_empty(stack)){
        return stack->items[stack->top];
    }
    printf("FATAL: Peek at empty stack!\n");
    exit(-1);
}