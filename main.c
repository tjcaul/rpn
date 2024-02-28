#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stack.h"
#include "eval.h"
#include "op.h"

void run_stdin()
{
	struct stack stack;
	stack_init(&stack);

	char *line = NULL;
	size_t linecap;
	ssize_t linelen;

	printf(">>> ");
	fflush(stdout);
	while ((linelen = getline(&line, &linecap, stdin)) > 0) {
		if (line[linelen-1] == '\n')
			line[--linelen]	= '\0';
		if (!strcmp(line, "exit") || !strcmp(line, "quit"))
			break;
		if (linelen != 0) {
			double result = eval_expression(line, &stack);
			if (!isnan(result))
				printf("%lg\n", result);
		}

		free(line);
		line = NULL;
		printf(">>> ");
		fflush(stdout);
	}
}

int main(int argc, char **argv)
{
	operation_list_init();

	if (argc == 1) {
		run_stdin();
	} else {
		for (int i = 1; i < argc; ++i) {
			double result = eval_expression(argv[i], NULL);
			if (!isnan(result))
				printf("%lg\n", result);
		}
	}
	return 0;
}
