#include <stdio.h>
#include <omp.h>
#include <string>
#define N 512

using namespace std;

int main(int argc, char **argv)
{
	int threads;	
	threads = stoi(string(argv[1]),nullptr, 10);
	int mA[N][N];
	int mB[N][N];
	int mC[N][N];
	int i,h;
	#ifdef _OPENMP
		omp_set_dynamic(0);
		omp_set_num_threads(threads);
	#endif
	for(h=0;h<1;h++)
	{
		for(i = 0; i < N; i++)
		{
			int j;
			for(j = 0; j < N; j++)
			{
				mA[i][j] = (i+1)*(j+1);
				mB[i][j] = mA[i][j];
			}
		}
		#pragma omp parallel for shared(mA,mB,mC)
		for(i = 0; i < N; i++)
		{
			int j;
			//#pragma omp parallel for private(j)
			for(j = 0; j < N; j++)
			{
				int k;
				int sum = 0;
				//#pragma omp parallel for private(k) reduction(+:sum)
				for(k = 0; k < N; k++)
					sum += mA[i][k]*mB[k][j];
				mC[i][j] = sum;
			}
		}
	}
	return mC[2][4];
}
