#ifndef FOREST_H
#define FOREST_H

#include <iostream>

#include "hashtable.h"
#include "Treap.h"
#include "Vector.h"

class Forest {
    private:
        // retine toate radacinile
        Treap<int, bool> *roots;

        // vector ce tine pentru fiecare nod-radacina adancimea maxima
        Vector<int> max_depth;

        // vector ce tine pentru fiecare nod-radacina numarul de noduri din arbore
        Vector<int> num_nodes;

        // vector ce tine pentru fiecare nod-radacina numarul de vizite din arbore
        Vector<int> num_visits;

        // vector ce tine pentru fiecare nod-radacina id-ul minim al unui client asociat nodului
        Vector<int> min_id;

        // vector ce tine pentru fiecare nod numarul de fii
        Vector<int> num_sons;

        // vector ce tine pentru fiecare nod parintele
        Vector<int> parent;

        // vector ce tine pentru fiecare nod parintele, dar cu compresie de drumuri pentru optimizare
        Vector<int> set;

        // Vector ce tine id-ul utilizatorului pentru fiecare nod
        Vector<int> client;

        // cel mai lung lant per total
        int longest_chain;

        // nodul cu cel mai mare numar de fii
        int node_max;

        // numarul maxim de fii
        int max_sons;

        // numarul total de noduri in padurea de arbori
        int nodes;

        struct task11_cmp {
            bool operator()(const pair<int, int>& a,
                            const pair<int, int>& b) {
                if (a.first == b.first)
                    return a.second < b.second;
                return a.first > b.first;
            }
        };

        // treap care contine perechi <numar de vizite pe grup, id minim al grupului>
        Treap<pair<int, int>, bool, task11_cmp> *treap_task11;

        // cauta radacina unui nod x, facand simultan compresie de drumuri
        int find(int x) {
            if (set[x] == x)
                return x;
            else return set[x] = find(set[x]);
        }



    public:
        // constructor default
        Forest() {
            longest_chain = 0;
            max_sons = 0;
            nodes = 0;
            roots = new Treap<int, bool>();
            treap_task11 = new Treap<pair<int, int>, bool, task11_cmp>();
            node_max = -1;
        }

        // destructor
        ~Forest() {
            delete roots;
            delete treap_task11;
        }

        // copy constructor
        Forest(const Forest& other) :
            max_depth(other.max_depth),
            num_nodes(other.num_nodes), num_visits(other.num_visits),
            num_sons(other.num_sons), parent(other.parent), set(other.set),
            longest_chain(other.longest_chain), node_max(other.node_max), nodes(other.nodes),
            max_sons(other.max_sons), client(other.client) {
            roots = new Treap<int, bool>(*other.roots);
            treap_task11 = new Treap<pair<int, int>, bool, task11_cmp>(*other.treap_task11);
        }

        // copy assignment
        Forest& operator=(const Forest& other) {
            max_depth = other.max_depth;
            parent = other.parent;
            num_nodes = other.num_nodes;
            num_visits = other.num_visits;
            num_sons = other.num_sons;
            set = other.set;
            longest_chain = other.longest_chain;
            node_max = other.node_max;
            nodes = other.nodes;
            max_sons = other.max_sons;
            client = other.client;
            roots = new Treap<int, bool>(*other.roots);
            treap_task11 = new Treap<pair<int, int>, bool, task11_cmp>(*other.treap_task11);
            return *this;
        }

        // adauga vizita pe nodul x
        void add_visit(int x) {
            int root = find(x);
            if (num_nodes[root] > 1)
                treap_task11 = treap_task11->erase(make_pair(num_visits[root], min_id[root]));
            num_visits[root]++;
            if (num_nodes[root] > 1)
                treap_task11 = treap_task11->insert(make_pair(num_visits[root], min_id[root]), 0);
        }

        // adauga muchie de la nodul x la nodul y
        void unite(int x, int y) {
            int rx = find(x);
            int ry = find(y);

            roots = roots->erase(y);

            // scoatem grupurile reprezentate de rx si de ry
            if (num_nodes[rx] > 1) {
                treap_task11 = treap_task11->erase(make_pair(num_visits[rx], min_id[rx]));
            }
            if (num_nodes[ry] > 1) {
                treap_task11 = treap_task11->erase(make_pair(num_visits[ry], min_id[ry]));
            }

            set[ry] = rx;
            parent[y] = x;
            min_id[rx] = (min_id[rx] < min_id[ry]) ? min_id[rx] : min_id[ry];
            num_nodes[rx] += num_nodes[ry];
            num_visits[rx] += num_visits[ry];
            num_sons[x]++;

            // bagam grupul ce reprezinta reuninea dintre cele 2 grupuri anterioare
            treap_task11 = treap_task11->insert(make_pair(num_visits[rx], min_id[rx]), 0);

            // update nodul cu numar maxim de fii
            if (num_sons[x] > max_sons) {
                max_sons = num_sons[x];
                node_max = client[x];
            } else if (num_sons[x] == max_sons) {
                if (client[x] < node_max)
                    node_max = client[x];
            }

            // update max_depth
            int depth_to_root = 0;

            // Aici e O(H) worst case, idei de optimizare cineva?
            while (x != rx && x != -1) {
                depth_to_root++;
                x = parent[x];
            }

            if (depth_to_root + 1 + max_depth[ry] > max_depth[rx]) {
                max_depth[rx] = depth_to_root + 1 + max_depth[ry];
            }

            if (max_depth[rx] > longest_chain)
                longest_chain = max_depth[rx];
        }

        // adauga un nou nod in arbore separat
        void add(int id) {
            roots = roots->insert(nodes, 0);
            set.push_back(nodes);
            parent.push_back(-1);
            num_nodes.push_back(1);
            num_visits.push_back(0);
            num_sons.push_back(0);
            max_depth.push_back(0);
            client.push_back(id);
            min_id.push_back(id);

            if (max_sons == 0 && (node_max == -1 || id < node_max))
                node_max = id;

            nodes++;
        }

        // returneaza pair<numar noduri, numar vizite> pe componentele conexe cu mai mult de 1 nod
        Vector<pair<int, int> > trees() {
            Vector<pair<int, int> > ans;
            for (Treap<int, bool>::iterator it = roots->begin(); it != roots->end(); ++it) {
                int x = (*it).first;
                ans.push_back(make_pair(num_nodes[x], num_visits[x]));
            }

            return ans;
        }

        // returneaza client id-ul asociat nodului cu numarul maxim de fii
        int node_max_sons() {
            return node_max;
        }

        // returneaza lungimea lantului de lungime maxima
        int longest() {
            return longest_chain;
        }

        Vector<int> most_visited_groups(int K) {
            Vector<int> ans;

            for (Treap<pair<int, int>, bool, task11_cmp>::iterator it = treap_task11->begin();
                 it != treap_task11->end() && ans.size() < K; ++it) {
                ans.push_back((*it).first.second);
            }

            return ans;
        }

        Vector<pair<int, double> > average_visits() {
            Vector<pair<int, double> > ans;

            for (Treap<int, bool>::iterator it = roots->begin();
                 it != roots->end(); ++it) {
                int root = (*it).first;
                if (num_nodes[root] <= 1)
                    continue;
                ans.push_back(make_pair(min_id[root], 1. * num_visits[root] / num_nodes[root]));
            }

            return ans;
        }
};

#endif // FOREST_H
