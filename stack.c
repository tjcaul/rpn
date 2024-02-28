#include <stdlib.h>
#include <stdio.h>
#include <sys/errno.h>
#include <stdbool.h>
#include "stack.h"

/* Represents an item on the stack that holds a double.
 * If next is NULL, this is the bottom item.
 */
struct stack_item {
	double item;
	struct stack_item *next; //Pointer to the item below
};

/* Initialize the stack as empty.
 */
void stack_init(struct stack *s)
{
	if (!s) {
		fprintf(stderr, "stack_init: stack is a NULL pointer\n");
		exit(EFAULT);
	}

	s->top = NULL;
	s->size = 0;
}

double stack_pop(struct stack *s)
{
	if (!s) {
		fprintf(stderr, "stack_pop: stack is a NULL pointer\n");
		exit(EFAULT);
	} else if (stack_is_empty(s)) {
		fprintf(stderr, "stack_pop: cannot pop empty stack\n");
		exit(EINVAL);
	}

	double ret = s->top->item;
	struct stack_item *new_top = s->top->next;
	free(s->top);
	s->top = new_top;
	s->size--;
	return ret;
}

double stack_peek(const struct stack *s)
{
	if (!s) {
		fprintf(stderr, "stack_peek: stack is a NULL pointer\n");
		exit(EFAULT);
	} else if (stack_is_empty(s)) {
		fprintf(stderr, "stack_peek: cannot peek empty stack\n");
		exit(EINVAL);
	}

	return s->top->item;
}

void stack_push(struct stack *s, double item)
{
	if (!s) {
		fprintf(stderr, "stack_push: stack is a NULL pointer\n");
		exit(EFAULT);
	}

	struct stack_item *new_item = malloc(sizeof(*new_item));
	new_item->item = item;
	new_item->next = s->top;
	s->top = new_item;
	s->size++;
}

bool stack_is_empty(const struct stack *s)
{
	if (!s) {
		fprintf(stderr, "stack_is_empty: stack is a NULL pointer\n");
		exit(EFAULT);
	}

	if (s->top == NULL && s->size != 0) {
		fprintf(stderr, "stack_is_empty: incoherent stack has non-zero size %d but no top\n", s->size);
		exit(EINVAL);
	} else if (s->top != NULL && s->size == 0) {
		fprintf(stderr, "stack_is_empty: incoherent stack has size 0 but no top is not NULL\n");
		exit(EINVAL);
	}

	return s->top == NULL;
}

/* Remove all items from the stack
 */
void stack_clear(struct stack *s)
{
	if (!s) {
		fprintf(stderr, "stack_clear: stack is a NULL pointer\n");
		exit(EFAULT);
	}

	while (!stack_is_empty(s))
		stack_pop(s);
}

/* Duplicate the src stack to the dst stack. If dst is not empty, all data in
 * it is lost.
 */
void stack_dup(struct stack *dst, const struct stack *src)
{
	if (!dst) {
		fprintf(stderr, "stack_dup: destination stack is a NULL pointer\n");
		exit(EFAULT);
	} else if (!src) {
		fprintf(stderr, "stack_dup: source stack is a NULL pointer\n");
		exit(EFAULT);
	}

	stack_clear(dst);

	struct stack_item *curr = src->top;
	while (curr) {
		stack_push(dst, curr->item);
		curr = curr->next;
	}
}

/* Print the stack with the top item on top.
 */
void stack_print(const struct stack *s)
{
	struct stack_item *curr = s->top;

	fprintf(stderr, ".--------------.\n");
	while (curr) {
		fprintf(stderr, "[ %12.5lg ]\n", curr->item);
		curr = curr->next;
	}
	fprintf(stderr, "'--------------'\n");
}
