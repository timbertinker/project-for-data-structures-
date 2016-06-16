#ifndef TREAP_H
#define TREAP_H

#include <cstdlib>
#include <ctime>
#include <functional> // pentru std::less
#include <iostream>

using std::make_pair;
using std::pair;
using std::srand;
using std::time;

// Treapul va retine perechi de genul Cheie, Valoare.
// De asemenea, va primi ca template parameter si un Comparator pentru chei,
// care va da ordinea in care vor fi sortate cheile in treap, e.g:
//
// struct my_comp {
//      inline bool operator()(pair<int, int>& a, const pair<int, int>& b) {
//          if (a.second == b.second)
//              return a.first < b.first;
//          return a.second < b.second;
//      }
// };
//
// Treap<pair<int, int>, int, my_comp> treap;
//
// Acesta poate ramane si neprecizat, default-ul fiind std::less<K>,
// in cazul in care acesta este definit.
//
// Astfel, vom putea afla suma de valori pe un interval de chei date.
//
// Spre exemplu, un posibil use-case ar fi gasirea discountului total
// pe un interval de timp dat.
//
// ******************************************************************
// *        Descrierea metodelor ce se pot apela pe treap.          *
// ******************************************************************
//
// **************
// *** Constructor default
// *** Descriere: Initializeaza campurile pentru a specifica faptul
//                ca nodul curent este un nod gol. La prima inserare,
//                acest nod este refolosit.
// *** Complexitate Timp: O(1).
//
// **************
// *** Copy constructor/assigment
// *** Descriere: Initializeaza un nou treap pornind de la altul.
// *** Complexitate Timp: O(N).
// *** Alte mentiuni: de preferat ar fi sa nu il folosim niciodata,
//                    caci este extrem de costisitor.
//
// **************
// *** begin()
// *** Descriere: Returneaza un iterator catre cel mai mic nod din treap,
//                bineinteles in ordinea data de comparator.
//
// *** Complexitate Timp: O(log N).
//
// **************
// *** rbegin()
// *** Descriere: Returneaza un reverse_iterator catre cel mai mare nod
//                din treap, bineinteles in ordinea data de comparator.
//
// *** Complexitate Timp: O(log N).
//
// **************
// *** end()
// *** Descriere: Returneaza un iterator care reprezinta sfarsitul treapului,
//                am stabilit prin conventie ca iteratorul acesta sa fie
//                iterator(NULL) - caci predecesorul primului nod si succesorul
//                ultimului nod vor fi NULL.
//
// *** Complexitate Timp: O(1).
//
// **************
// *** rend()
// *** Descriere: Returneaza un reverse_iterator care reprezinta sfarsitul
//                treapului, analog cu end() - acesta este un reverse_iterator(NULL)
//
// *** Complexitate Timp: O(1).
//
// **************
// *** find(Key k), rfind(Key k)
// *** Descriere: Returneaza un iterator/reverse_iterator care va "pointa" catre
//                nodul cu cheia k din treap.
// *** Alte mentiuni: Daca x nu este o cheie in treap, atunci
//                    find(x) == end() sau rfind(x) == rend().
//
// *** Complexitate Timp: O(log N).
//
// **************
// *** insert(Key k, Value v), insert_unique(Key k, Value v)
// *** Descriere: insert(): In cazul in care cheia k nu exista in treap,
//                          se va insera un nou nod cu informatia data ca parametru.
//    ++++++++++++++++++    In cazul in care cheia k EXISTA in treap,
//    !! ATENTIE AICI !!    la valoarea de pe nodul corespunzator cheii k se va aduna
//    ++++++++++++++++++    noua valoare.
//                          In cazul in care Value este un tip user-defined, se poate
//                          suprascrie operator+() astfel incat sa putem schimba modul
//                          in care se comporta adunarea aici.
//                          Acest operator+ trebuie sa prezinte urmatoarea proprietate:
//                          a + Value() = a;
//                          Cu alte cuvinte, constructorul default al tipului Value
//                          trebuie sa fie element neutru al operator+().
//                insert_unique():
//                          In cazul in care cheia k nu exista in treap,
//                          se va insera un nou nod cu informatia data ca parametru.
//                          In cazul in care cheia exista, nu se intampla nimic!
//
// *** Return value: Atat insert() cat si insert_unique() vor returna pointer catre
//                   radacina treapului dupa operatia de adaugare.
//                   De ce se intampla acest lucru? Deoarece este posibil ca in urma
//                   rotirilor pentru echilibrarea treapului, radacina sa se schimbe.
//                   Astfel:
//
//      ++++++++++++++++++++++++++++++++++++++++++++
//      |         RIGHT        |      WRONG        |
//      ++++++++++++++++++++++++++++++++++++++++++++
//      | t = t->insert(10,5); | t->insert(10, 5); |
//      ++++++++++++++++++++++++++++++++++++++++++++
//
// *** Complexitate Timp: O(log N).
//
// **************
// *** erase(iterator), erase(reverse_iterator), erase(Key k)
// *** Descriere: Din treap se sterge, in functie de ce varianta a lui erase() se apeleaza,
//                fie nodul catre care "pointeaza" iteratorul dat,
//                fie nodul cu cheia k - BEWARE, aici se sterge tot nodul din treap,
//                                       indiferent de cate operatii insert(k, v1),
//                                       insert(k, v2), insert(k, v3), .... au fost anterior.
//
// *** Return value: Vezi insert() pentru explicatii.
//                   Astfel:
//
//      ++++++++++++++++++++++++++++++++++++++++++++
//      |         RIGHT        |      WRONG        |
//      ++++++++++++++++++++++++++++++++++++++++++++
//      | t = t->erase(10);    | t->erase(10);     |
//      ++++++++++++++++++++++++++++++++++++++++++++
//
// *** Complexitate Timp: O(log N).
//
//
// **************
// *** prefix_sum(iterator)
// *** Descriere: Din treap se face suma tuturor valorilor mai mici decat cheia nodului
//                dat prin iteratorul parametru.
//
//
// *** Complexitate Timp: O(log N).
//
// **************
// *** nth_element(int n)
// *** Descriere: In treap se cauta al n-ulea element in parcurgerea inordine.
//                Atentie: n este indexat de la 0, deci poate lua valori
//                         in intervalul [0, size), size = numarul de noduri din treap
//
// *** Return value: iterator ce pointeaza catre nodul cautat.
//                   end() in cazul in care acesta nu exista.
//
// *** Complexitate Timp: O(log N).

