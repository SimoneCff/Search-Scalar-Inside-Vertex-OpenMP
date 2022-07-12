#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, const char * argv[]) {
    int id_thread, N,fsize, beta, numT, *a,i,findIndex=0;
    double t0,t1,t;
    char c;
    FILE *fv;
    
    printf("--------Progetto Calcolo Parallelo--------\n");
    //controllo N Threads dati in input
    if(argc >1 && atoi(argv[1])) {
        numT = atoi(argv[1]);
    } else {
        printf("ERROR, numero Threads assente o errato, shutdwon \n");
        exit(EXIT_FAILURE);
    }
    
    //controllo file input
    if((fv=fopen("vector.txt","r"))==NULL){
        printf("Errore apertura file \n");
        exit(EXIT_FAILURE);
    }
    
    //Controllo N elementi
    N=1;
    do {
        c = fgetc(fv);
        if (c == '\n') N++;
    } while(c != EOF);
    
    //reset pointer
    rewind(fv);
    printf("Vettore ha un Size Di N elementi: %d \n",N);
    
    //allocazione moemoria vettore a
    a=(int*)malloc(N*sizeof(int));
    
    //Lettura vettore A
    printf("\n Inseriremto numeri all'interno del vettore A \n\n");
    
   //lettura elementi in A
    i=0;
    while (i<N) {
        fscanf(fv,"%d\n",&a[i]);
        i++;
    }
    //chiusura file dopo inserimento vettore
    fclose(fv);
    
     //Lettore scalare Beta
    beta=-1;
    while (beta<0) {
        printf("\n Inserire valore di Beta \n");
        scanf("%d",&beta);
        if(beta<0){
            printf("\n Attenzione!, valore di Beta minore di 0, riprovare");
        }
    }

     //prima che avviene la parallelizazione
    t0=omp_get_wtime();
    printf("\n");

    printf("\n ---Inizio Paralellizazione su %d threads--- \n",numT);
    #pragma omp parallel for schedule(static) shared(N,a,beta,findIndex) private(i) num_threads(numT)
    for (i=0; i<N; i++) {
        if (a[i]==beta){
           #pragma omp atomic update
                findIndex=findIndex+1;
        }
        printf("\n sono il Thread %d, findIndex = %d \n",omp_get_thread_num(),findIndex);
      }
        

    //Seconda chiamata
    t1=omp_get_wtime();
    t=t1-t0;
    printf("\n ---------------------------------------\n");
    //Stampa risultato
    printf("\n Sono stati necessari %lf secondi per la ricerca",t);
    printf("\n lo scalare beta: %d, é stato trovato %d volte nel vettore a \n",beta,findIndex);
    
    //Free
    free(a);
    return 0;
}
