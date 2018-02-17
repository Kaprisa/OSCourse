#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include <map>
#include <set>

using namespace std;

// Структура для сортировки слов в set (в порядке убывания)
struct comparator {
    bool operator()(pair<string, int> const& a, pair<string, int> const& b) const {
        if (a.second == b.second) return true;
        return a.second > b.second;
    };

};

struct thread_data {
    int thread_id; // PID потока
    char const *name; // имя файла
    map<string, int>* m; // указатель на контейнер для слов
};

void *ProcessFile(void *thr) {
    struct thread_data *d;
    d = (struct thread_data *) thr;
    string s;
    ifstream f(d->name);
    if (f.is_open()) {
        while (f >> s) {
            pair<map<string, int >::iterator, bool> existing;
            existing = d->m->insert(pair<string, int >(s, 1)); // Добавить новое слово
            if (!existing.second) existing.first->second++; // Если слово уже существует, увеличить его счетчик
        }
        f.close(); // Освободить ресурсы
    } else {
        cout << "Не удалось открыть файл " << d->name << endl;
    }
    cout << "PID потока : " << d->thread_id << endl;
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[argc - 1];
    struct thread_data td[argc - 1];
    int rc;
    map<string, int> m;
    void *status;
    for (int i = 0; i < argc - 1; i++) {
        cout << "Создание потока " << i << endl;
        td[i].thread_id = i;
        td[i].name = argv[i + 1];
        td[i].m = &m; // передать адрес контейнера, чтобы все потоки записывали слова в него
        rc = pthread_create(&threads[i], NULL, ProcessFile, (void *) &td[i]); // создать поток
        if (rc) {
            cout << "Не удалось создать поток, ошибка " << rc << endl;
            exit(-1);
        }
    }
    for (int i = 0; i < argc - 1; i++) {
        rc = pthread_join(threads[i], &status); // ожидание завершения потока
        if (rc) {
            cout << "Не удалось присоединить поток, ошибка " << rc << endl;
            exit(-1);
        }
        cout << "Выполнен поток " << i << ", выход со статусом " << status << endl;
    }
    cout << "Найдено " << m.size() << " слов" << endl;
    set<pair<string, int>, comparator> se;
    for (auto i = m.begin(); i != m.end(); i ++)
      se.insert(pair<string, int>(i->first, i->second));
    for (auto i = se.begin(); i != se.end(); i++) // Вывод результатов
        cout << i->first << ": " << i->second << endl;
    pthread_exit(NULL);
}