static bool is_random_seeded = false; // flag care retine daca a mai fost
                                      // apelat srand() inainte, de vreun constructor
                                      // pe vreo instanta Treap.


template<typename Key, typename Value, typename Comp = std::less<Key> >
class Treap {
    private:
        // definim tipul aici, pentru a fi mai usor de schimbat codul in cazul in care
        // este nevoie de modificarea parametrilor template.
        typedef Treap<Key, Value, Comp> type;
        bool  is_empty;
        type  *left, *right, *parent;
        Value subtree_value; // valoarea pe (sub)tree-ul din nodul acesta.
        int   subtree_nodes; // numarul de noduri din (sub)tree-ul din nodul acesta.

        Key key;
        Value value;

        int priority;
        const static int MAXIMUM_PRIORITY =  0x7fffffff; // valoarea maxima pe 32 biti
        const static int MINIMUM_PRIORITY = -0x80000000; // valoarea minima pe 32 biti

        // Iteratorul intern va lua ca parametru template un struct ce contine o functie
        // care determina modul in care va fi calculat ++it:
        //
        // 1. pentru reverse_iterator va fi predecesor inordine,
        // 2. pentru iterator va fi succesor inordine.
        template<typename Next>
        class internal_iterator {
            private:
                // ii dam acces clasei Treap la campul privat ptr.
                friend class Treap;

                type *ptr; // pointer catre nodul din treap catre
                           // care face referire iteratorul acesta

            public:
                // constructori
                internal_iterator() {
                    ptr = NULL;
                }

                internal_iterator(type *p) {
                    ptr = p;
                }

