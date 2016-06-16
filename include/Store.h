#ifndef STORE_H
#define STORE_H

#include <iostream>
#include "Treap.h"
#include "Vector.h"

using std::pair;

//typedef pair<double, double> Store;

struct Store {
    double x;
    double y;
    int id;

    struct task7_cmp {
        inline bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
            if (a.second == b.second)
                return a.first < b.first;
            return a.second > b.second;
        }
    };

    // Treap pentru task 4
    Treap<int, int>* treap_task4;

    // Treap pentru task 5
    Treap<int, int>* treap_task5;

    // Treap pentru task 6
    Treap<double, int>* treap_task6;

    // Treap care contine fiecare secunda la care a fost inregistrata o vizita
    Treap<int, int>* treap_sec_task7;

    // Treap care contine drept cheie o pereche <indice zi, numar de vizite din acea zi>
    Treap<pair<int, int>, bool, task7_cmp>* treap_day_task7;

    Store() {
        x = y = id = 0;
        treap_task4 = new Treap<int, int>();
        treap_task5 = new Treap<int, int>();
        treap_task6 = new Treap<double, int>();
        treap_sec_task7 = new Treap<int, int>();
        treap_day_task7 = new Treap<pair<int, int>, bool, task7_cmp>();
    }

    Store(const Store& other) {
        x = other.x;
        y = other.y;
        id = other.id;
        treap_task4 = new Treap<int, int>(*other.treap_task4);
        treap_task5 = new Treap<int, int>(*other.treap_task5);
        treap_task6 = new Treap<double, int>(*other.treap_task6);
        treap_sec_task7 = new Treap<int, int>(*other.treap_sec_task7);
        treap_day_task7 = new Treap<pair<int, int>, bool, task7_cmp>(*other.treap_day_task7);
    }

    Store& operator=(const Store& other) {
        if (treap_task4 != NULL)
            delete treap_task4;
        if (treap_task5 != NULL)
            delete treap_task5;
        if (treap_task6 != NULL)
            delete treap_task6;
        if (treap_sec_task7 != NULL)
            delete treap_sec_task7;
        if (treap_day_task7 != NULL)
            delete treap_day_task7;
        x = other.x;
        y = other.y;
        id = other.id;
        treap_task4 = new Treap<int, int>(*other.treap_task4);
        treap_task5 = new Treap<int, int>(*other.treap_task5);
        treap_task6 = new Treap<double, int>(*other.treap_task6);
        treap_sec_task7 = new Treap<int, int>(*other.treap_sec_task7);
        treap_day_task7 = new Treap<pair<int, int>, bool, task7_cmp>(*other.treap_day_task7);
        return *this;
    }

    Store(int Id, double X, double Y) : id(Id), x(X), y(Y) {
        treap_task4 = new Treap<int, int>();
        treap_task5 = new Treap<int, int>();
        treap_task6 = new Treap<double, int>();
        treap_sec_task7 = new Treap<int, int>();
        treap_day_task7 = new Treap<pair<int, int>, bool, task7_cmp>();
    }

    ~Store() {
        delete treap_task4;
        delete treap_task5;
        delete treap_task6;
        delete treap_sec_task7;
        delete treap_day_task7;
    }
};

#endif // STORE_H
