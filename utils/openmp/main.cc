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
#define ITER 100000000l

int main(void) {
	srand(time(NULL));
	auto start = chrono::system_clock::now();
	for(int i = 0; i<ITER; ++i) {
		int xi = rand() % MOD;
		double x = xi / (double)MOD;
		if (x == 1.0) {
			printf("%f\n", x);
		}
	}
	auto end = chrono::system_clock::now();
	chrono::duration<double> diff = end - start;
	printf("Elapsted time : %f s\n", diff.count());

	vector<int32_t> work;
	work.resize(24, 0);
	start = chrono::system_clock::now();
#pragma omp parallel for
	for (int i = 0; i<ITER; ++i) {
		int tid = omp_get_thread_num();
		++work[tid];
		int xi = rand() % MOD;
		double x = xi / (double)MOD;
		if (x == 1.0) {
			printf("%f\n", x);
		}
		if (i == ITER -1) {
			printf("Threas %d\n", omp_get_num_threads());
		}
	}
	end = chrono::system_clock::now();
	diff = end - start;
	printf("MP Elapsted time : %f s\n", diff.count());
	for(int i=0;i<24;++i) {
		printf("Thread[%d]=%d\n", i, work[i]);
	}
	return 0;
}
