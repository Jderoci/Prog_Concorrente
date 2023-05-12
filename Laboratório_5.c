#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

//Criando as Variáveis Globais
int x = 0;
pthread_mutex_t x_mutex;
pthread_mutex_t x_cond1;
pthread_mutex_t x_cond2;

//Definindo as Threads
//Thread 1
void *thread1 (void *t){
    printf("Oi Maria!\n");
    pthread_mutex_lock(&x_mutex);
    x++;
    
    if (x==2){
        
    }
}

//Thread 2
//Thread 3
//Thread 4
//Thread 5

//Função Principal
int main(int argc, char *argv[]){
    int i;
    pthread_t threads[NTHREADS];

    //Inicializando os recursos de sincronização
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond1, NULL);
    pthread_cond_init(&cond2, NULL);

    //Criando as Threads
    pthread_create(&threads[0], NULL, thread1, NULL);
    pthread_create(&threads[1], NULL, thread2, NULL);
    pthread_create(&threads[2], NULL, thread3, NULL);
    pthread_create(&threads[3], NULL, thread4, NULL);
    pthread_create(&threads[4], NULL, thread5, NULL);

    //Esperando as threads completarem
    for (int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
    printf("\nFIM\n");
    
    //Liberando os recursos de sincronização
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond1);
    pthread_cond_destroy(&cond2);
}
