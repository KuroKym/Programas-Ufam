#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
    int itens;
    int valor;
} carrinho;

carrinho car;
sem_t semaforo;

void *adiciona(void *arg) {
    int valor = *(int *)arg;
    sem_wait(&semaforo);

    printf("[ESCRITORA] Antes de adicionar | ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens++;
    sleep(1); // simular a perda de CPU
    car.valor += valor;
    printf("[ESCRITORA] Depois de adicionar | ITENS: %d VALOR: %d\n", car.itens, car.valor);

    sem_post(&semaforo);
    return NULL;
}

void *retira(void *arg) {
    int valor = *(int *)arg;
    sem_wait(&semaforo);

    printf("[ESCRITORA] Antes de retirar | ITENS: %d VALOR: %d\n", car.itens, car.valor);
    car.itens--;
    sleep(1);
    car.valor -= valor;
    printf("[ESCRITORA] Depois de retirar | ITENS: %d VALOR: %d\n", car.itens, car.valor);

    sem_post(&semaforo);
    return NULL;
}

void *consulta(void *arg) {
    sem_wait(&semaforo);
    printf("[LEITORA] Carrinho | ITENS: %d VALOR: %d\n", car.itens, car.valor);
    sleep(0.2);
    sem_post(&semaforo);
    return NULL;
}

int main() {
    sem_init(&semaforo, 0, 1);
    car.itens = 0;
    car.valor = 0;

    int num_adiciona, num_retira, leitoras, add, rm;

    printf("Insira a quantidade de itens que deseja adicionar, retirar e quantidade de threads leitoras: ");
    scanf("%d %d %d", &num_adiciona, &num_retira, &leitoras);

    printf("Qual valor deseja adicionar e retirar?: ");
    scanf("%d %d", &add, &rm);

    pthread_t threads[num_adiciona + num_retira + leitoras];

    for (int i = 0; i < num_adiciona; i++) {
        pthread_create(&threads[i], NULL, adiciona, &add);
    }

    for (int i = num_adiciona; i < num_adiciona + num_retira; i++) {
        pthread_create(&threads[i], NULL, retira, &rm);
    }

    for (int i = num_adiciona + num_retira; i < num_adiciona + num_retira + leitoras; i++) {
        pthread_create(&threads[i], NULL, consulta, NULL);
    }

    for (int i = 0; i < num_adiciona + num_retira + leitoras; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("VALORES FINAIS | ITENS: %d VALOR: %d\n", car.itens, car.valor);
    sem_destroy(&semaforo); 
    return 0;
}