                // copy constructor
                internal_iterator(const internal_iterator<Next> &other) {
                    ptr = other.ptr;
                }

                // destructor
                ~internal_iterator() {}

                // copy assignment
                internal_iterator<Next>& operator=(const internal_iterator<Next> &other) {
                    ptr = other.ptr;
                    return *this;
                }

                // (in)equality check

                bool operator==(const internal_iterator<Next> &other) {
                    return ptr == other.ptr;
                }

                bool operator!=(const internal_iterator<Next> &other) {
                    return ptr != other.ptr;
                }

                // prefix ++
                internal_iterator<Next>& operator++() {
                    ptr = Next()(ptr);
                    return *this;
                }

                // postfix ++, dummy parameter.
                internal_iterator<Next> operator++(int) {
                    // fac o copie a iteratorului
                    internal_iterator<Next> tmp = *this;

                    // incrementez iteratorul it folosind prefix
                    ++(*this);

                    // returnez copia veche
                    return tmp;
                }

                // returnam pereche <key, value> a nodului pointat de catre iterator
                pair<Key, Value> operator*() {
                    return make_pair(ptr->key, ptr->value);
                }
        };

        struct next_forward {
            type* operator()(type *p) {
                return p->successor();
            }
        };

        struct next_reverse {
            type* operator()(type *p) {
                return p->predecessor();
            }
        };

        type* internal_find(const Key& k) {
            // Cum dispunem doar de Comp::operator()(a, b) care returneaza true
            // pentru a < b, putem echivala operatia de >= cu !(a < b)
            // si, deci, a == b o putem echivala cu a >= b && b >= a,
            // adica !(a < b) && !(b < a).
            //
            // Altfel spus: a == b e echivalent cu !(a < b) && !(b < a)
            //              a > b e echivalent cu !(a < b) && !(a == b)

            if (is_empty)
                return NULL;

            bool lt = Comp()(k, this->key);
            bool gt = Comp()(this->key, k);

            if (lt && this->left != NULL) {
                return this->left->internal_find(k);
            } else if (!lt && !gt) {
                return this;
            } else if (this->right != NULL) {
                return this->right->internal_find(k);
            }

            return NULL;
        }

        // Returneaza un pointer catre nodul care reprezinta al n-ulea element din parcurgerea inordine
        type* internal_nth_element(int n) {
            // n trebuie sa se afle in intervalul [0, nr noduri)
            if (get_subtree_nodes(this) <= n || n < 0)
                return NULL;

            // Nodul cautat se afla in subarborele din stanga
            if (get_subtree_nodes(this->left) > n && this->left) {
                return this->left->internal_nth_element(n);
            } else {
                n -= get_subtree_nodes(this->left);
                if (n == 0)
                    return this;

                n--;
                return this->right->internal_nth_element(n);
            }
        }

        // Returneaza un pointer catre nodul care are cheia cea mai mare <= o cheie data,
        // sau NULL in cazul in care acesta nu exista.
        type* internal_lower_bound(const Key& k) {
            if (is_empty)
                return NULL;

            type* ptr = this;
            type* ans = NULL;

            while (ptr) {
                bool lt = Comp()(k, ptr->key);
                if (lt) {
                    ptr = ptr->left;
                } else {
                    ans = ptr;
                    ptr = ptr->right;
                }
            }

            return ans;
        }

        // Returneaza un pointer catre nodul care are cheia cea mai mica > o cheie data,
        // sau NULL in cazul in care acesta nu exista.
        type* internal_upper_bound(const Key& k) {
            if (is_empty)
                return NULL;

            type* ptr = this;
            type* ans = NULL;

            while (ptr) {
                bool lt = Comp()(k, ptr->key);
                if (lt) {
                    ans = ptr;
                    ptr = ptr->left;
                } else {
                    ptr = ptr->right;
                }
            }

            return ans;
        }

