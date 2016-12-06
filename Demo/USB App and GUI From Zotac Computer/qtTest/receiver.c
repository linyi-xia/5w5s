#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int i = 0;
	char *buf = malloc(20);

	while (fgets(buf, 20, stdin)) {
		if (strncmp(buf, "Send a message!\n", 20) == 0)
			printf("%d: data received\n", ++i);
		else 
			printf("%d: no string matched\n", ++i);
	}

	return 0;
}
