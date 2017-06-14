#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define ARQUIVO 1
#define MAX_THREAD 1000


struct SMatriz{
    int tam;
    int *elementos;
};
typedef struct SMatriz Matriz;


typedef struct Elem{
    int index;
    void *(*func);
    struct Elem *next;
} Elem;

typedef struct Queue{
    Elem *begin;
    Elem *end;
    int size;
} Queue;

typedef struct {
    int id;
    int num_threads;
    Queue *bag;
    Queue *working_bag;
} p_args;

Matriz *novaMatriz(int t){
    int i;
    Matriz *mat = malloc(sizeof(Matriz));
    mat->tam = t;
    mat->elementos = (int *)malloc(t*t*sizeof(int*));
    return mat;
}

Elem *newElem(int index, void *(*func)){
    Elem *elem = malloc(sizeof(Elem));
    elem->index = index;
    elem->func = func;
    elem->next = NULL;
    return elem;
}

Queue *newQueue(){
    Queue *q = malloc(sizeof(Queue));
    q->begin = NULL;
    q->end = NULL;
    q->size = 0;
    return q;
}

int emptyQueue(Queue *Q){
    if (Q->begin == NULL)
        return 1;
    return 0;
}

Elem *getElem(int index, Queue *Q){
    if (emptyQueue(Q)) return NULL;

    Elem *aux = Q->begin;
    while (aux != NULL && aux->index != index)
        aux = aux->next;
    return aux;
}

void insertElem(int index, void *(*func), Queue *Q){
    Elem *e = newElem(index, func);

    if (emptyQueue(Q)){
        Q->begin = e;
        Q->end = e;
    } else {
        Q->end->next = e;
        Q->end = e;
    }
    Q->size++;
}

int existsHeadInQueue(int x, Queue *Q){
    Elem *e = Q->begin;
    while (e != NULL){
        if (e->index == x) return 1;
        e = e->next;
    }
    return 0;
}

void printElem(Elem *elem){
    Elem *aux;
    for (aux = elem; aux != NULL; aux = aux->next)
        printf("%d - %p\n", aux->index, aux->func);
}

int getElemMatrix(Matriz *mat, int lin, int col){
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
            if (getElemMatrix(mat, i, j) < 10){
                printf(" %d ", getElemMatrix(mat, i, j));
            }
            else{
                printf("%d ", getElemMatrix(mat, i, j));
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
                aux = getElemMatrix(out, i, j) + (getElemMatrix(m1, i, k) * getElemMatrix(m2, k, j));
                setElem(out, i, j, aux);
            }
        }
    }
}

void *findInArray(int key, int *v, int r){
    int i;
    for (i = 0; i < sizeof(v); i++){
        if (v[i] == key){
            r = 1;
        }
    }
    r = 0;
}

void createOnBag(void *(*task), Queue *bag, int index){
    insertElem(index, task, bag);
    printElem(getElem(index, bag));
}

void getFromBag(){}

void *worker(void *arg){
    p_args *p = (p_args *) arg;
    /*int id;
    int num_threads;
    Queue *bag;
    Queue *working_bag;*/
    printf("%d\n", p->id);
    return NULL;
}

void *request(void *arg){
    p_args *p = (p_args *) arg;

    srand(time(NULL));
    int r = rand();

    int i;
    
    while (1){
        r = rand()%(p->bag->size);
        printf("%d\n", r);
        Elem *e = getElem(r, p->bag);
        insertElem(e->index, e->func, p->working_bag);
        /*for (i = 0; i < working_bag->size; i++){
            printElem(getElem(i, working_bag));
            printf("\n");
        }*/
    }

    return NULL;
}

int main(int argc, char *argv[]){

    int i, j, n, ndim;

    pthread_t *threads;
    pthread_attr_t pthread_custom_attr;

    p_args *arg;

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

    arg = (p_args *)malloc(sizeof(p_args)*n);

    
    srand(time(NULL));
    int r = rand();
    

    int arr[100];

    for (i = 0; i < (sizeof(arr)/sizeof(arr[0])); i++){
        r = rand()%(99);
        arr[i] = r;
    }

    Queue *disponible_tasks = newQueue();
    Queue *working_bag = newQueue();
    int result;
    
    createOnBag(multMatriz(matA, matB, matSaida), disponible_tasks, 0);
    createOnBag(findInArray(10, arr, result), disponible_tasks, 1);

    clock_gettime(CLOCK_MONOTONIC, &start);

    // THREAD REQUEST
    arg[0].id = i;
    arg[0].num_threads = 1;
    arg[0].bag = disponible_tasks;
    arg[0].working_bag = working_bag;
    pthread_create(&threads[0], &pthread_custom_attr, request, (void *)(arg));
    pthread_join(threads[0],NULL);

    // FIM THREAD REQUEST

    // PARALELO

    for (i = 1; i < (n + 1); i++){
        arg[i].id = i;
        arg[i].num_threads = n;
        arg[i].bag = disponible_tasks;
        arg[i].working_bag = working_bag;
        pthread_create(&threads[i], &pthread_custom_attr, worker, (void *)(arg+i));
    }

    for (i = 0; i < (n + 1); i++){
        pthread_join(threads[i],NULL);
    }
    
    // FIM PARALELO

    clock_gettime(CLOCK_MONOTONIC, &finish);

    free(arg);


    

    /*int res;
    printf("\n");
    res = findInArray(arr[4], arr);
    printf("%d\n", res);*/

    /*createOnBag(1, bag);*/
    
    /*printf("%p\n", multMatriz(matA, matB, matSaida));
    printf("%p\n", findInArray(10, arr, result));*/


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