        // Returneaza suma valorilor tuturor nodurilor cu cheia <= cheia nodului catre care pointeaza ptr.
        Value internal_prefix_sum(type *ptr) {
            type *p = ptr->parent;
            Value ans = ptr->value + get_subtree_value(ptr->left);

            while (p != NULL) {
                if (p->right == ptr) {
                    ans += p->value + get_subtree_value(p->left);
                }

                ptr = p;
                p = p->parent;
            }

            return ans;
        }

        // Functii helper pentru update-uri la rotiri
        void set_parent(type *n, type *p) {
            if (n)
                n->parent = p;
        }

        void set_subtree_value(type *n, Value v) {
            if (n)
                n->subtree_value = v;
        }

        Value get_subtree_value(type *n) {
            if (n)
                return n->subtree_value;
            return Value(); // valoarea pe un constructor default,
                            // ar trebui sa fie element neutru pentru operator+(Value, Value)
        }

        void set_subtree_nodes(type *n, int v) {
            if (n)
                n->subtree_nodes = v;
        }

        int get_subtree_nodes(type *n) {
            if (n)
                return n->subtree_nodes;
            return 0;
        }

        // Rotiri la stanga / la dreapta
        void rotleft(type* &n) {
            type *t = n->left;
            set_parent(t, n->parent);
            set_subtree_value(t, get_subtree_value(n));
            set_subtree_nodes(t, get_subtree_nodes(n));
            set_parent(t->right, n);
            set_subtree_nodes(n, get_subtree_nodes(t->right) + get_subtree_nodes(n->right) + 1);
            set_subtree_value(n, get_subtree_value(t->right) + get_subtree_value(n->right) + n->value);
            set_parent(n, t);
            n->left = t->right;
            t->right = n;
            n = t;
        }

        void rotright(type* &n) {
            type *t = n->right;
            set_parent(t, n->parent);
            set_subtree_value(t, get_subtree_value(n));
            set_subtree_nodes(t, get_subtree_nodes(n));
            set_parent(t->left, n);
            set_subtree_nodes(n, get_subtree_nodes(t->left) + get_subtree_nodes(n->left) + 1);
            set_subtree_value(n, get_subtree_value(t->left) + get_subtree_value(n->left) + n->value);
            set_parent(n, t);
            n->right = t->left;
            t->left = n;
            n = t;
        }

        // Echilibrarea treapului cu rotiri la stanga / la dreapta
        void balance(type* &n) {
            if (n->left != NULL && n->left->priority > n->priority)
                rotleft(n);
            else if (n->right != NULL && n->right->priority > n->priority)
                rotright(n);
        }

        bool internal_insert(type* &n, Key k, Value v, int priority) {
            if (n->is_empty) {
                n->key = k;
                n->value = v;
                n->subtree_value = v;
                n->subtree_nodes = 1;
                n->priority = priority;
                n->is_empty = 0;
                return true;
            }

            // n se va afla mereu pe lantul de la radacina la nodul inserat, deci
            // va fi mereu o radacina a unui subarbore care contine nodul nou.
            // Astfel, incrementam subtree_value pentru nodul curent.
            n->subtree_value = n->subtree_value + v;

            // Compara la fel ca internal_find

            bool lt = Comp()(k, n->key); // less than
            bool gt = Comp()(n->key, k); // greater than

            bool le = !gt;               // less or equal
            bool ge = !lt;               // greater or equal

            bool eq = le && ge;          // equal

            bool ok;

            if (lt) {
                // Cazul in care inseram in stanga
                if (n->left == NULL) {
                    n->left = new type(k, v, priority);
                    n->left->parent = n;
                    ok = true;
                } else {
                    ok = internal_insert(n->left, k, v, priority);
                }
            } else if (eq) {
                // Incrementam valoarea din nodul curent si ne oprim, nu este nevoie de o reechilibrare
                n->value = n->value + v;
                return false;
            } else { // gt
                if (n->right == NULL) {
                    n->right = new type(k, v, priority);
                    n->right->parent = n;
                    ok = true;
                } else {
                    ok = internal_insert(n->right, k, v, priority);
                }
            }

            if (ok)
                n->subtree_nodes++;

            balance(n);

            return ok;
        }

