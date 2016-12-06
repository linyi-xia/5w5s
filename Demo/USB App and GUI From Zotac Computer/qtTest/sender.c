#include <stdio.h>
#include <string.h>

int cnt;
int main()
{
	while (1) {
        cnt++;

        printf("%d\n",cnt%3 );

	fflush(stdout);
        sleep(3);
	}
	
	return 0;
}
