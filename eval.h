#ifndef EVAL_H
#define EVAL_H

#include "stack.h"

double eval_expression(const char *expr, struct stack *context_stack);

#endif
