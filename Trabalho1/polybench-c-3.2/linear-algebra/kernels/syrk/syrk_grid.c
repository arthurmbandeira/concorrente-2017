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
#include <mpi.h>

/* Include polybench common header. */
#include <polybench.h>

/* Include benchmark-specific header. */
/* Default data type is double, default size is 4000. */
#include "syrk.h"
#include "utils.h"


// POLYBENCH STUFF
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
  for (i = 0; i < ni; i++)
    for (j = 0; j < nj; j++)
      A[i][j] = ((DATA_TYPE) i*j) / ni;
  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++)
      C[i][j] = ((DATA_TYPE) i*j) / ni;
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
	fprintf (stderr, DATA_PRINTF_MODIFIER, C[i][j]);
	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
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

// END POLYBENCH STUFF


// MPI STUFF

MPI_Group  global_group;
MPI_Group  gr0,  gr1,  gr2,  gr3,
           gr4,  gr5,  gr6,  gr7,
           gr8,  gr9, gr10, gr11,
          gr12, gr13, gr14, gr15;

MPI_Comm  comm0,  comm1,  comm2,  comm3,
          comm4,  comm5,  comm6,  comm7,
          comm8,  comm9, comm10, comm11,
         comm12, comm13, comm14, comm15;

MPI_Request req_up, req_down, req_left, req_right;

MPI_Status status;

// END MPI STUFF

// GRID STUFF



// END GRID STUFF


void init_groups(){
  MPI_Comm_Group(MPI_COMM_WORLD, &global_group);

  int connections[16][2] = {{10, 7}, {11, 1}, {12, 3}, {13, 9},
                             {1, 2},  {2, 3},  {4, 5},  {5, 6},
                             {7, 8},  {8, 9},  {1, 4},  {4, 7},
                             {2, 5},  {5, 8},  {3, 6},  {6, 9}};

  MPI_Group_incl(global_group, 2, connections[0], &gr0);

  MPI_Group_incl(global_group, 2, connections[1],   &gr1);
  MPI_Group_incl(global_group, 2, connections[2],   &gr2);
  MPI_Group_incl(global_group, 2, connections[3],   &gr3);

  MPI_Group_incl(global_group, 2, connections[4],   &gr4);
  MPI_Group_incl(global_group, 2, connections[5],   &gr5);
  MPI_Group_incl(global_group, 2, connections[6],   &gr6);
  MPI_Group_incl(global_group, 2, connections[7],   &gr7);
  MPI_Group_incl(global_group, 2, connections[8],   &gr8);
  MPI_Group_incl(global_group, 2, connections[9],   &gr9);
  MPI_Group_incl(global_group, 2, connections[10], &gr10);
  MPI_Group_incl(global_group, 2, connections[11], &gr11);
  MPI_Group_incl(global_group, 2, connections[12], &gr12);
  MPI_Group_incl(global_group, 2, connections[13], &gr13);
  MPI_Group_incl(global_group, 2, connections[14], &gr14);
  MPI_Group_incl(global_group, 2, connections[15], &gr15);

  
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr0, 0,  &comm0);

  MPI_Comm_create_group(MPI_COMM_WORLD,  gr1, 0,  &comm1);
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr2, 0,  &comm2);
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr3, 0,  &comm3);

  MPI_Comm_create_group(MPI_COMM_WORLD,  gr4, 0,  &comm4);
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr5, 0,  &comm5);
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr6, 0,  &comm6);
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr7, 0,  &comm7);
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr8, 0,  &comm8);
  MPI_Comm_create_group(MPI_COMM_WORLD,  gr9, 0,  &comm9);
  MPI_Comm_create_group(MPI_COMM_WORLD, gr10, 0, &comm10);
  MPI_Comm_create_group(MPI_COMM_WORLD, gr11, 0, &comm11);
  MPI_Comm_create_group(MPI_COMM_WORLD, gr12, 0, &comm12);
  MPI_Comm_create_group(MPI_COMM_WORLD, gr13, 0, &comm13);
  MPI_Comm_create_group(MPI_COMM_WORLD, gr14, 0, &comm14);
  MPI_Comm_create_group(MPI_COMM_WORLD, gr15, 0, &comm15);

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

  /* Start timer. */
  polybench_start_instruments;

  /* Run kernel. */
  kernel_syrk (ni, nj, alpha, beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));

  /* Stop and print timer. */
  polybench_stop_instruments;
  polybench_print_instruments;

  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  polybench_prevent_dce(print_array(ni, POLYBENCH_ARRAY(C)));

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}
