#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int	checker(int *arr)
{
	int i;
	int	sol[9] = {1,1,1,1,1,1,1,1,1};

	i = 0;
	while (i < 9)
	{
		sol[arr[i] - 1]--;
		i++;
	}
	i = 0;
	while (i < 9)
	{
		if (sol[i] != 0)
		{
			printf("%i %i %i %i %i %i %i %i %i\n", arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],arr[6],arr[7],arr[8]);
			printf("Error: Row does not have a solution. Program was exited\n");
			exit (EXIT_FAILURE);
		}
		i++;
	}
	return (0);
}

void	*row_checker(void *param)
{
	int row;
	int col;
	int (*x)[9] = (int (*)[9])param;
	int	arr_to_check[9];

	row = 0;
	while (row < 9)
	{
		printf("beginnig of the loop\n");
		col = 0;
		while (col < 9)
		{
			arr_to_check[col] = x[row][col];
			col++;
		}
		checker(arr_to_check);
		row++;
	}
	printf("Sucessfully checked rows. No errors detected.\n");
	return (NULL);
}

void	*col_checker(void *param)
{
	int row;
	int col;
	int (*x)[9] = (int (*)[9])param;
	int	arr_to_check[9];

	col = 0;
	while (col < 9)
	{
		row = 0;
		while (row < 9)
		{
			arr_to_check[row] = x[row][col];
			row++;
		}
		checker(arr_to_check);
		col++;
	}
	printf("Sucessfully checked columns. No errors detected.\n");
	return (NULL);
}

void	matrix_loader(int *arr[9], int i)
{
	int row;
	int col;
	int j;
	int k;

	if (i == 0)
	{
		row = 0;
		col = 0;
	}
	else if(i == 1)
	{
		row = 3;
		col = 0;
	}
	else if(i == 2)
	{
		row = 6;
		col = 0;
	}
	else if(i == 3)
	{
		row = 0;
		col = 3;
	}
	else if(i == 4)
	{
		row = 3;
		col = 3;
	}
	else if(i == 5)
	{
		row = 6;
		col = 3;
	}
	else if(i == 6)
	{
		row = 0;
		col = 6;
	}
	else if(i == 7)
	{
		row = 3;
		col = 6;
	}
	else if(i == 8)
	{
		row = 6;
		col = 6;
	}
	
	j = 0;
	while (j < 3)
	{
		k = 0;
		while (k < 3)
		{
			/* code */
		}
		
	}
	
}

int	main()
{
	pthread_t	arr[11];
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
	pthread_create(&arr[i], NULL, &row_checker, x);
	i++;
	// column
	pthread_create(&arr[i], NULL, &col_checker, x);
	i++;
	// while (i < 11)
	// {
	// 	pthread_create(arr[i++], NULL, col_checker, x);
	// 	i++;
	// }

	pthread_join(arr[0], NULL);
	pthread_join(arr[1], NULL);

	// i = 0;
	// while (i < 11)
	// {
	// 	pthread_join(arr[i], NULL);
	// }
	// return (EXIT_SUCCESS);
}