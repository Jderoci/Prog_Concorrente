#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  5

//Criando as Variáveis Globais
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond1;
pthread_cond_t x_cond2;

//Definindo as Threads
//Thread 1
void *thread1 (void *t){
    printf("Oi Maria!\n");
    pthread_mutex_lock(&x_mutex);
    x++;

    if(x == 2){
        pthread_cond_signal(&x_cond1);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

//Thread 2
void *thread2 (void *t){
    printf("Oi José!\n");
    pthread_mutex_lock(&x_mutex);
    x++;

    if(x == 2){
        pthread_cond_signal(&x_cond1);
    }
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

//Thread 3
void *thread3 (void *t){
    pthread_mutex_lock(&x_mutex);

    if(x < 2){
        pthread_cond_wait(&x_cond1, &x_mutex);
    }
    x++;

    printf("Sente-se por favor.\n");
    pthread_cond_signal(&x_cond2);
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

//Thread 4
void *thread4 (void *t){
    pthread_mutex_lock(&x_mutex);

    if(x < 3){
        pthread_cond_wait(&x_cond2, &x_mutex);
    }
  
    printf("Até mais José!\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

//Thread 5
void *thread5 (void *t){
    pthread_mutex_lock(&x_mutex);

    if(x < 3){
        pthread_cond_wait(&x_cond2, &x_mutex);
    }
  
    printf("Até mais Maria!\n");
    pthread_mutex_unlock(&x_mutex);
    pthread_exit(NULL);
}

//Função Principal
int main(int argc, char *argv[]){
    int i;
    pthread_t threads[NTHREADS];

    //Inicializando os recursos de sincronização
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond1, NULL);
    pthread_cond_init(&x_cond2, NULL);

    //Criando as Threads
    pthread_create(&threads[0], NULL, thread1, NULL);
    pthread_create(&threads[1], NULL, thread2, NULL);
    pthread_create(&threads[2], NULL, thread3, NULL);
    pthread_create(&threads[3], NULL, thread4, NULL);
    pthread_create(&threads[4], NULL, thread5, NULL);

    //Esperando as Threads completarem
    for (int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
    printf("\nFIM!\n");
    
    //Desalocando as Variáveis e Finalizando
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond1);
    pthread_cond_destroy(&x_cond2);
}
