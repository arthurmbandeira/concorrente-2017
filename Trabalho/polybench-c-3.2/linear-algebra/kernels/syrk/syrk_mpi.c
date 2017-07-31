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

Matriz *A, *A_copy, *C, *C_copy, *C_copy2, *C_copy3;

static void init_array(int ni, int nj, DATA_TYPE *alpha, DATA_TYPE *beta) {
  int i, j;

  *alpha = 32412;
  *beta = 2123;

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

static void print_array(int ni){
  int i, j;

  for (i = 0; i < ni; i++)
    for (j = 0; j < ni; j++) {
  fprintf (stderr, DATA_PRINTF_MODIFIER, getElem(C, i, j));
  if ((i * ni + j) % 20 == 0) fprintf (stderr, "\n");
    }
  fprintf (stderr, "\n");
}


int main(int argc, char** argv){
  /* Retrieve problem size. */
  int ni = NI;
  int nj = NJ;


  /* Variable declaration/allocation. */
  DATA_TYPE alpha;
  DATA_TYPE beta;

  int numtasks,              /* number of tasks in partition */
  taskid,                /* a task identifier */
  numworkers,            /* number of worker tasks */
  source,                /* task id of message source */
  dest,                  /* task id of message destination */
  mtype,                 /* message type */
  rows,                  /* rows of matrix A sent to each worker */
  averow, extra, offset, /* used to determine rows sent to each worker */
  i, j, k, rc;           /* misc */
  MPI_Status status;


  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  if (numtasks < 2 ){
    printf("Need at least two MPI tasks. Quitting...\n");
    MPI_Abort(MPI_COMM_WORLD, rc);
    exit(1);
  }
  numworkers = numtasks-1;


  if (taskid == MASTER){

    // Start time measure with communication
    polybench_start_instruments;
    
    init_array(ni, nj, &alpha, &beta);

    /* Send matrix data to the worker tasks */
    averow = NI/numworkers;
    extra = NI%numworkers;
    offset = 0;

    mtype = FROM_MASTER;

    for (dest=1; dest<=numworkers; dest++){
      rows = (dest <= extra) ? averow+1 : averow;

      MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
      MPI_Send(&A->elementos[0], NI*NI, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
      MPI_Send(&C->elementos[offset * C->tam], rows*NI, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    /* Receive results from worker tasks */
    mtype = FROM_WORKER;
    for (i=1; i<=numworkers; i++){
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);
      MPI_Recv(&C->elementos[offset*C->tam], rows*NI, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
       
    }   
  }


  if (taskid > MASTER){
        
    A_copy = novaMatriz(ni);
    C_copy2 = novaMatriz(ni);
    C_copy3 = novaMatriz(ni);

    mtype = FROM_MASTER;

    MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

    MPI_Recv(&A_copy->elementos[0], NI*NI, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
    MPI_Recv(&C_copy2->elementos[offset*C_copy2->tam], rows*NI, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);

    
    // // Start time measure without communication
    // polybench_start_instruments;

    for (i = offset; i < rows + offset; i++){
      for (j = 0; j < NI; j++){
        setElem(C_copy2, i, j, (getElem(C_copy2, i, j) * beta));
      }
    }

    for (i = offset; i < rows + offset; i++){
      for (j = 0; j < NI; j++){
        for (k = 0; k < NI; k++){
          setElem(C_copy3, i, j, getElem(C_copy3, i, j) + getElem(A_copy, i, k) * getElem(A_copy, k, j));
        }
        setElem(C_copy3, i, j, (getElem(C_copy3, i, j) * alpha));
      }
    }

    for (i = offset; i < rows + offset; i++)
      for (j = 0; j < NI; j++)
        setElem(C_copy3, i, j, getElem(C_copy3, i, j) + getElem(C_copy2, i, j));

    // // End time measure without communication
    // polybench_stop_instruments;
    // polybench_print_instruments;


    mtype = FROM_WORKER;
    MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);
    MPI_Send(&C_copy3->elementos[offset*C_copy3->tam], rows*NI, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);


    /* Be clean. */
    POLYBENCH_FREE_ARRAY(A_copy);
    POLYBENCH_FREE_ARRAY(C_copy2);
    POLYBENCH_FREE_ARRAY(C_copy3);
  }


  // End time measure with communication
  if (taskid == MASTER) {
    polybench_stop_instruments;
    polybench_print_instruments;
  }

  MPI_Finalize();

  /* Be clean. */
  POLYBENCH_FREE_ARRAY(C_copy);
  POLYBENCH_FREE_ARRAY(C);
  POLYBENCH_FREE_ARRAY(A);

  return 0;
}
