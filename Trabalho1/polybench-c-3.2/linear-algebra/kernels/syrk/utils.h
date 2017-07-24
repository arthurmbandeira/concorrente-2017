struct SMatriz{
    int tam;
    DATA_TYPE *elementos;
};
typedef struct SMatriz Matriz;

Matriz *novaMatriz(int t);

// Matriz *novaMatriz(int t){
//     int i;
//     Matriz *mat = malloc(sizeof(Matriz));
//     mat->tam = t;
//     mat->elementos = (int *)malloc(t*t*sizeof(int*));
//     return mat;
// }

DATA_TYPE getElem(Matriz *mat, int lin, int col);

// int getElemMatrix(Matriz *mat, int lin, int col){
//     return mat->elementos[lin*mat->tam + col];
// }

void setElem(Matriz *mat, int lin, int col, DATA_TYPE val);

// void setElem(Matriz *mat, int lin, int col, int val){
//     mat->elementos[lin*mat->tam + col] = val;
// }