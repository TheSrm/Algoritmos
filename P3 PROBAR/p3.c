//Autores y logins
//Jose Manuel Villar Garcia
//jose.villarg
//Sergio Regueiro Martínez
//sergio.regueiro.martinez
//Daniel Ruibal Balboa
//daniel.ruibal.balboa
//Grupo 4.2

#include <sys/time.h>
#include "math.h"
#include <stdio.h>
#include <stdlib.h>

#define TAMANOPROBA 256000
#define NUMTEMPOS 10
#define INICIO_PROBA 2000
#define UMBRAL_CONFIANZA 500
#define REPETICIONS_SUBUMBRAL 10000

//Librería



struct monticulo{
    int ultimo;
    int vector[TAMANOPROBA];
};

typedef struct monticulo * pmonticulo;


void intercambiarEnMonticulo(int a, int b, pmonticulo m){
    int aux = m->vector[a];
    m->vector[a] = m->vector[b];
    m->vector[b] = aux;
}

void Hundir(pmonticulo m, int i){
    int FilloEsq, FilloDer, j;
    do {
        FilloEsq = 2*i+1;
        FilloDer = 2*(i+1);
        j = i;
        if(FilloDer <= m->ultimo && m->vector[FilloDer] < m->vector[i])
            i = FilloDer;
        if(FilloEsq <= m->ultimo && m->vector[FilloEsq] < m->vector[i])
            i = FilloEsq;
        intercambiarEnMonticulo(i,j,m);
    } while (j != i);
}




void crearMonticulo(int v [], int n, pmonticulo m){
    int i, j;

    for (i = 0; i<n; i++){
        m->vector[i]=v[i];
    }

    m->ultimo=n-1;
    for(j = m->ultimo / 2; j >= 0; j--) {
        Hundir(m, j);
    }

}


int quitarMenor(pmonticulo M){
    if (M->ultimo==-1){
        printf("No se puede quitar el menor si no hay montículo");
        return -1; // cod erro
    } else {
       int x=M->vector[0];
       M->vector[0]=M->vector[M->ultimo];
       M->ultimo--;

       if (M->ultimo > 0)
           Hundir(M,0);
       return x;
    }
}


void OrdenarPorMonticulos(int Vector[], int n){
    int i;
    pmonticulo Monticulo = malloc(sizeof(struct monticulo));

    crearMonticulo(Vector,n,Monticulo );

    for (i = 0; i < n; ++i) {
        Vector[i]=quitarMenor(Monticulo);
    }

    free(Monticulo);
}


double microsegundos() { /* obtiene la hora del sistema en microsegundos */
    struct timeval t;
    if (gettimeofday(&t, NULL) < 0 )
        return 0.0;
    return (t.tv_usec + t.tv_sec * 1000000.0);
}

void VectorOrdenado(int v[], int n){
    int i;
    for (i=0; i < n; i++) {
        v[i] = i;
    }
}

void VectorOrdenadoInverso(int v[], int n){
    int i;
    for (i=0; i < n; i++) {
        v[i]=n-i-1;
    }
}


void aleatorio(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++)
        v[i] = (rand() % m) - n;
}




void EsOrdenado( int v[], int n){
    int i, anterior = v[0];
    for (i=1; i < n; i++){
        if (anterior > v[i]){
            printf("Fallo en la definnicion, no conseguimos un vector ordenado");
            return;
        }
    }
    printf("La implementación es correcta, el vector está ordenado");
}
double tempoSubumbral(int vector[], int n,
                      void (*sort)(int*,int), void (*init)(int*, int)){
    double tempoAux1, tempoAux2;
    int i;

    tempoAux1 = microsegundos();
    for (i = 0; i < REPETICIONS_SUBUMBRAL; i++) {
        (*init)(vector, n);
        (*sort)(vector,n);
    }
    tempoAux1 = microsegundos()-tempoAux1;
    tempoAux2=microsegundos();
    for (i = 0; i < REPETICIONS_SUBUMBRAL; i++)
        (*init)(vector,n);
    tempoAux2=microsegundos()-tempoAux2;
    return (tempoAux1-tempoAux2)/REPETICIONS_SUBUMBRAL;
}

double tempo(int vector[], int n, void (*sort)(int*,int),
             void (*init)(int*, int)) {
    double tempo=-1;

    (*init)(vector,n);
    tempo = microsegundos();
    (*sort)(vector, n);
    tempo = microsegundos() - tempo;
    if (tempo < UMBRAL_CONFIANZA && tempo != -1)
        tempo = tempoSubumbral(vector,n,sort,(*init));
    return tempo;
}

//Crear MOntículo

double tempoSubumbralM(int vector[], int n,
                      void (*sort)(int*,int,pmonticulo), void (*init)(int*, int)){
    double tempoAux1, tempoAux2;
    int i;
    pmonticulo Monticulo = malloc(sizeof(struct monticulo));

    tempoAux1 = microsegundos();
    for (i = 0; i < REPETICIONS_SUBUMBRAL; i++) {
        (*init)(vector, n);
        (*sort)(vector,n,Monticulo);
    }
    tempoAux1 = microsegundos()-tempoAux1;
    tempoAux2=microsegundos();
    for (i = 0; i < REPETICIONS_SUBUMBRAL; i++)
        (*init)(vector,n);
    tempoAux2=microsegundos()-tempoAux2;
    return (tempoAux1-tempoAux2)/REPETICIONS_SUBUMBRAL;
}