        type* internal_erase(type* &n, const Key& k, const Value& v) {
            if (n->is_empty)
                return n;

            bool lt = Comp()(k, n->key);
            bool gt = Comp()(n->key, k);

            if (lt) {
                n->subtree_value -= v;
                n->subtree_nodes--;
                internal_erase(n->left, k, v);
            } else if (!lt && !gt) {
                if (n->left == NULL && n->right == NULL) {
                    if (n->parent == NULL) {
                        // radacina?
                        n->is_empty = true;
                        n->priority = 0;
                        n->value = Value();
                        n->key = Key();
                        n->subtree_nodes = 0;
                        n->subtree_value = Value();

                        return n;
                    } else {
                        delete n;
                        return n = NULL;
                    }
                } else {
                    if (n->left == NULL) {
                        rotright(n);
                    } else if (n->right == NULL) {
                        rotleft(n);
                    } else if (n->left->priority > n->right->priority) {
                        rotleft(n);
                    } else {
                        rotright(n);
                    }

                    internal_erase(n, k, v);
                }
            } else {
                n->subtree_value -= v;
                n->subtree_nodes--;
                internal_erase(n->right, k, v);
            }

            return n;
        }

        // operatii de gasire a successorului / predecesorului
        // din parcurgerea inordine

        type* successor() {
            if (is_empty)
                return NULL;

            type *current = this;

            // Cazul 1. are fiu dreapta nodul curent, deci este cel mai din stanga
            //          nod din parcurgerea inordine a subarborelui drept.
            if (current->right != NULL) {
                current = current->right;
                while (current->left != NULL)
                    current = current->left;
                return current;
            }
            // Cazul 2. nu are fiu dreapta, deci parcurgem stramosii acestui nod
            //          in ordine pana cand gasim doi stramosi consecutivi dintre care unul
            //          este fiul stanga al celuilalt.
            else {
                type *p = this->parent;
                while (p != NULL && current == p->right) {
                    current = p;
                    p = p->parent;
                }

                return p;
            }
        }

        type* predecessor() {
            // Analog lui successor().

            if (is_empty)
                return NULL;

            type *current = this;

            // Cazul 1. are fiu stanga.
            if (current->left != NULL) {
                current = current->left;
                while (current->right != NULL)
                    current = current->right;

                return current;
            }
            // Cazul 2. nu are fiu stanga
            else {
                type *p = current->parent;
                while (p != NULL && current == p->left) {
                    current = p;
                    p = current->parent;
                }

                return p;
            }
        }

        // Acest constructor ramane privat.
        Treap(Key k, Value v, int prio) : key(k), value(v), subtree_value(v), subtree_nodes(1), priority(prio) {
            left = right = parent = NULL;
            is_empty = 0;
        }


    public:
        // constructor default
        Treap() : key(Key()), value(Value()), subtree_value(Value()), subtree_nodes(0), priority(0) {
            // seeduim pseudo rng-ul.
            if (!is_random_seeded) {
                int seed = time(NULL);
                srand(seed);
                is_random_seeded = true;
            }

            left = right = parent = NULL;
            is_empty = 1;
        }

        // destructor
        ~Treap() {
            if (left != NULL) delete left;
            if (right != NULL) delete right;
        }

        // ATENTIE: copy constructor este O(N), USE IT SPARINGLY!
        // copy constructor
        Treap(const type& other) :
            key(other.key), value(other.value), subtree_value(other.subtree_value),
            subtree_nodes(other.subtree_nodes), priority(other.priority),
            is_empty(other.is_empty) {

            parent   = NULL;

            // Copiem recursiv treapul
            if (other.left != NULL) {
                left = new type(*other.left);
                left->parent = this;
            } else {
                left = NULL;
            }

            if (other.right != NULL) {
                right = new type(*other.right);
                right->parent = this;
            } else {
                right = NULL;
            }
        }

