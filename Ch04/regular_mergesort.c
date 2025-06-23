#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

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

void	merge_sort(int *original, int *temp, int left, int right)
{
	int mid;

	if (left == right)
		return ;
	mid = left + (right - left)/2;
	merge_sort(original, temp, left, mid);
	merge_sort(original, temp, mid + 1, right);
	merge(original, temp, left, mid, mid + 1, right);
}

int	main(int argc, char *argv[])
{	
	int	*original;
	int *temp;
	int	i;
	struct timeval start_time;
	struct timeval end_time;

	if (argc == 1)
	{
		printf("Invalid input: No arguments given.\n");
		exit(EXIT_FAILURE);
	}
	original = malloc(sizeof(int) * (argc - 1));
	temp = calloc(argc - 1, sizeof(int));
	if(!original || !temp)
		exit(EXIT_FAILURE);	
	i = 0;
	while (i < argc - 1)
	{
		original[i] = atoi(argv[i + 1]);
		i++;
	}
	gettimeofday(&start_time, NULL);
	merge_sort(original, temp, 0, argc - 2);
	i = 0;
	while (i < argc - 1)
	{
		printf("%i\n", original[i]);
		i++;
	}
	gettimeofday(&end_time, NULL);
	printf("Time spent: %ld\n", end_time.tv_usec - start_time.tv_usec);
}