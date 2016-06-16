#include "../include/Treap.h"
#include <iostream>

using std::cout;

int main() {
    Treap<int, int>* t = new Treap<int, int>();

    for (int i = 0; i < 100000; i++)
        t = t->insert(i, i+1);

    cout << t->validate_values() << '\n';

    for (int k = 0; k < 10; k++) {
        Treap<int, int>::iterator it = t->nth_element(k);
        if (it == t->end())
            cout << "k = " << k << " element nu exista.\n";
        else
            cout << "k = " << k << ", cheie = " << (*it).first << " " << ", valoare = " << (*it).second << '\n';
    }

    for (int k = 0; k < 10; k++) {
        Treap<int, int>::iterator it = t->find(k);
        cout << "Suma pana la " << k << " este " << t->prefix_sum(it) << '\n';
    }

    // copiem treapul
    Treap<int, int> *q = new Treap<int, int>(*t);

    for (int k = 0; k < 10; k++) {
        t = t->insert(k, -k);
    }

    cout << t->validate_values() << ' ' << q->validate_values() << '\n';


    for (int k = 0; k < 10; k++) {
        Treap<int, int>::iterator it = t->find(k);
        cout << "Suma pana la k " << k << " este " << t->prefix_sum(it) << '\n';
    }

    for (int k = 0; k < 10; k++) {
        t = t->erase(t->find(k));
    }

    cout << t->validate_values() << ' ' << q->validate_values() << '\n';


    for (int k = 0; k < 13; k++) {
        Treap<int, int>::iterator it = t->find(k);
        if (it == t->end()) continue;
        cout << "Suma pana la k " << k << " este " << t->prefix_sum(it) << '\n';
    }

    for (int k = 0; k < 10; k++) {
        Treap<int, int>::iterator it = t->nth_element(k);
        if (it == t->end())
            cout << "k = " << k << " element nu exista.\n";
        else
            cout << "k = " << k << ", cheie = " << (*it).first << " " << ", valoare = " << (*it).second << '\n';
    }

    // verificam daca treapul copiat face bine ce face
    for (int k = 0; k < 10; k++) {
        Treap<int, int>::iterator it = q->nth_element(k);
        if (it == q->end())
            cout << "k = " << k << " element nu exista.\n";
        else
            cout << "k = " << k << ", cheie = " << (*it).first << " " << ", valoare = " << (*it).second << '\n';
    }

    cout << t->validate_values() << ' ' << q->validate_values() << '\n';

    cout << t->size() << ' ' << q->size() << '\n';

    for (int i = 0; i < 15; i++) {
        cout << (*(t->upper_bound(i))).first << ' ' << (*(q->upper_bound(i))).first << '\n';
    }

    for (int i = 99990; i < 100015; i++) {
        cout << (*(t->lower_bound(i))).first << ' ' << (*(q->lower_bound(i))).first << '\n';
    }

    cout << (*(t->upper_bound(10))).first << '\n';

    // Calculam suma pe intervalul [0, 100], o data ca bossii, o data ca tantalaii
    // rezultatele ar trebui sa fie identice
    cout << t->prefix_sum(100) - t->prefix_sum(0) << '\n';

    int sum = 0;
    for (int i = 0; i <= 100; i++) {
        Treap<int, int>::iterator it = t->find(i);
        if (it == t->end())
            continue;
        sum += (*it).second;
    }

    cout << sum << '\n';

    delete t;
    delete q;

    return 0;
}
