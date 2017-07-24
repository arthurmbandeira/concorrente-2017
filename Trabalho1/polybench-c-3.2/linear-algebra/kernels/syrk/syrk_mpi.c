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

#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

// DATA_TYPE **A, **B_copy, **C_copy;

Matriz *novaMatriz(int t){
    int i;
    Matriz *mat = malloc(sizeof(Matriz));
    mat->tam = t;
    mat->elementos = (DATA_TYPE *)malloc(t*t*sizeof(DATA_TYPE*));
    return mat;
}

DATA_TYPE getElem(Matriz *mat, int lin, int col){
    return mat->elementos[lin*mat->tam + col];
}

void setElem(Matriz *mat, int lin, int col, DATA_TYPE val){
    mat->elementos[lin*mat->tam + col] = val;
}

Matriz *A, *C, *C_copy;


/* Array initialization. */
// static
// void init_array(int ni, int nj,
// 		DATA_TYPE *alpha,
// 		DATA_TYPE *beta,
// 		DATA_TYPE POLYBENCH_2D(C,NI,NI,ni,ni),
// 		DATA_TYPE POLYBENCH_2D(A,NI,NJ,ni,nj))
// {
//   int i, j;

//   *alpha = 32412;
//   *beta = 2123;
//   for (i = 0; i < ni; i++)
//     for (j = 0; j < nj; j++)
//       A[i][j] = ((DATA_TYPE) i*j) / ni;
//   for (i = 0; i < ni; i++)
//     for (j = 0; j < ni; j++)
//       C[i][j] = ((DATA_TYPE) i*j) / ni;
// }

// static
// void init_array(int ni, int nj,
//     DATA_TYPE *alpha,
//     DATA_TYPE *beta,
//     DATA_TYPE POLYBENCH_2D(C,NI,NI,ni,ni),
//     DATA_TYPE POLYBENCH_2D(A,NI,NJ,ni,nj))
// {
//   int i, j;

//   *alpha = 32412;
//   *beta = 2123;

//   A = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));
//   B_copy = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));
//   C_copy = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));

//   for (i = 0; i < ni; i++){
//     A[i] = (DATA_TYPE *)malloc(nj*sizeof(DATA_TYPE));
//     B_copy[i] = (DATA_TYPE *)malloc(nj*sizeof(DATA_TYPE));
//     for (j = 0; j < nj; j++){
//       // A[i][j] = ((DATA_TYPE) i*j) / ni;
//       A[i][j] = ((DATA_TYPE) i*j) / ni;
//       B_copy[i][j] = ((DATA_TYPE) i*j) / ni;
//       }
//   }
//   for (i = 0; i < ni; i++){
//     C_copy[i] = (DATA_TYPE *)malloc(ni*sizeof(DATA_TYPE));
//     for (j = 0; j < ni; j++){
//       // C[i][j] = ((DATA_TYPE) i*j) / ni;
//       C_copy[i][j] = ((DATA_TYPE) i*j) / ni;
//       }
//   }
// }

static
void init_array(int ni, int nj, DATA_TYPE *alpha, DATA_TYPE *beta) {
  int i, j;

  *alpha = 32412;
  *beta = 2123;

  // A = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));
  // B_copy = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));
  // C_copy = (DATA_TYPE **)malloc(ni*sizeof(DATA_TYPE));
  A = novaMatriz(ni);
  C = novaMatriz(ni);
  C_copy = novaMatriz(ni);

  for (i = 0; i < ni; i++){
    for (j = 0; j < nj; j++){
      setElem(A, i, j, (((DATA_TYPE) i*j) / ni));
    }
  }
  for (i = 0; i < ni; i++){
    for (j = 0; j < ni; j++){
      setElem(C, i, j, (((DATA_TYPE) i*j) / ni));
      setElem(C_copy, i, j, (((DATA_TYPE) i*j) / ni));
    }
  }
}


/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
// static
// void print_array(int ni,
// 		 DATA_TYPE POLYBENCH_2D(C,NI,NI,ni,ni))
// {
//   int i, j;

