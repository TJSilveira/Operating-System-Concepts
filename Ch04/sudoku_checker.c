#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void	*row_checker(void *param)
{
	int row;
}

int	checker(int *arr)
{
	int i;

	i = 0;
	while (i < 9)
	{
		i++;
	}
	
}

int	main()
{
	pthread_t	*arr[11];
	int			x[9][9] = {{1,2,3,6,7,8,9,4,5},
							{5,8,4,2,3,9,7,6,1},
							{9,6,7,1,4,5,3,2,8},
							{3,7,2,4,6,1,5,8,9},
							{6,9,1,5,8,3,2,7,4},
							{4,5,8,7,9,2,6,1,2},
							{8,3,6,9,2,4,1,5,7},
							{2,1,9,8,5,7,4,3,6},
							{7,4,5,3,1,6,8,9,2}};
	int			i;

	i = 0;
	// Rows
	while (i < 9)
	{
		printf("%i\n", x[0][i]);
		i++;
	}
}