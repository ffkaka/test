#include <iostream>
#include <fstream>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <vector>
#include <time.h>

using namespace std;
#define MOD (0x7FFFl)
#define ITER 2000000000l
#define MAX 1e+12l

int main(void)
{
	vector<int32_t> work;
	work.resize(24, 0);
	auto start = chrono::system_clock::now();
	//int xi;
	//double x;
	for(int i = 0; i<ITER; ++i) {
		int xi = 3937923 % MOD;
		double x = xi / (double)MOD;
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> diff = end - start;
	printf("Elapsted time : %f s\n", diff.count());

	start = chrono::system_clock::now();
#pragma omp parallel
	{
		int tid = omp_get_thread_num();
#pragma omp for
		for (int i = 0; i<ITER; ++i) {
			int xi = 3937923 % 0x7FFFl;
			double x = xi / (double)0x7FFFl;
			//int tid = omp_get_thread_num();
			//++work[tid];
		}
	}
	end = chrono::system_clock::now();
	diff = end - start;
	printf("MP Elapsted time : %f s\n", diff.count());
	for(int i=0;i<24;++i) {
		printf("Thread[%d]=%d\n", i, work[i]);
	}
	printf("%lf\n", MAX);
	return 0;
}
