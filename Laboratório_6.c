#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10
#define P 5
#define C 5

//Variáveis Globais
int buffer[N];
int cont = 0;

sem_t slotVazio, slotCheio; //Semáforo para sincronização
sem_t mutexProd, mutexCons; //Semáforo para exclusão mútua

//Inicializando o Buffer
void iniciaBuffer(int n){
    int i;
    for (i = 0; i < n; i++)
        buffer[i] = 0;
}

//Imprimindo o Buffer
void imprimeBuffer(int n){
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", buffer[i]);
    printf("\n");
}

//Inserindo elementos no buffer
void insere(int item, int id){
    static int in = 0;

    sem_wait(&slotVazio);
    sem_wait(&mutexProd);
    printf("Produtor %d: Quer inserir item.\n", id);

    for(int i = 0; i < N; i++){
        buffer[in] = item;
        in = (in + 1) % N;
        sem_post(&slotCheio);
        cont++;
    }
    printf("Produtor %d: Inseriu o item %d no Buffer.\n", id, item);
    
    imprimeBuffer(N);
    printf("Produtor %d: Imprimiu o Buffer.\n", id);
    printf("\n");

    sem_post(&mutexProd);
}

//Retirando elementos do buffer
int retira(int id){
    static int out = 0;
    int item;

    sem_wait(&slotCheio);
    sem_wait(&mutexCons);
    printf("Consumidor %d: Quer remover item.\n", id);

    item = buffer[out];
    buffer[out] = 0;
    out = (out + 1) % N;
    cont--;
    printf("Consumidor %d: Removeu o item %d do Buffer.\n", id, item);

    imprimeBuffer(N);
    printf("Consumidor %d: Imprimiu o Buffer.\n", id);
    printf("\n");

    sem_post(&mutexCons);

    //Sinalizando se o Buffer estiver vazio
    if(cont == 0){
        sem_post(&slotVazio);
    }
    return item;
}

void *produtor(void *arg){
    int *id = (int *)arg;

    while (1){
        insere(*id, *id);
        sleep(1);
    }
    pthread_exit(NULL);
}

void *consumidor(void *arg){
    int id = *(int *)arg;
    int item;

    while (1){
        item = retira(id);
        sleep(1);
    }

    pthread_exit(NULL);
}

int main(void){
    pthread_t produtores[P];
    pthread_t consumidores[C];
    int i, id_prod[P], id_cons[C];

    //Inicializando o Buffer
    iniciaBuffer(N);

    //Inicializando os semáforos
    sem_init(&mutexProd, 0, 1);
    sem_init(&mutexCons, 0, 1);
    sem_init(&slotVazio, 0, N);
    sem_init(&slotCheio, 0, 0);

    //Criando as threads dos produtores
    for (i = 0; i < P; i++){
        id_prod[i] = i + 1;
        pthread_create(&produtores[i], NULL, produtor, (void *)&id_prod[i]);
    }

    //Criando as threads dos consumidores
    for (i = 0; i < C; i++){
        id_cons[i] = i + 1;
        pthread_create(&consumidores[i], NULL, consumidor, (void *)&id_cons[i]);
    }

    //Aguardando as threads terminarem
    for (i = 0; i < P; i++){
        pthread_join(produtores[i], NULL);
    }
    for (i = 0; i < C; i++){
        pthread_join(consumidores[i], NULL);
    }

    sem_destroy(&mutexCons);
    sem_destroy(&mutexProd);
    sem_destroy(&slotVazio);
    sem_destroy(&slotCheio);

    return 0;
}