double tempoM(int vector[], int n, void (*sort)(int*,int,pmonticulo),
             void (*init)(int*, int)) {
    double tempo=-1;
    pmonticulo Monticulo = malloc(sizeof(struct monticulo));

    (*init)(vector,n);
    tempo = microsegundos();
    (*sort)(vector, n,Monticulo);
    tempo = microsegundos() - tempo;
    if (tempo < UMBRAL_CONFIANZA && tempo != -1)
        tempo = tempoSubumbralM(vector,n,sort,(*init));
    return tempo;
}



void TemposCrearMonticulo(int* vector) {
    int j, k;
    double temposAlg[NUMTEMPOS];
    printf("\nCrear montículos\n\n");
    printf("n\t\tt(n)\t\t\t\tt(n)/n^0.8\t\tt(n)/n\t\t\tt(n)/n^1.2\n");
    for (j = INICIO_PROBA, k = 0; j <= TAMANOPROBA; j *= 2, k++) {
        temposAlg[k] = tempoM(vector, j,crearMonticulo,aleatorio);
        printf("%10d%21.7f%26.7f%24.7f%24.7f\n",
               j, temposAlg[k], temposAlg[k] / pow(j, 0.8),
               temposAlg[k] /j, temposAlg[k] / pow(j, 1.2));
    }
}

void imprimirOrdenado(int* vector) {
    int j, k;
    double temposAlg[NUMTEMPOS];
    printf("\nVectores ordenados:\n\n");
    printf("Ordenacion por monticulo \n\n\tn\t\tt(n)\t\t\tt(n)/n^0.8\t\t"
           "t(n)/n\t\t\tt(n)/n^1.2\n");
    for (j = INICIO_PROBA, k = 0; j <= TAMANOPROBA; j *= 2, k++) {
        temposAlg[k] = tempo(vector, j, OrdenarPorMonticulos, VectorOrdenado);
        printf("%10d%21.7f%26.7f%24.7f%24.7f\n",
               j, temposAlg[k], temposAlg[k] / pow(j, 0.8),
               temposAlg[k] /pow(j,1.06), temposAlg[k] / pow(j, 1.2));
    }
}

void imprimirReves(int* vector) {
    int j, k;
    double temposAlg[NUMTEMPOS];
    printf("\nVectores ordenados al revés:\n\n");
    printf("Ordenacion por monticulo \n\n\t\tn\t\t\t\tt(n)\t\t\t\t\tt(n)/n^0.8\t\t\t\t"
           "t(n)/n\t\t\t\t\tt(n)/n^1.2\n");
    for (j = INICIO_PROBA, k = 0; j <= TAMANOPROBA; j *= 2, k++) {
        temposAlg[k] = tempo(vector, j, OrdenarPorMonticulos, VectorOrdenadoInverso);
        printf("%10d%21.7f%26.7f%24.7f%24.7f\n",
               j, temposAlg[k], temposAlg[k] / pow(j, 0.8),
               temposAlg[k] / j, temposAlg[k] / pow(j, 1.2));
    }
}//As cotas non están probadas, so etá cambiado para

void imprimirAleatorio(int* vector) {
    int j, k;
    double temposAlg[NUMTEMPOS];
    printf("\nVectores aleatorios:\n\n");
    printf("Ordenacion por monticulo \n\n\t\tn\t\t\t\tt(n)\t\t\t\t\tt(n)/n^0.8\t\t\t\t"
           "t(n)/n\t\t\t\t\tt(n)/n^1.2\n");
    for (j = INICIO_PROBA, k = 0; j <= TAMANOPROBA; j *= 2, k++) {
        temposAlg[k] = tempo(vector, j, OrdenarPorMonticulos, aleatorio);
        printf("%10d%21.7f%26.7f%24.7f%24.7f\n",
               j, temposAlg[k], temposAlg[k] / pow(j, 0.8),
               temposAlg[k] / j, temposAlg[k] / pow(j, 1.2));
    }//Cotas no calculadoas
}

void Test(int N){
    int VectorOrdeado[N],VectorOriginal[N], i;
    pmonticulo Monticulo = malloc(sizeof(struct monticulo));

    aleatorio(VectorOriginal,N);

    crearMonticulo(VectorOriginal,N,Monticulo);

    printf("\n");

    printf("Si aplicamos sucesivamente el quitar menor, obtendremos un vector de numeros ordenado, este es:\n\n");

    for (i = 0; i < N; ++i) {
        VectorOrdeado[i]=quitarMenor(Monticulo);
        printf("%6d",VectorOrdeado[i]);
        if ((i + 1) % 20== 0)
        printf("\n");
    }

    printf("\n\n");
    EsOrdenado(VectorOrdeado,N);
    printf("\n");

    free(Monticulo);

}

    void imprimirTempos(){
        int *vector=malloc(TAMANOPROBA*sizeof(int));
        // Con vectores ordeados
        for (int i = 0; i <5 ; ++i) {
            imprimirOrdenado(vector);
        }
        // Con vectores ordeados ao revés
        //imprimirReves(vector);
        // Con vectores aleatorios
        //imprimirAleatorio(vector);
        //for (int i = 0; i < 10; ++i) {
          //  TemposCrearMonticulo(vector);
        //}

        free(vector);
    }


int main(){
    //Test(40);
    imprimirTempos();

}
