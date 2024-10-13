#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct ThreadTask {
  int threadNum;
  int duration;
} ThreadTask;

void *ThreadProc(void *arg) {
  ThreadTask *task = (ThreadTask *)arg;
  printf("%d-й поток начал работу\n", task->threadNum);
  sleep(task->duration); // Используем sleep() из unistd.h
  printf("%d-й поток выполнил задачу\n", task->threadNum);
  free(task);
  return NULL;
}

int main() {
  char choice;
  do {
    int durations[5];
    printf("Введите длительность работы для 5 потоков (в секундах):\n");
    for (int i = 0; i < 5; ++i) {
      printf("Длительность работы для потока %d: ", i + 1);
      scanf("%d", &durations[i]);
    }

    pthread_t threads[5];
    for (int i = 0; i < 5; ++i) {
      ThreadTask *task = (ThreadTask *)calloc(1, sizeof(ThreadTask));
      if (task == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
      }
      task->threadNum = i + 1;
      task->duration = durations[i];

      if (pthread_create(&threads[i], NULL, ThreadProc, task) != 0) {
        fprintf(stderr, "Ошибка создания потока %d\n", i + 1);
        return 1;
      }
    }

    for (int i = 0; i < 5; ++i) {
      pthread_join(threads[i], NULL);
    }

    printf("Нажмите x, чтобы выйти, нажмите r, чтобы перезапустить: ");
    scanf(" %c", &choice);
  } while (choice == 'r' || choice == 'R');

  return 0;
}

