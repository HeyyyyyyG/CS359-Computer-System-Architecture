/*
	name: Jiaqi Zeng
	studentID:517021910882
*/
/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
 /*
from the given information we know that :
each block is 32byte = 8 int
there are 32 blocks



 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
/*
matrix A and B are stored in the memory as neighbors
if M=N=32, there are 32 int in a row, that is 4 blocks
so the cache can store 8 rows
This means that if 2 ints' location different at some point that can divided by 8 rows
then there map to the same block in cache
But this won't happen in the transpose.
What will happen is that for i=j, A[i][j] and B[j][i] map to the same block thus conflict
so use temp to store elements in A so that even B covers A in the cache we don't need to bring A in the cache again

*/
	if(M==32 && N==32){
	int i,j,k,temp0,temp1,temp2,temp3,temp4,temp5,temp6,temp7;
	for(i=0;i<N;i=i+8)
	{
		for(j=0;j<M;j+=8)
		{
			for(k=i;k<i+8;k++)
			{
				temp0 = A[k][j];
				temp1 = A[k][j+1];
				temp2 = A[k][j+2];
				temp3 = A[k][j+3];
				temp4 = A[k][j+4];
				temp5 = A[k][j+5];
				temp6 = A[k][j+6];
				temp7 = A[k][j+7];

				B[j][k] = temp0;
				B[j+1][k] = temp1;
				B[j+2][k] = temp2;
				B[j+3][k] = temp3;
				B[j+4][k] = temp4;
				B[j+5][k] = temp5;
				B[j+6][k] = temp6;
				B[j+7][k] = temp7;
			}

		}
	}
}

/*
if M=N=64, then now the cache can only store 4 rows
This means that if 2 ints' location different at some point that can divided by 8 rows
then there map to the same block in cache
If we still use the above strategy, the second 4 rows will conflict with the first 4 rows
so we divide 8*8 into 4 4*4
1   2
3   4
we can transfer part 1 & 2 in A to part 1 & 2 in B
 								part 3 & 4 in A tp part 3 & 4 in B
	then exchange part2 and part3 in B

it seems to be not good enough
so I search for some techiniques on the Internet
refer to https://www.cnblogs.com/liqiuhao/p/8026100.html?utm_source=debugrun&utm_medium=referral
this technique is what we cannot think of
}
*/

else if (M == 64 && N == 64) {
			 //12 variables
			 int i,j,k,y,tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7;
			 for (i = 0; i < N; i += 8) {
							 for ( j = 0; j < M; j += 8) {

									 for ( k = i; k < i + 8/2; k++) {
											 //part1
											 		 tmp0 = A[k][j],
													 tmp1 = A[k][j + 1],
													 tmp2 = A[k][j + 2],
													 tmp3 = A[k][j + 3],
											 //part2
													 tmp4 = A[k][j + 4],
													 tmp5 = A[k][j + 5],
													 tmp6 = A[k][j + 6],
													 tmp7 = A[k][j + 7];

											 //part1 of A to part1 of B
											 B[j][k] = tmp0;
											 B[j + 1][k] = tmp1;
											 B[j + 2][k] = tmp2;
											 B[j + 3][k] = tmp3;
											 //part2 of A to part2 of B
											 B[j + 3][k + 4] = tmp4;
											 B[j + 2][k + 4] = tmp5;
											 B[j + 1][k + 4] = tmp6;
											 B[j][k + 4] = tmp7;
									 }

									 for (y= 0; y < 8/2 ; y++) {
											 //read from the center to side by columns

											 //part3 of A
											 tmp0 = A[i + 4][j +  8/2 -1 - y];
											 tmp1 = A[i + 5][j +  8/2 -1 - y];
											 tmp2 = A[i + 6][j +  8/2 -1 - y];
											 tmp3 = A[i + 7][j +  8/2 -1 - y];
											 //part4 of A
											 tmp4 = A[i + 4][j + 8/2 + y];
											 tmp5 = A[i + 5][j + 8/2 + y];
											 tmp6 = A[i + 6][j + 8/2 + y];
											 tmp7 = A[i + 7][j + 8/2 + y];


			 								//part 2 of B to part3 of B by rows from down to up
											 B[j + 8/2 + y][i] = B[j +  8/2 -1 - y][i + 4];
											 B[j + 8/2 + y][i + 1] = B[j +  8/2 -1 - y][i + 5];
											 B[j + 8/2 + y][i + 2] = B[j +  8/2 -1 - y][i + 6];
											 B[j + 8/2 + y][i + 3] = B[j +  8/2 -1 - y][i + 7];

											 //part3 of B(already in tmp) to part2 of B
											 B[j +  8/2 -1 - y][i + 4] = tmp0;
											 B[j +  8/2 -1 - y][i + 5] = tmp1;
											 B[j +  8/2 -1 - y][i + 6] = tmp2;
											 B[j +  8/2 -1 - y][i + 7] = tmp3;

											 //part4 of B (already in tmp) to part4 of B
											 B[j + 8/2 + y][i + 4] = tmp4;
											 B[j + 8/2 + y][i + 5] = tmp5;
											 B[j + 8/2 + y][i + 6] = tmp6;
											 B[j + 8/2 + y][i + 7] = tmp7;
									 }
							 }
					 }
}

/*
this case is irrregular
we just take A[i][j]( i==j) out and deal with that later
*/
else if(M==61 && N==67)
{
	int step = (M+N)/8; //block size
	int i,j,x,y,cross_index ,cur_value,cross_value; //8variables
	for(i=0;i<N;i+=step)
	{
		for(j=0; j < M; j += step)
		{
			for(x = i; x<i+step && x<N;x++)
			{

				//store diagonal
				cross_index = -1;
				for(y = j; y < j+step && y<M;y++)
				{
					cur_value = A[x][y];
					if(y!=x) //simply transpose
						B[y][x]=cur_value;
					else
					{
						cross_index = x;
						cross_value = cur_value;
					}
				}
				//deal with diagonal
				if(cross_index !=-1)
				{
					B[cross_index][cross_index] = cross_value;
				}

			}
		}
	}
}

}
/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}