//   for (i = 0; i < ni; i++)
//     for (j = 0; j < ni; j++) {
// 	fprintf (stderr, DATA_PRINTF_MODIFIER, C[i][j]);
// 	if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
//     }
//   fprintf (stderr, "\n");
// }

static
void print_array(int ni)
{
  int i, j;
  // fprintf (stderr, "%d \n", ni);

  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++) {
  fprintf (stderr, DATA_PRINTF_MODIFIER, getElem(C, i, j));
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


int main(int argc, char** argv)
{
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;


  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;
  // POLYBENCH_2D_ARRAY_DECL(C,DATA_TYPE,NI,NI,ni,ni);
  // POLYBENCH_2D_ARRAY_DECL(A,DATA_TYPE,NI,NJ,ni,nj);

  /* Initialize array(s). */
  /*init_array (ni, nj, &alpha, &beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));*/

  int numtasks,              /* number of tasks in partition */
  taskid,                /* a task identifier */
  numworkers,            /* number of worker tasks */
  source,                /* task id of message source */
  dest,                  /* task id of message destination */
  mtype,                 /* message type */
  rows,                  /* rows of matrix A sent to each worker */
  averow, extra, offset, /* used to determine rows sent to each worker */
  i, j, k, rc;           /* misc */
// double  a[NI][NI],           /* matrix A to be multiplied */
//        c[NI][NJ];           /* result matrix C */
MPI_Status status;



MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
if (numtasks < 2 ) {
  printf("Need at least two MPI tasks. Quitting...\n");
  MPI_Abort(MPI_COMM_WORLD, rc);
  exit(1);
  }
numworkers = numtasks-1;

init_array (ni, nj, &alpha, &beta);


  if (taskid == MASTER)
   {

    polybench_start_instruments;
      // printf("syrk_mpi has started with %d tasks.\n",numtasks);
      // printf("Initializing arrays...\n");
      // print_array(NI);

      /* Send matrix data to the worker tasks */
      averow = NI/numworkers;
      extra = NI%numworkers;
      offset = 0;

      /*int data_division = ni/numworkers;
      int start = data_division * taskid;
      int end = data_division * (taskid + 1);*/
      mtype = FROM_MASTER;

      for (i = 0; i < NI; i++)
        for (j = 0; j < NI; j++)
          setElem(C, i, j, (getElem(C, i, j) * beta));
          

      /*printf("%d, %d\n", ni, numworkers);
      printf("%d, %d, %d\n", data_division, start, end);*/


      for (dest=1; dest<=numworkers; dest++)
      {
          /*printf("varios\n");*/
         rows = (dest <= extra) ? averow+1 : averow;    
         // printf("Sending %d rows to task %d offset=%d\n", rows, dest, offset);

         MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
         // MPI_Send(POLYBENCH_ARRAY(C), NI*NI, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         // printf("%lf\n", getElem(A, offset, 0));
         // double *d1 = getElem(A, offset, 0);
         // double *d1 = &A->elementos[offset*A->tam];
         // mat->elementos[lin*mat->tam + col];
         // MPI_Send(d1, rows*NI, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         // printf("ok\n");
         // double d2 = getElem(B_copy, 0, 0);
         // MPI_Send(&d2, NI*NI, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         /*MPI_Send(&A, NI*NJ, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);*/
         // printf("ok2\n");
         // print_array(NI);
         offset = offset + rows;
      }

      /* Receive results from worker tasks */
      mtype = FROM_WORKER;
      for (i=1; i<=numworkers; i++)
      {
         source = i;
         MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);

         // MPI_Recv(&[offset][0], rows*NI, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
         MPI_Recv(&C_copy->elementos[offset*C_copy->tam], rows*NI, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
         // printf("Received results from task %d\n",source);

        //  if (source == 5)
        //  {
        //    for (i = offset; i < offset + rows; i++){
        //   for (j = 0; j < NI; j++){
        //     printf("%lf ", getElem(C_copy, i, j));
        //   }
        //   printf("\n");
        // }
        //  }
         
      }
      



      for (i = 0; i < NI; i++)
        for (j = 0; j < NI; j++)
          setElem(C, i, j, getElem(C, i, j) + getElem(C_copy, i, j));
          // C[i][j] *= beta;

      /* Print results */
      /*printf("******************************************************\n");
      printf("Result Matrix:\n");
      for (i=0; i<NI; i++)
      {
         printf("\n"); 
         for (j=0; j<NJ; j++) 
            printf("%6.2f   ", c[i][j]);
      }
      printf("\n******************************************************\n");
      printf ("Done.\n");*/
      // print_array(NI);
      
   }


   if (taskid > MASTER)
   {
      mtype = FROM_MASTER;
      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
      // printf("%p\n", A);
      // MPI_Recv(A->elementos, rows*NI, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      // MPI_Recv(&B_copy, NI*NI, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

      // printf("%d %d\n", rows, offset);
      // printf("%d %d\n", taskid, NI);
      // for (i = 0; i < rows; i++){
      //   for (j = 0; j < NI; j++){
      //     printf("%lf ", getElem(A, i, j));
      //   }
      // }

      for (i = offset; i < rows + offset; i++){
        for (j = 0; j < NI; j++){
          setElem(C_copy, i, j, 0.0);
          for (k = 0; k < NI; k++){
            setElem(C_copy, i, j, getElem(C_copy, i, j) + getElem(A, i, k) * getElem(A, k, j));
          }
          setElem(C_copy, i, j, (getElem(C_copy, i, j) * alpha));
        }
      }

      // char buf[1024];
      // char ibuf[512];
      // buf[0] = '\0';
      // for (i=0; i < rows; i++){
      //   for (j = 0; j < NI; j++){
      //     sprintf(ibuf, "%lf ", getElem(C_copy, i, j));
      //     strcat(buf, ibuf);
      //   }
      //   sprintf(ibuf, "\n");
      //   strcat(buf, ibuf);
      // }
      // printf("C %d - %s\n", taskid, buf);

      // printf("\n");

      // buf[0] = '\0';
      // for (i=0; i < rows; i++){
      //   for (j = 0; j < NI; j++){
      //     sprintf(ibuf, "%lf ", getElem(A, i, j));
      //     strcat(buf, ibuf);
      //   }
      //   sprintf(ibuf, "\n");
      //   strcat(buf, ibuf);
      // }
      // printf("A %d - %s\n", taskid, buf);
      

      mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&C_copy->elementos[offset*C_copy->tam], rows*NI, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);

      // printf("%d\n", (int) A);


      // for (i = 0; i < offset; i++)
      //   for (j = 0; j < rows; j++)
      //     A[i][j] *= beta;
      // printf("ok\n");
      // print_array(rows*NI);
      // printf("%d, %d, %d\n", offset, rows, rows*NI);
      /*printf("%d, %d\n", offset, rows);*/
      /*printf("chegoy\n");
      MPI_Recv(&A, NI*NI, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
      printf("passou\n");*/

      /*kernel_syrk(ni, nj, alpha, beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));*/

      /*mtype = FROM_WORKER;
      MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);*/
      /*MPI_Send(&C, rows*NJ, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);*/
   }


   if (taskid == MASTER) {
     polybench_stop_instruments;
     polybench_print_instruments;
   }

   MPI_Finalize();




  /* Start timer. */
  // polybench_start_instruments;

  /* Run kernel. */
  /*kernel_syrk (ni, nj, alpha, beta, POLYBENCH_ARRAY(C), POLYBENCH_ARRAY(A));*/

  /* Stop and print timer. */
  // polybench_stop_instruments;
  // polybench_print_instruments;










  /* Prevent dead-code elimination. All live-out data must be printed
     by the function call in argument. */
  // polybench_prevent_dce(print_array(ni, POLYBENCH_ARRAY(C)));
  

  /* Be clean. */
  // POLYBENCH_FREE_ARRAY(C);
  // POLYBENCH_FREE_ARRAY(A);

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C_copy);
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}
