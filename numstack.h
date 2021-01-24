#include "dll.h"

/*Stack for storing numbers or operands*/
typedef struct num_node{
	number *a;
	struct num_node *p;
}num_node;
typedef num_node *num_stack;

void num_init(num_stack *s);
void num_push(num_stack *s, number *n);
number *num_pop(num_stack *s);
int num_isempty(num_stack *s);


