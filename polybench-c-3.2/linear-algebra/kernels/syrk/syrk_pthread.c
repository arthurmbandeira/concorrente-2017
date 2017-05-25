/**
 * syrk.c: This file is part of the PolyBench/C 3.2 test suite.
 *
 *
 * Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>
 * Web address: http://polybench.sourceforge.net
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

/* Include polybench common header. */
#include <polybench.h>

#include <pthread.h>

/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "syrk.h"

#define MAX_THREAD 1000

pthread_barrier_t barrier;

DATA_TYPE **A_copy, **C_copy;

/* Array initialization. */
static
void init_array(int ni, int nj,
		DATA_TYPE *alpha,
		DATA_TYPE *beta,
		DATA_TYPE POLYBENCH_2D(C,NI,NI,ni,ni),
		DATA_TYPE POLYBENCH_2D(A,NI,NJ,ni,nj))
{
  int i, j;

  *alpha = 32412;
  *beta = 2123;

  A_copy = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));
  C_copy = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));

  for (i = 0; i < ni; i++){
    A_copy[i] = (DATA_TYPE *)malloc(nj*sizeof(DATA_TYPE));
    for (j = 0; j < nj; j++){
      A[i][j] = ((DATA_TYPE) i*j) / ni;
      A_copy[i][j] = ((DATA_TYPE) i*j) / ni;
      }
  }
  for (i = 0; i < ni; i++){
    C_copy[i] = (DATA_TYPE *)malloc(ni*sizeof(DATA_TYPE));
    for (j = 0; j < ni; j++){
      C[i][j] = ((DATA_TYPE) i*j) / ni;
      C_copy[i][j] = ((DATA_TYPE) i*j) / ni;
      }
  }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
static
void print_array(int ni,
		 DATA_TYPE POLYBENCH_2D(C,NI,NI,ni,ni))
{
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++) {
	fprintf (stderr, DATA_PRINTF_MODIFIER, C_copy[i][j]);
	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}

typedef struct {
    int id;
    int ni, nj, num_threads;
    DATA_TYPE alpha;
    DATA_TYPE beta;
} parg;


static
void kernel_parallel_syrk(int id, int ni, int nj, int num_threads,
     DATA_TYPE alpha,
     DATA_TYPE beta)
{
  int i, j, k;

  int data_division = ni/num_threads;

  int start = data_division * id;
  int end = data_division * (id + 1);


#pragma scop
  /*  C := alpha*A*A' + beta*C */
  for (i = start; i < end; i++)
    for (j = 0; j < ni; j++)
      C_copy[i][j] *= beta;

  pthread_barrier_wait(&barrier);
  
  for (i = start; i < end; i++)
    for (j = 0; j < ni; j++)
      for (k = 0; k < nj; k++)
        C_copy[i][j] += alpha * A_copy[i][k] * A_copy[j][k];

  
#pragma endscop

}


/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_syrk(int ni, int nj,
		 DATA_TYPE alpha,
		 DATA_TYPE beta,
		 DATA_TYPE POLYBENCH_2D(C,NI,NI,ni,ni),
		 DATA_TYPE POLYBENCH_2D(A,NI,NJ,ni,nj))
{
  int i, j, k;

#pragma scop
  /*  C := alpha*A*A' + beta*C */
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NI; j++)
      C[i][j] *= beta;
  for (i = 0; i < _PB_NI; i++)
    for (j = 0; j < _PB_NI; j++)
      for (k = 0; k < _PB_NJ; k++)
	      C[i][j] += alpha * A[i][k] * A[j][k];
#pragma endscop

}

void * worker(void *arg){
    parg *p = (parg *) arg;
    kernel_parallel_syrk(p->id, p->ni, p->nj,
                         p->num_threads, p->alpha, p->beta);
    return NULL;
}


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;

  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,NI,NI,ni,ni);
  POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,NI,NJ,ni,nj);

  /* Initialize array(s). */
  init_array (ni, nj, &alpha, &beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));

  // /* Start timer. */
  // polybench_start_instruments;

  // /* Run kernel. */
  // kernel_syrk (ni, nj, alpha, beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));

  // /* Stop and print timer. */
  // polybench_stop_instruments;
  // polybench_print_instruments;

  /*  PARALELL  */

  int i, n;

  if (argc != 2)
    {
        printf ("Usage: %s n\n  where n is no. of threads\n", argv[0]);
        exit(1);
    }

    n = atoi(argv[1]);

    if ((n < 1) || (n > MAX_THREAD))
    {
        printf ("The no of thread should between 1 and %d.\n", MAX_THREAD);
        exit(1);
    }

  pthread_t *threads;
  pthread_attr_t pthread_custom_attr;

  parg *arg;

  threads = (pthread_t *)malloc(n*sizeof(*threads));
  pthread_attr_init(&pthread_custom_attr);

  arg = (parg *)malloc(sizeof(parg)*n);

  pthread_barrier_init(&barrier, NULL, n + 1);

  /* Start timer. */
  polybench_start_instruments;

  for (i = 0; i < n; i++){
    arg[i].id = i;
    arg[i].ni = NI;
    arg[i].nj = NJ;
    arg[i].num_threads = n;
    arg[i].alpha = alpha;
    arg[i].beta = beta;
    pthread_create(&threads[i], &pthread_custom_attr, worker, (void *)(arg+i));
  }
  pthread_barrier_wait(&barrier);
  

  for (i = 0; i < n; i++){
    pthread_join(threads[i],NULL);
  }

  pthread_barrier_destroy(&barrier);


  /* Run kernel. */
  /*kernel_parallel_syrk (ni, nj, alpha, beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));*/

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /*  END PARALLEL  */


  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}
