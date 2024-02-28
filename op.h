#ifndef OP_H
#define OP_H

#include "stack.h"

struct operation {
	const char *symbol;
	int num_args;
	double (*function)(struct stack *);
};

extern struct operation operations[];

void operation_list_init(void);
struct operation *find_operation(const char *symbol);

#endif
