#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <glib.h>

#define MAXN 100000
int* arr = 0;

void printarr(int arr[], int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void mk_rand(long long size)
{
	for(long long i=0; i < size; i++) {
		arr[i] = rand() % MAXN;
	}
}

void merge(int arr[], int start ,int mid, int end) 
{
	int i = start;
	int j = mid + 1;
	int k = 0;
	int t_size = end - start + 1;
	int tarr[t_size] = {0,};
	while(i <= mid && j <= end) {
		if (arr[i] < arr[j]) {
			tarr[k++] = arr[i++];
		} 
		else {
			tarr[k++] = arr[j++];
		}
	}

	if (i <= mid) {
		while(i <= mid) {
			tarr[k++] = arr[i++];
		}
	}
	else if (j <= end) {
		while(j <= end) {
			tarr[k++] = arr[j++];
		}
	}

	for(int i = 0; i<t_size; i++) {
		arr[i+start] = tarr[i];
	}

	return;
}

void msort(int arr[], long long start, long long end)
{
	if (start < end) {
		int mid = start + (end-start) / 2;
		msort(arr, start, mid);
		msort(arr, mid+1, end);

		merge(arr, start, mid, end);
	}
	return;
}

int main(int argc, char* argv[])
{
	long long size = 0;
	srand(time(NULL));
	if (argc < 2) {
		size = 100;
	}
	else {
		size = atoi(argv[1]);
	}
	printf("size=%lld\n", size);
	arr = new int[size];
	mk_rand(size);
	//printarr(arr, size);
	gint64 startTime = g_get_monotonic_time();
	msort(arr, 0, size-1);
	//printarr(arr, size);
	gint64 endTime = g_get_monotonic_time();
	printf("elapsed %ld us\n", endTime - startTime);
	delete [] arr;
	return 0;
}
