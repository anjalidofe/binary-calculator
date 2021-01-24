#include <stdio.h>
#include <stdlib.h>
#include "numstack.h"

void num_init(num_stack *s){
    *s = NULL;
}

void num_push(num_stack *s, number *n){
	num_stack temp;
	temp = (num_node *)malloc(sizeof(num_node));
	temp->a = n;
	temp->p = (*s);
	*s = temp;
}

number *num_pop(num_stack *s){
	number *n;
	num_stack temp;
	n = (*s)->a;
	temp = (*s);
	*s = (*s)->p;
	free(temp);
	return n;
}

int num_isempty(num_stack *s){
	return *s == NULL;
}

