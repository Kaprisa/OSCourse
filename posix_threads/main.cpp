#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include <map>
#include <set>

// Программа считает колличество одинаковых слов в файлах и выводит их в порядке убывания слово : колличество
// Сборка: make
// Запуск: ./main Файлы [Колличество потоков]
// где файлы - перечисление файлов через запятую
// Если колличество потоков не указано, то оно равно колличеству файлов
// Пример: ./main a.txt b.txt c.txt 2

using namespace std;

pthread_mutex_t mutex;
set<const char*> files; // Множество файлов для обработки
map<string, int> m;

// Структура для сортировки слов в set (в порядке убывания)
struct comparator {
    bool operator()(pair<string, int> const& a, pair<string, int> const& b) const {
        if (a.second == b.second) return true;
        return a.second > b.second;
    };

};

bool process_file() {
    if (!files.empty()) {
        pthread_mutex_lock(&mutex); // Блокировка, т.к используется глобальный контейнер
        auto e = files.begin(); // Извлечь файл для обработки данным потоком
        files.erase(e);
        pthread_mutex_unlock(&mutex);
        string s; // Обработка файла
        ifstream f(*e);
        if (f.is_open()) {
            while (f >> s) {
                pair<map<string, int >::iterator, bool> existing;
                pthread_mutex_lock(&mutex); // Блокировка, т.к используется глобальный контейнер
                existing = m.insert(pair<string, int >(s, 1)); // Добавить новое слово
                if (!existing.second) existing.first->second++; // Если слово уже существует, увеличить его счетчик
                pthread_mutex_unlock(&mutex);
            }
            f.close(); // Освободить ресурсы
        } else {
            cout << "Не удалось открыть файл " << *e << endl;
        }
        return process_file();
    }
    return false; // Если файлы закончились, закончить рекурсию
}

void *ProcessFile(void *pid) {
    int* id = (int *) pid;
    while(process_file()) { };
    cout << "PID потока : " << *id << endl;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int threads_count = isdigit(argv[argc - 1][0]) ? atoi(argv[argc - 1]) : argc - 1; // Определить колличество потоков
    pthread_mutex_init(&mutex, NULL); // Инициализация мьютекса
    pthread_t threads[threads_count];
    int td[threads_count];
    int rc;
    void *status;
    for (int i = 1; i < (isdigit(argv[argc - 1][0]) ? argc - 1 : argc); i++) {
        files.insert(argv[i]);
    }
    for (int i = 0; i < threads_count; i++) {
        cout << "Создание потока " << i << endl;
        td[i] = i;
        rc = pthread_create(&threads[i], NULL, ProcessFile, (void *) &td[i]); // создать поток
        if (rc) {
            cout << "Не удалось создать поток, ошибка " << rc << endl;
            exit(-1);
        }
    }
    for (int i = 0; i < threads_count; i++) {
        rc = pthread_join(threads[i], &status); // ожидание завершения потока
        if (rc) {
            cout << "Не удалось присоединить поток, ошибка " << rc << endl;
            exit(-1);
        }
        cout << "Выполнен поток " << i << ", выход со статусом " << status << endl;
    }
    pthread_mutex_destroy(&mutex); // Освободить ресурсы мьютекса
    cout << "Найдено " << m.size() << " слов" << endl;
    set<pair<string, int>, comparator> se; // Множество для сортировки слов
    for (auto i = m.begin(); i != m.end(); i ++)
        se.insert(pair<string, int>(i->first, i->second));
    for (auto i = se.begin(); i != se.end(); i++) // Вывод результатов
        cout << i->first << ": " << i->second << endl;
    pthread_exit(NULL);
}
