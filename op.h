#ifndef OP_H
#define OP_H

#include "stack.h"

struct operation {
	const char *symbol;
	int num_args;
	double (*function)(struct stack *);
	const char *description;
};

extern struct operation operations[];

void operation_list_init(void);
struct operation *find_operation(const char *symbol);
void print_operations(const char *prefix);

#endif
