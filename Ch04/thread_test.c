#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int	sum;
void	*runner(void *param);

void *runner(void *param)
{
	int i = 0;
	int	upper = atoi(param);

	sum = 0;
	while (++i <= upper)
		sum += i;
	pthread_exit(0);
}

int main(int argc, char *argv[])
{
	pthread_t tid;
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_create(&tid, &attr, runner, argv[1]);
	pthread_join(tid, NULL);

	printf("sum = %d\n", sum);
}