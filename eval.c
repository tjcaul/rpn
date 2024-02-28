#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <sys/errno.h>
#include "eval.h"
#include "op.h"

/* Perform the operation with the given symbol on the stack by popping the
 * necessary items off the top, then pushing the result back onto the stack.
 * Returns 0 if the operation is valid, or -1 otherwise.
 */
static int exec_operation(struct stack *stack, const char *symbol)
{
	if (!stack) {
		fprintf(stderr, "exec_operation: stack is a NULL pointer\n");
		exit(EFAULT);
	}
	
	const struct operation *op = find_operation(symbol);
	if (!op) {
		fprintf(stderr, "Invalid operator '%s'\n", symbol);
		return -1;
	} else if (op->num_args > stack->size) {
		fprintf(stderr, "Operation takes %d arguments, but there %s %d on the stack.\n",
				op->num_args, stack->size == 1 ? "is" : "are",
				stack->size);
		return -1;
	}

	stack_push(stack, (op->function)(stack));
	return 0;
}

/* Return whether the string is a valid floating-point number, by strtof.
 */
static bool is_number(const char *str)
{
	char *endptr;
	int len = strlen(str);
	strtof(str, &endptr);
	return len > 0 && endptr == str + len;
}

/* Evaluate the expression. If stack points to a stack, use the context of that
 * stack. If the stack is a NULL pointer, use a temporary empty stack.
 * If the expression is valid, return its result and modify the stack.
 * If the expression is invalid, return NaN without modifying the stack.
 */
double eval_expression(const char *expr, struct stack *context_stack)
{
	double ret;
	bool error = false;
	struct stack *stack, *backup_stack = NULL;
	if (context_stack) { //Use the supplied stack, keeping a backup
		stack = context_stack;
		backup_stack = malloc(sizeof(*backup_stack));
		stack_dup(backup_stack, stack);
	} else { //Use a temporary stack
		stack = malloc(sizeof(*backup_stack));
		stack_init(stack);
	}

	char *exprdup, *tofree, *token;
	tofree = exprdup = strdup(expr);

	while ((token = strsep(&exprdup, " ")) != NULL) {
		if (is_number(token)) {
			stack_push(stack, strtof(token, NULL));
			ret = nan("");
		} else if (!strcmp(token, "p") || !strcmp(token, "print")) {
			stack_print(stack);
			ret = nan("");
		} else if (exec_operation(stack, token) == 0) {
			ret = stack_peek(stack);
		} else {
			ret = nan("");
			error = true;
			break;
		}
	}

	if (context_stack) {
		if (error)
			stack_dup(stack, backup_stack); //restore stack
		free(backup_stack);
	} else {
		free(stack);
	}
	free(tofree);
	return ret;
}
