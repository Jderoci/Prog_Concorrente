#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int buffer_index = 0;

sem_t empty_slots;   // Semáforo para controlar os slots vazios no buffer
sem_t filled_slots;  // Semáforo para controlar os slots preenchidos no buffer
sem_t mutex;         // Semáforo para exclusão mútua no acesso ao buffer

void inserir(int item) {
    sem_wait(&empty_slots);  // Aguarda por um slot vazio no buffer
    sem_wait(&mutex);        // Obtém acesso exclusivo ao buffer

    // Insere o item no buffer
    buffer[buffer_index] = item;
    buffer_index = (buffer_index + 1) % BUFFER_SIZE;

    sem_post(&mutex);        // Libera o acesso ao buffer
    sem_post(&filled_slots); // Indica que um slot foi preenchido
}

int retirar() {
    int item;

    sem_wait(&filled_slots); // Aguarda por um slot preenchido no buffer
    sem_wait(&mutex);        // Obtém acesso exclusivo ao buffer

    // Remove o item do buffer
    item = buffer[buffer_index];
    buffer_index = (buffer_index - 1 + BUFFER_SIZE) % BUFFER_SIZE;

    sem_post(&mutex);        // Libera o acesso ao buffer
    sem_post(&empty_slots);  // Indica que um slot foi esvaziado

    return item;
}

void *produtor(void *arg) {
    int id = *((int *)arg);

    while (1) {
        int item = rand() % 100;  // Gera um número aleatório para produzir
        inserir(item);
        printf("Produtor %d inseriu item %d\n", id, item);
    }
}

void *consumidor(void *arg) {
    int id = *((int *)arg);

    while (1) {
        int item = retirar();
        printf("Consumidor %d retirou item %d\n", id, item);
    }
}

int main() {
    int num_produtores, num_consumidores;
    int i;

    // Inicializa os semáforos
    sem_init(&empty_slots, 0, BUFFER_SIZE);
    sem_init(&filled_slots, 0, 0);
    sem_init(&mutex, 0, 1);

    printf("Digite o número de produtores: ");
    scanf("%d", &num_produtores);
    printf("Digite o número de consumidores: ");
    scanf("%d", &num_consumidores);

    // Cria as threads dos produtores
    pthread_t produtores[num_produtores];
    int produtores_ids[num_produtores];
    for (i = 0; i < num_produtores; i++) {
        produtores_ids[i] = i;
        pthread_create(&produtores[i], NULL, produtor, &produtores_ids[i]);
    }

    // Cria as threads dos consumidores
    pthread_t consumidores[num_consumidores];
    int consumidores_ids[num_consumidores];
    for (i = 0; i < num_consumidores; i++) {
        consumidores_ids[i] = i;
        pthread_create(&consumidores[i], NULL, consumidor, &consumidores_ids[i]);
    }
  
      // Aguarda a finalização das threads dos consumidores
    for (i = 0; i < num_consumidores; i++) {
        pthread_join(consumidores[i], NULL);
    }

    // Realiza a limpeza dos semáforos
    sem_destroy(&empty_slots);
    sem_destroy(&filled_slots);
    sem_destroy(&mutex);

    return 0;
}
