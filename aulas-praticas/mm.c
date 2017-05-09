#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <omp.h>
#include <math.h>
#include <time.h>

#define ARQUIVO 1
#define MAX_THREAD 1000

struct SMatriz{
    int tam;
    int *elementos;
};
typedef struct SMatriz Matriz;

typedef struct {
    int id;
    int noproc;
    int dim;
    Matriz *A;
    Matriz *B;
    Matriz *C;
} parm;

Matriz *novaMatriz(int t){
    int i;
    Matriz *mat = malloc(sizeof(Matriz));
    mat->tam = t;
    mat->elementos = (int *)malloc(t*t*sizeof(int*));
    return mat;
}

int getElem(Matriz *mat, int lin, int col){
    return mat->elementos[lin*mat->tam + col];
}

void setElem(Matriz *mat, int lin, int col, int val){
    mat->elementos[lin*mat->tam + col] = val;
}

void preencheMatriz(Matriz *m1, Matriz *m2, int tam){
    int i, j;

    srand(time(NULL));
    int r, s = rand();


    for (i = 0; i < tam; i++){
        for (j = 0; j < tam; j++){
            r = rand()%(999);
            s = rand()%(999);
            setElem(m1, i, j, r);
            setElem(m2, i, j, s);
        }
    }
}

void imprimeMatriz(Matriz *mat){
    int i, j;
    
    printf("   ");
    for (i = 0; i < mat->tam; i++){
        if (i > 10){
            printf(" %d ", i);
        }
        else{
            printf("  %d", i);
        }
    }
    printf("\n\n");
    for (i = 0; i < mat->tam; i++){
        if (i < 10){
            printf(" ");
        }
        printf("%d  ", i);
        for (j = 0; j < mat->tam; j++){
            if (getElem(mat, i, j) < 10){
                printf(" %d ", getElem(mat, i, j));
            }
            else{
                printf("%d ", getElem(mat, i, j));
            }
        }
        printf("\n");
    }
}


void *multMatriz(Matriz *m1, Matriz *m2, Matriz *out){

    int i, j, k, aux;

    aux = 0;

    for (i=0; i < m1->tam; i++) {
        for (j=0; j < m1->tam; j++) {
            for (k=0; k < m2->tam; k++) {
                aux = getElem(out, i, j) + (getElem(m1, i, k) * getElem(m2, k, j));
                setElem(out, i, j, aux);
            }
        }
    }
}

void *multMatrizPthread_WORKING(int me_no, int noproc, int n, Matriz *m1, Matriz *m2, Matriz *out){

    int i, j, k, aux;

    i = me_no;

    while (i < n){
        for (j=0; j < n; j++) {
            aux = 0;
            for (k=0; k < n; k++) {
                aux = aux + (getElem(m1, i, k) * getElem(m2, k, j));
            }
            setElem(out, i, j, aux);
        }
        i += noproc;
    }
}

void *multMatrizPthread(int me_no, int noproc, int n, Matriz *m1, Matriz *m2, Matriz *out){

    int i, j, k, aux;

    /*i = me_no;*/

    aux = 0;

    for (i = me_no; i < n; i += noproc){
        for (j=0; j < n; j++) {
            /*aux = 0;*/
            for (k=0; k < n; k++) {
                aux = getElem(out, i, j) + (getElem(m1, i, k) * getElem(m2, k, j));
                setElem(out, i, j, aux);
            }   
        }
    }
}

void *multMatrizOpenmp(Matriz *m1, Matriz *m2, Matriz *out){

    int i, j, k, aux;

    for (i=0; i < m1->tam; i++) {
        for (j=0; j < m1->tam; j++) {
            for (k=0; k < m2->tam; k++) {
                aux = getElem(out, i, j) + (getElem(m1, i, k) * getElem(m2, k, j));
                setElem(out, i, j, aux);
            }
        }
    }
}

void * worker(void *arg){
    parm *p = (parm *) arg;
    multMatrizPthread(p->id, p->noproc, p->dim, p->A, p->B, p->C);
    return NULL;
}


int main(int argc, char *argv[]){

    int i, j, n, ndim;

    pthread_t *threads;
    pthread_attr_t pthread_custom_attr;

    parm *arg;

    struct timespec start, finish;
    double elapsed;

    if (argc != 3)
    {
        printf ("Usage: %s n\n  where n is no. of threads\n",argv[0]);
        exit(1);
    }

    n=atoi(argv[2]);

    if ((n < 1) || (n > MAX_THREAD))
    {
        printf ("The no of thread should between 1 and %d.\n",MAX_THREAD);
        exit(1);
    }

    Matriz *matA;
    Matriz *matB;
    Matriz *matSaida;

    ndim = atoi(argv[1]);

    matA = novaMatriz(ndim);
    matB = novaMatriz(ndim);
    matSaida = novaMatriz(ndim);

    preencheMatriz(matA, matB, ndim);

    threads = (pthread_t *)malloc(n*sizeof(*threads));
    pthread_attr_init(&pthread_custom_attr);

    arg = (parm *)malloc(sizeof(parm)*n);



    clock_gettime(CLOCK_MONOTONIC, &start);

    // SEQUENCIAL

    /*multMatriz(matA, matB, matSaida);*/

    // FIM SEQUENCIAL

    // PARALELO

    for (i = 0; i < n; i++){
        arg[i].id = i;
        arg[i].noproc = n;
        arg[i].dim = ndim;
        arg[i].A = matA;
        arg[i].B = matB;
        arg[i].C = matSaida;
        pthread_create(&threads[i], &pthread_custom_attr, worker, (void *)(arg+i));
    }

    for (i = 0; i < n; i++){
        pthread_join(threads[i],NULL);
    }
    
    // FIM PARALELO

    clock_gettime(CLOCK_MONOTONIC, &finish);

    free(arg);

    /*imprimeMatriz(matA);
    printf("\n");
    imprimeMatriz(matB);
    printf("\n");
    imprimeMatriz(matSaida);*/

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("Total time: %f\n\n", elapsed);

    return 0;
}
