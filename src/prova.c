#include <stdio.h>
#include <stdlib.h>

void usage(void)
{
	printf("Usage:\n");
	printf(" -a <name>\n");
	printf(" -c <name>\n");
	exit (8);
}

int main(int argc, char *argv[]) {

	if ((argc > 2) && (argv[1][0] == '-'))
	{
		switch (argv[1][1])
		{
			case 'a':
				printf("%s\n",&argv[1][3]);
				break;

			case 'c':
				printf("%s\n",&argv[1][3]);
				break;

			default:
				printf("Wrong Argument: %s\n", argv[1]);
				usage();
		}

		++argv;
		--argc;
	}
    
	return (0);
}
