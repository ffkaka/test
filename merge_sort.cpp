#include <stdio.h>

void printarr(int arr[], int size)
{
	for(int i = 0; i < size; i++)
	{
		printf("%d ", arr[i]);
	}
	printf("\n");
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

void msort(int arr[], int start, int end)
{
	if (start < end) {
		int mid = start + (end-start) / 2;
		msort(arr, start, mid);
		msort(arr, mid+1, end);

		merge(arr, start, mid, end);
	}
	return;
}

int main()
{
	int arr[] = {8, 3, 5, 0, 2, 4, 1, 5, 6};
	int size = sizeof(arr) / sizeof(int);
	printarr(arr, size);
	msort(arr, 0, size-1);
	printarr(arr, size);
	printf("size = %d\n", size);
	return 0;
}
