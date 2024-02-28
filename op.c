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
	{"swap", 2, op_swap, "swap the top two values on the stack"},
	{"+",    2, op_add, "add"},
	{"-",    2, op_subtract, "subtract the top value from the value below"},
	{"*",    2, op_multiply, "multiply"},
	{"/",    2, op_divide, "divide the value below by the top value"},
	{"//",   2, op_integer_divide, "integer divide: divide, then floor"},
	{"%",    2, op_remainder, "remainder: divide, returning the remainder"},
	{"^",    2, op_power, "power: raise value below to the power of the top"},
	{"root", 2, op_root, "root: take value below to the root of the top"},
	{"sqrt", 1, op_sqrt, "square root"},
	{"cbrt", 1, op_cbrt, "cube root"},
	{"sin",  1, op_sin, "sine"},
	{"cos",  1, op_cos, "cosine"},
	{"tan",  1, op_tan, "tangent"},
	{"asin", 1, op_asin, "inverse sine"},
	{"acos", 1, op_acos, "inverse cos"},
	{"atan", 1, op_atan, "inverse tangent"},
	{"exp",  1, op_exp, "natural exponent"},
	{"log",  1, op_log, "natural logarithm"},
	{"pi",   0, op_pi, "pi"},
	{"e",    0, op_e, "Euler's number"},
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

/* Print the operations list in a readable format for documentation.
 * Every line will be prefixed by the prefix; use it to indent the output.
 */
void print_operations(const char *prefix) {
	printf("%sAvailable operators:\n", prefix);
	printf("%sSYMBOL\t# ARGUMENTS\tDESCRIPTION\n", prefix);

	for (int i = 0; i < ARRAY_LEN(operations); ++i) {
		const struct operation op = operations[i];
		char *num_args_descriptions[] = {"constant", "unary", "binary"};
		char *num_args_desc;
		if (op.num_args < 0 || op.num_args > 2)
			num_args_desc = "ARGUMENT NUMBER ERROR";
		else
			num_args_desc = num_args_descriptions[op.num_args];
		printf("%s'%s':\t%d (%s)\t%s\n", prefix, op.symbol,
				op.num_args, num_args_desc, op.description);
	}
}
