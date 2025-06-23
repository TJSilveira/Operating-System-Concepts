#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	struct timeval tv;
	int i = 0;
	
	while (i < 1000)
	{
		gettimeofday(&tv, NULL);
		printf("%ld\n",tv.tv_sec);
		printf("%ld\n",tv.tv_usec);
		i++;
	}
	printf("%i\n", 3/2);
}
