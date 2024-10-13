#include <iostream>
#include <pthread.h>
#include <unistd.h> // для sleep()
#include <vector>
#include <limits> 

class ThreadTask {
public:
    ThreadTask(int threadNum, int duration)
        : threadNum(threadNum), duration(duration) {}

    void Run() {
        std::cout << threadNum << "-й поток начал работу" << std::endl;
        sleep(duration); // Используем sleep() из unistd.h
        std::cout << threadNum << "-й поток выполнил задачу" << std::endl;
    }

private:
    int threadNum;
    int duration;
};

void* thread_function(void* arg) {
 
    ThreadTask* task = static_cast<ThreadTask*>(arg);

 
    task->Run();

 
    delete task;

    return nullptr; 
}

int main() {
    char choice;
    do {
 
        std::vector<int> durations(5);
        std::cout << "Введите длительность работы для 5 потоков (в секундах):" << std::endl;
        for (int i = 0; i < 5; ++i) {
            int duration;
            while (true) {
                std::cout << "Длительность работы для потока " << (i + 1) << ": ";
                std::cin >> duration;

 
                if (std::cin.fail() || duration < 0) {
                    std::cin.clear(); // Сбрасываем состояние ошибки
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
                    std::cout << "Ошибка ввода! Пожалуйста, введите целое неотрицательное число." << std::endl;
                } else {
                    durations[i] = duration;
                    break; // Выходим из цикла при корректном вводе
                }
            }
        }

        pthread_t threads[5];
        for (int i = 0; i < 5; ++i) {
            ThreadTask* task = new ThreadTask(i + 1, durations[i]);
            if (pthread_create(&threads[i], NULL, thread_function, task) != 0) {
                std::cerr << "Ошибка создания потока " << (i + 1) << std::endl;
                return 1;
            }
        }

 
        for (int i = 0; i < 5; ++i) {
            pthread_join(threads[i], NULL);
        }

 
        std::cout << "Нажмите x, чтобы выйти, нажмите r, чтобы перезапустить: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Очищаем буфер ввода
    } while (choice == 'r' || choice == 'R');

    return 0;
}