        // ATENTIE: operator= este O(N), USE IT SPARINGLY!
        type* operator=(const type& other) {
            this->key = other.key;
            this->value = other.value;
            this->subtree_value = other.subtree_value;
            this->subtree_nodes = other.subtree_nodes;
            this->priority = other.priority;
            this->is_empty = other.is_empty;
            this->parent   = NULL;

            if (this->left != NULL) {
                delete this->left;
                this->left = new type(*other.left);
                this->left->parent = this;
            } else {
                this->left = NULL;
            }

            if (this->right != NULL) {
                delete this->right;
                this->right = new type(*other.right);
                this->right->parent = this;
            } else {
                this->right = NULL;
            }
        }

        // definirea iteratorilor folosind internal_iterator

        typedef internal_iterator<next_forward> iterator;
        typedef internal_iterator<next_reverse> reverse_iterator;

        // operatii de gasirea celui mai din stanga / din dreapta nod
        // in parcurgerea inordine.

        iterator begin() {
            type *node = this;

            if (node->is_empty) // daca radacina e goala, begin() == end() == NULL.
                return NULL;

            while (node->left != NULL)
                node = node->left;

            return iterator(node);
        }

        reverse_iterator rbegin() {
            type *node = this;

            if (node->is_empty)
                return NULL;

            while (node->right != NULL)
                node = node->right;

            return reverse_iterator(node);
        }

        // iterator pentru end() si rend()
        iterator end() {
            return iterator(NULL);
        }

        reverse_iterator rend() {
            return reverse_iterator(NULL);
        }


        // metode wrapper pentru internal_*()
        iterator find(Key k) {
            return iterator(internal_find(k));
        }

        reverse_iterator rfind(Key k) {
            return reverse_iterator(internal_find(k));
        }

        type* insert(Key k, Value v) {
            type* ptr = this;
            internal_insert(ptr, k, v, rand());
            return ptr;
        }

        type* insert_unique(Key k, Value v) {
            if (internal_find(k) != NULL) {
                // daca cheia exista, nu mai este updatata valoarea din nodul acesteia.
                return this;
            }

            return insert(k, v);
        }

        // ATENTIE: o cheie inexistenta data ca parametru la erase() nu va influenta treapul.
        // DAR un iterator invalid va da, cel mai probabil, segmentation fault!
        type* erase(Key k) {
            iterator it = find(k);
            if (it == end())
                return this;

            type* ptr = this;
            return internal_erase(ptr, (*it).first, (*it).second);
        }

        type* erase(iterator it) {
            type* ptr = this;
            return internal_erase(ptr, (*it).first, (*it).second);
        }

        type* erase(reverse_iterator rit) {
            type* ptr = this;
            return internal_erase(this, (*rit).first, (*rit).second);
        }

        // Calculeaza valoarea pe toate nodurile <= nodul catre care pointeaza iteratorul it.
        Value prefix_sum(iterator it) {
            return internal_prefix_sum(it.ptr);
        }

        // Calculeaza valoarea pe toate nodurile cu cheia <= k
        Value prefix_sum(Key k) {
            type* ptr = internal_lower_bound(k);
            if (ptr == NULL) // Nu a fost gasit niciun nod <= k, deci suma este elementul neutru.
                return Value();
            return internal_prefix_sum(ptr);
        }

        // Returneaza iterator catre al n-ulea element din treap
        iterator nth_element(int n) {
            return iterator(internal_nth_element(n));
        }

        // Returneaza iterator catre cel mai mare nod cu cheia <= cheia data
        iterator lower_bound(Key k) {
            return iterator(internal_lower_bound(k));
        }

        // Reuturneaza iterator catre cel mai mic nod cu cheia > cheia data
        iterator upper_bound(Key k) {
            return iterator(internal_upper_bound(k));
        }

        int size() {
            return get_subtree_nodes(this);
        }

        bool validate_values() {
            bool l = true;
            bool r = true;

            if (left)
                l &= left->validate_values();
            if (right)
                r &= right->validate_values();

            return l && r && (get_subtree_nodes(this) == 1 + get_subtree_nodes(left) + get_subtree_nodes(right)) &&
                             (get_subtree_value(this) == this->value + get_subtree_value(left) + get_subtree_value(right));
        }
};

#endif // TREAP_H
