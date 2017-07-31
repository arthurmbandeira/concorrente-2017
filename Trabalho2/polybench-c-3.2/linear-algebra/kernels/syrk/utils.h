struct SMatriz{
    int tam;
    DATA_TYPE *elementos;
};
typedef struct SMatriz Matriz;

Matriz *novaMatriz(int t);

DATA_TYPE getElem(Matriz *mat, int lin, int col);

void setElem(Matriz *mat, int lin, int col, DATA_TYPE val);