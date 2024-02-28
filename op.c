#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/errno.h>
#include <string.h>
#include "op.h"

#define ARRAY_LEN(x) (sizeof(x)/sizeof(x[0]))

/* swap the top two values on the stack, returning the new top value
 */
static double op_swap(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	stack_push(s, b);
	return a;
}

static double op_add(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return a + b;
}

static double op_subtract(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return a - b;
}

static double op_multiply(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return a * b;
}

static double op_divide(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return a / b;
}

/*
 * Return the integer division of the top two values on the stack.
 * Integer division means we floor the result.
 */
static double op_integer_divide(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return floor(a / b);
}

static double op_remainder(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return remainder(a, b);
}

static double op_power(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return pow(a, b);
}

static double op_root(struct stack *s)
{
	double b = stack_pop(s);
	double a = stack_pop(s);
	return pow(a, 1.0 / b);
}

static double op_sqrt(struct stack *s)
{
	return sqrt(stack_pop(s));
}

static double op_cbrt(struct stack *s)
{
	return cbrt(stack_pop(s));
}

static double op_sin(struct stack *s)
{
	return sin(stack_pop(s));
}

static double op_cos(struct stack *s)
{
	return cos(stack_pop(s));
}

static double op_tan(struct stack *s)
{
	return tan(stack_pop(s));
}

static double op_asin(struct stack *s)
{
	return asin(stack_pop(s));
}

static double op_acos(struct stack *s)
{
	return acos(stack_pop(s));
}

static double op_atan(struct stack *s)
{
	return atan(stack_pop(s));
}

static double op_exp(struct stack *s)
{
	return exp(stack_pop(s));
}

static double op_log(struct stack *s)
{
	return log(stack_pop(s));
}

static double op_pi(struct stack *s)
{
	return M_PI;
}

static double op_e(struct stack *s)
{
	return M_E;
}

struct operation operations[] = {
	{"swap", 2, op_swap},
	{"+",    2, op_add},
	{"-",    2, op_subtract},
	{"*",    2, op_multiply},
	{"/",    2, op_divide},
	{"//",   2, op_integer_divide},
	{"%",    2, op_remainder},
	{"^",    2, op_power},
	{"root", 2, op_root},
	{"sqrt", 1, op_sqrt},
	{"cbrt", 1, op_cbrt},
	{"sin",  1, op_sin},
	{"cos",  1, op_cos},
	{"tan",  1, op_tan},
	{"asin", 1, op_asin},
	{"acos", 1, op_acos},
	{"atan", 1, op_atan},
	{"exp",  1, op_exp},
	{"log",  1, op_log},
	{"pi",   0, op_pi},
	{"e",    0, op_e},
};

/* Compare operations based on symbol.
 * Helper for operation_list_init() and find_operation
 */
static int operation_cmp(const void *a, const void *b)
{
	const char *a_symbol = ((const struct operation *)a)->symbol;
	const char *b_symbol = ((const struct operation *)b)->symbol;
	return strcmp(a_symbol, b_symbol);
}

/* Return the operation with the given symbol from the global operation list,
 * or NULL if one doesn't exist.
 * Precondition: the operation list must be sorted.
 * operation_list_init() ensures this.
 */
struct operation *find_operation(const char *symbol)
{
	const struct operation search_op = {symbol, -1, NULL};
	return bsearch(&search_op, operations, ARRAY_LEN(operations),
			sizeof(operations[0]), operation_cmp);
}

/* Sort the global operation list. Must be called once before the first
 * find_operation() call in the program.
 */
void operation_list_init(void)
{
	qsort(operations, ARRAY_LEN(operations),
			sizeof(operations[0]), operation_cmp);
}
