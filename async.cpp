#include <iostream>
#include <fstream>
#include <future>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <thread>

using namespace std;

mutex mtx;

template<class Iterator, class F, class Q>
auto map_reduce(Iterator p, Iterator q, F f1, Q f2,  size_t threads) -> decltype(f2(f1(*p), f1(*p)))
{
    Iterator m;
    auto result = f1(*p);
    if (threads <= 1) m = q;
    else {
        size_t d = distance(p, q) / threads;
        m = p;
        advance(m, d);
        future<decltype(f2(f1(*p), f1(*m)))> h = async(launch::async, map_reduce<Iterator, F, Q>, m, q, f1, f2, threads - 1);
        result = f2(result , h.get());
    }
    while(++p != m)
        result = f2(result, f1(*p));
    return result;
};

template<class Iterator, class F, class Q>
auto map_reduce2(Iterator p, Iterator q, F f1, Q f2,  size_t threads) -> decltype(f2(f1(*p), f1(*p)))
{
    Iterator m;
    auto result = f1(*p);
    if (threads <= 1) m = q;
    else {
        size_t d = distance(p, q) / threads;
        m = p;
        advance(m, d);
        decltype(f2(f1(*p), f1(*p))) h;
        thread t([&](){ h = map_reduce2<Iterator, F, Q>(m, q, f1, f2, threads - 1); });
        t.join();
        result = f2(result , h);
    }
    while(++p != m)
        result = f2(result, f1(*p));
    return result;
};

template<class Iterator, class F>
void parallel_p(Iterator p, Iterator q, F f1, size_t threads)
{
    Iterator m;
    if (threads <= 1) m = q;
    else {
        size_t d = distance(p, q) / threads;
        m = p;
        advance(m, d);
        thread t([&](){ parallel_p<Iterator, F>(m, q, f1, threads - 1); });
        t.join();
    }
    f1(*p);
    while(++p != m)
        f1(*p);
};

struct comparator {
    bool operator()(pair<string, int> const& a, pair<string, int> const& b) const {
        if (a.second == b.second) return true;
        return a.second > b.second;
    };

};

map<string, int> m;

void processFile(const string &name) {
    string s; // Обработка файла
    ifstream f(name);
    pair<map<string, int >::iterator, bool> existing;
    if (f.is_open()) {
        while (f >> s) {
            //pthread_mutex_lock(&mutex); // Блокировка, т.к используется глобальный контейнер
            lock_guard<mutex> lk(mtx);
            existing = m.insert(pair<string, int >(s, 1)); // Добавить новое слово
            if (!existing.second) existing.first->second++; // Если слово уже существует, увеличить его счетчик
            //pthread_mutex_unlock(&mutex);
        }
        f.close(); // Освободить ресурсы
    } else {
        cout << "Не удалось открыть файл " << name << endl;
    }
}

int main() {
    list<string> l = {"../a.txt", "../b.txt", "../c.txt"};
    parallel_p(l.begin(), l.end(), processFile, 3);
    for (auto & i : m)
        cout << i.first << ' ' << i.second << endl;
    set<pair<string, int>, comparator> se;
    cout << typeid(*m.begin()).name();
    //parallel_p(m.begin(), m.end(), [&se](pair<string, int> e){ se.insert(pair<string, int>(e.first, e.second)); }, 3);
    std::sort(m.begin(), m.end(), [](pair<string, int> e, pair<string, int> q){ return e.second - q.second; });
    for (auto & i : m)
        cout << i.first << ' ' << i.second << endl;
//    for (auto i = m.begin(); i != m.end(); i ++)
//        se.insert(pair<string, int>(i->first, i->second));
    return 0;
}