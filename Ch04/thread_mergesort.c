#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct s_thread_conditions
{
	int	*original;
	int	*temp;
	int	left;
	int	right;
}	t_thread_conditions;


void	merge(int *original, int *temp, int left, int mid_left, int mid_right, int right)
{
	int	i;
	int i_left;
	int i_right;

	i = left;
	i_left = left;
	i_right = mid_right;

	while (i_left <= mid_left && i_right <= right)
	{
		if (original[i_left] <= original[i_right])
		{
			temp[i] = original[i_left];
			i++;
			i_left++;
		}
		else
		{
			temp[i] = original[i_right];
			i++;
			i_right++;
		}
	}
	while (i_left <= mid_left)
	{
		temp[i] = original[i_left];
		i++;
		i_left++;
	}
	while (i_right <= right)
	{
		temp[i] = original[i_right];
		i++;
		i_right++;
	}
	i = left;
	while (i <= right)
	{
		original[i] = temp [i];
		i++;
	}
	i = 0;
}

void	*merge_sort(void *param)
{
	pthread_t 			thread[2];
	t_thread_conditions	*cond;
	t_thread_conditions	cond_left;
	t_thread_conditions	cond_right;
	int mid;

	cond = (t_thread_conditions*) param;
	if (cond->left == cond->right)
		return (NULL);
	cond_left.original = cond->original;
	cond_right.original = cond->original;
	cond_left.temp = cond->temp;
	cond_right.temp = cond->temp;

	mid = cond->left + (cond->right - cond->left)/2;
	cond_left.left = cond->left;
	cond_left.right = mid;
	cond_right.left = mid + 1;
	cond_right.right = cond->right;
	if(pthread_create(&thread[0], NULL, &merge_sort, &cond_left) != 0)
		exit(EXIT_FAILURE);
	if(pthread_create(&thread[1], NULL, &merge_sort, &cond_right) != 0)
		exit(EXIT_FAILURE);
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);
	merge(cond->original, cond->temp, cond->left, mid, mid + 1, cond->right);
	return (NULL);
}

int	main(int argc, char *argv[])
{	
	t_thread_conditions	cond;
	int	i;
	struct timeval start_time;
	struct timeval end_time;

	if (argc == 1)
	{
		printf("Invalid input: No arguments given.\n");
		exit(EXIT_FAILURE);
	}
	cond.original = malloc(sizeof(int) * (argc - 1));
	cond.temp = calloc(argc - 1, sizeof(int));
	if(!cond.original || !cond.temp)
		exit(EXIT_FAILURE);	
	i = 0;
	while (i < argc - 1)
	{
		cond.original[i] = atoi(argv[i + 1]);
		i++;
	}
	cond.left = 0;
	cond.right = argc -2;
	gettimeofday(&start_time, NULL);
	merge_sort(&cond);
	i = 0;
	while (i < argc - 1)
	{
		printf("%i\n", cond.original[i]);
		i++;
	}
	gettimeofday(&end_time, NULL);
	printf("Time spent: %ld\n", end_time.tv_usec - start_time.tv_usec);
}