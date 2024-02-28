#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

/* If the stack is empty, size == 0 and top == NULL. If only one is true, the
 * stack is in an inconsistent state and stack_is_empty() will throw an error.
 */
struct stack {
	int size;
	struct stack_item *top;
};

void stack_init(struct stack *s);
double stack_pop(struct stack *s);
double stack_peek(const struct stack *s);
void stack_push(struct stack *s, double item);
bool stack_is_empty(const struct stack *s);
void stack_delete(struct stack *s);
void stack_dup(struct stack *dst, const struct stack *src);
void stack_print(const struct stack *s);

#endif
