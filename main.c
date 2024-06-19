#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stack.h"
#include "eval.h"
#include "op.h"

#define CMDLINE_HELP_STR "Usage: %s [-h | --help] [expression] ...\n" \
                         "\tIf no expression is supplied, run in interactive mode.\n" \
		         "\tType \"help\" or \"?\" in interactive mode for help.\n" \
		         "\tAn expression is a space-separated list of numbers and operators.\n"
#define INTERACTIVE_HELP_STR "Type an expression to evaluate and print it.\n" \
                             "The result will be pushed to the stack.\n" \
                             "Use \"print\" or \"p\" to view the stack.\n" \
                             "Use \"pop\", \"del\", or \"d\" to remove the top item from the stack.\n"

static void run_stdin()
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
		if (!strcmp(line, "exit") || !strcmp(line, "quit")) {
			free(line);
			break;
		} else if (!strcmp(line, "help") || !strcmp(line, "?")) {
			fprintf(stderr, INTERACTIVE_HELP_STR);
			print_operations("");
		} else if (!strcmp(line, "del") || !strcmp(line, "pop")
				|| !strcmp(line, "d")) {
			if (stack_is_empty(&stack))
				fprintf(stderr, "Nothing to remove.");
			else
				stack_pop(&stack);
		} else if (linelen != 0) {
			double result = eval_expression(line, &stack);
			if (!isnan(result))
				printf("%lg\n", result);
		} else {
			; //do nothing; it's an empty line
		}

		free(line);
		line = NULL;
		printf(">>> ");
		fflush(stdout);
	}
}

int main(int argc, char **argv)
{
	if (argc > 1 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) {
		fprintf(stderr, CMDLINE_HELP_STR, argv[0]);
		print_operations("\t");
		return 0;
	}

	/* We sort the list *after* printing help, so that the operators are
	 * organized semantically and not alphabetically for the user.
	 * Note that this won't apply if the user types "help" in interactive.
	 */
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
