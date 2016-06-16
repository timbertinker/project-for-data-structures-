#include <iostream>
#include "Forest.h"
#include "Treap.h"
#include "hashtable.h"
#include "Store.h"
#include "User.h"
#include "Vector.h"
#include "distance.h"

using namespace std;

template<typename T>
struct Array {
    int resultSize;
    T* resultData;

    Array(int resultSize, T* resultData) :
        resultSize(resultSize), resultData(resultData) {}

    ~Array() {
        delete[] resultData;
    }
};

class Service {
    private:
        // Un hashtable care retine pentru fiecare id de utilizator
        // un indice in vectorul de useri.
        hashtable<int, int> user_index;

        // Vector care retine toti utilizatorii.
        Vector<User> vector_users;

        // Un hashtable care retine pentru fiecare id de store
        // un indice in vectorul de store.
        hashtable<int, int> store_index;

        // Vector cu toate obiectele de tip Store
        Vector<Store> vector_stores;

        // Treap care contine pentru un moment de timp numarul de utilizatori in vizita
        Treap<int, int>* treap_task1;

        // Treap care contine pentru un moment de timp discountul total
        Treap<int, int>* treap_task2;

        // Comparator pentru treap_task3
        struct task3_cmp {
            inline bool operator()(const pair<pair<int, int>, int>& a,
                                   const pair<pair<int, int>, int>& b) {
                long long diff = 1LL * a.first.first * b.first.second - 1LL * a.first.second * b.first.first;
                if (diff == 0)
                    return a.second < b.second;
                return diff > 0;
            }
        };

        // Treap care contine chei de tipul <<int, int>, int> = <raport = <numarator, numitor>, id_utilizator>
        Treap<pair<pair<int, int>, int>, bool, task3_cmp>* treap_task3;

        // Padure de arbori pentru taskurile 08-12
        Forest forest;

    public:
        Service() {
            treap_task1 = new Treap<int, int>();
            treap_task2 = new Treap<int, int>();
            treap_task3 = new Treap<pair<pair<int, int>, int>, bool, task3_cmp>();
        }

        ~Service() {
            delete treap_task1;
            delete treap_task2;
            delete treap_task3;
        }

        void createUser(int id, double homeX, double homeY) {
            User u = User(id, homeX, homeY);
            vector_users.push_back(u);
            user_index[id] = vector_users.size() - 1;
            forest.add(id);
        }

        void createStore(int id, double storeX, double storeY) {
            Store s = Store(id, storeX, storeY);
            vector_stores.push_back(s);
            store_index[id] = vector_stores.size() - 1;
        }

        // discount = -1 daca userul nu a cumparat nimic si, astfel nu s-a acordat discount
        void visit(int timestamp, int clientId, int storeId, int discount) {
            User& u = vector_users[user_index[clientId]]; // REFERINTA, JMEN
            u.vizite++;
            forest.add_visit(user_index[clientId]);

            if (discount != -1) {
                if (u.vizite_cu_discount > 0) {
                    treap_task3 = treap_task3->erase(make_pair(make_pair(u.vizite_cu_discount, u.discount), clientId));
                }

                u.discount += discount;
                u.vizite_cu_discount++;
                treap_task2 = treap_task2->insert(timestamp, discount);
                treap_task3 = treap_task3->insert(make_pair(make_pair(u.vizite_cu_discount, u.discount), clientId), 1);
            }

            // incrementam numarul de vizite la timpul timestamp cu 1
            treap_task1 = treap_task1->insert(timestamp, 1);

            // se updateaza structurile pentru store pentru taskurile 4-7
            int lower_day = timestamp / 86400;
            int lower_seconds = lower_day * 86400;
            int upper_seconds = (lower_day + 1) * 86400 - 1;

            Store& s = vector_stores[store_index[storeId]];

            s.treap_task4 = s.treap_task4->insert(timestamp, 1);

            int visits_per_day = s.treap_sec_task7->prefix_sum(upper_seconds) - s.treap_sec_task7->prefix_sum(lower_seconds - 1);
            auto it = s.treap_day_task7->find(make_pair(lower_day, visits_per_day));
            if (it != s.treap_day_task7->end())
                s.treap_day_task7 = s.treap_day_task7->erase(it);
            s.treap_sec_task7 = s.treap_sec_task7->insert(timestamp, 1);
            s.treap_day_task7 = s.treap_day_task7->insert(make_pair(lower_day, visits_per_day + 1), 1);

            double d = euclidean_distance(s.x, s.y, u.x, u.y);
            s.treap_task6 = s.treap_task6->insert(d, 1);

            // updatez distanta minima de la userul u la store-ul s
            if (u.dist_min_to_store > d)
                u.dist_min_to_store = d;

            if (discount != -1) {
                s.treap_task5 = s.treap_task5->insert(discount, 1);
            }
        }

        void invite(int userWhichInvites, int invitedUser) {
            int ux = user_index[userWhichInvites];
            int uy = user_index[invitedUser];

            forest.unite(ux, uy);
        }

        // numar de vizite in intervalul (startTime, endTime)
        int visitsInTimeframe(int startTime, int endTime) {
            // suma pe intervalul start, end, easy peasy lemon squeezy
            int ans = treap_task1->prefix_sum(endTime) -
                      treap_task1->prefix_sum(startTime - 1);
            return ans;
        }

        // discount total in intervalul (startTime, endTime)
        int totalDiscountInTimeframe(int startTime, int endTime) {
            int ans = treap_task2->prefix_sum(endTime) -
                      treap_task2->prefix_sum(startTime - 1);
            return ans;
        }

        // Returneaza un Array de k elemente cu id-urile user-ilor
        // Buy to discount rate se calculeaza in felul urmator
        // buy = numarul de vizite in care user-ul a cumparat ceva (discount > 0)
        // total = discount-ul total pe care utilizatorul la primit (excluzand vizitele cu discount = -1)
        // buyToDiscountRate = buy / total
        Array<int> usersWithBestBuyToDiscountRate(int K) {
            Treap<pair<pair<int, int>, int>, bool, task3_cmp>::iterator it;
            int i;
            Vector<int> ans;
            // parcurg treap-ul si extrag primii k useri cu raport maxim
            for (it = treap_task3->begin(), i = 0;
                 i < K && it != treap_task3->end();
                 i++, it++) {
                ans.push_back((*it).first.second);
            }

            // copiez rezultatul din vector<> intr-un array alocat dinamic pentru a-l
            // putea da ca parametru pentru functia Array<>()
            int *v = new int[ans.size()];
            for (int i = 0; i < ans.size(); i++) {
                v[i] = ans[i];
            }

            return Array<int>(ans.size(), v);
        }

        // numar de vizite in intervalul (startTime, endTime) pentru magazinul storeId
        int visitsInTimeframeOfStore(int startTime, int endTime, int storeId) {
            Store& s = vector_stores[store_index[storeId]];
            int ans = s.treap_task4->prefix_sum(endTime) -
                      s.treap_task4->prefix_sum(startTime - 1);
            return ans;
        }

        // cele mai mari discount-uri acordate in magazinul storeId
        Array<int> biggestKDiscounts(int K, int storeId) {
            Vector<int> ans;

            Store& s = vector_stores[store_index[storeId]];
            // iterez prin discounturi
            for (auto it = s.treap_task5->rbegin(); it != s.treap_task5->rend() && ans.size() < K; ++it) {
                // fiecare discount il pun de cate ori apare
                for (int i = 0; i < (*it).second && ans.size() < K; i++)
                    ans.push_back((*it).first);
            }

            int *v = new int[ans.size()];
            for (int i = 0; i < ans.size(); i++) {
                v[i] = ans[i];
            }

            return Array<int>(ans.size(), v);
        }

        // cele mai mari distante de la care vin clientii la magazinul storeId
        Array<double> biggestKClientDistances(int K, int storeId) {
            Vector<double> ans;
            Store& s = vector_stores[store_index[storeId]];
            // iterez prin distante, si pun o distanta o SINGURA data.
            for (auto it = s.treap_task6->rbegin(); it != s.treap_task6->rend() && ans.size() < K; ++it) {
                ans.push_back((*it).first);
            }

            double *v = new double[ans.size()];
            for (int i = 0; i < ans.size(); i++) {
                v[i] = ans[i];
            }

            Array<double> array(ans.size(), v);
            return array;
        }

        // Returneaza un array cu index-ul zilelor cele mai aglomerate
        // Index-ul unei zile este timestamp / (24 * 3600)
        // In cazul in care 2 zile au acelasi numar de vizite
        // in output va trebuie sa apara prima cea care are indicele mai mic
        Array<int> mostCrowdedKDays(int K, int storeId) {
            Store& s = vector_stores[store_index[storeId]];

            Vector<int> tmp;
            for (auto it = s.treap_day_task7->begin(); it != s.treap_day_task7->end() && tmp.size() < K; ++it) {
                tmp.push_back((*it).first.first);
            }

            int *result = new int[tmp.size()];
            for (int i = 0; i < tmp.size(); i++) {
                result[i] = tmp[i];
            }

            return Array<int>(tmp.size(), result);
        }

        // Returneaza un array cu dimensiunile grupurilor de utilizatori
        // Un grup de utilizatori se poate forma DOAR pe baza invitatiilor
        // Cu alte cuvinte, orice grup va avea dimensiunea minim 2
        Array<int> distinctGroupsOfUsers() {
            Vector<pair<int, int> > trees = forest.trees();
            Vector<int> ans;
            for (int i = 0; i < trees.size(); i++) {
                if (trees[i].first <= 1)
                    continue;
                ans.push_back(trees[i].first);
            }

            int *v = new int[ans.size()];
            for (int i = 0; i < ans.size(); i++) {
                v[i] = ans[i];
            }

            return Array<int>(ans.size(), v);
        }

        // Id-ul userului care a invitat cei mai multi utilizatori
        // Daca sunt doi cu aceeasi valoare, se va intoarce id-ul minim
        // Daca NU exista niciun utilizator adaugat si intalniti un astfel de query
        // returnati -1
        int userWithMostInvites() {
            return forest.node_max_sons();
        }

        // Lungimea celui mai lung lant de invitatii
        // Lungimea lantului se masoara ca numar de invitatii
        // A invita B => Lungime 1
        // A invita B, B invita C => Lungime 2
        int longestInvitesChainSize() {
            return forest.longest();
        }

        // Id-urile grupurilor cu cele mai multe invitatii overall
        // Id-ul unui grup se considera minimul id-urilor utilizatorilor din acel grup
        // Un grup de utilizatori se poate forma DOAR pe baza invitatiilor
        // Cu alte cuvinte, orice grup va avea dimensiunea minim 2
        Array<int> topKGroupsWithMostVisitsOverall(int K) {
            Vector<int> ans = forest.most_visited_groups(K);
            int *v = new int[ans.size()];
            for (int i = 0; i < ans.size(); i++) {
                v[i] = ans[i];
            }

            return Array<int>(ans.size(), v);
        }

        // O lista de perechi de forma (idGrup, numarMediuDeVizite)
        // Id-ul unui grup se considera minimul id-urilor utilizatorilor din acel grup
        // Un grup de utilizatori se poate forma DOAR pe baza invitatiilor
        // Cu alte cuvinte, orice grup va avea dimensiunea minim 2
        Array<pair<int, double>> averageVisitsPerUser() {

            Vector<pair<int, double> > ans = forest.average_visits();
            pair<int, double> *v = new pair<int, double>[ans.size()];
            for (int i = 0; i < ans.size(); i++) {
                v[i] = ans[i];
            }

            return Array<pair<int, double>>(ans.size(), v);
        }

        // Returneaza latitudine si longitudine pentru locatia recomandata pentru un nou magazin
        pair<double, double> newStoreCoordinates() {
            pair<double, double> coord_min(1.0 * 0x3f3f3f3f, 1.0 * 0x3f3f3f3f);
            pair<double, double> coord_max(-1.0 * 0x3f3f3f3f, -1.0 * 0x3f3f3f3f);

            if (vector_users.size() == 0)
                return pair<double, double>(0, 0);

            // gasesc bounding box-ul in care se incadreaza toti utilizatorii
            // aleg arbitrar un punct din acest domeniu - media tuturor punctelor.

            double x, y;
            x = y = 0;
            for (int i = 0; i < vector_users.size(); i++) {
                if (coord_max.first < vector_users[i].x)
                    coord_max.first = vector_users[i].x;

                if (coord_max.second < vector_users[i].y)
                    coord_max.second = vector_users[i].y;

                if (coord_min.first > vector_users[i].x)
                    coord_min.first = vector_users[i].x;

                if (coord_min.second > vector_users[i].y)
                    coord_min.second = vector_users[i].y;

                x += vector_users[i].x;
                y += vector_users[i].y;
            }

            x /= vector_users.size();
            y /= vector_users.size();

            // Facem 50 de iteratii pentru a gasi solutia
            const int ITERATIONS = 50;
            // vector de directie in sus/jos/stanga/dreapta
            const int dirx[] = { -1, 1, 0, 0 };
            const int diry[] = { 0, 0, -1, 1 };

            double dx, dy;

            pair<double, double> ans;

            dx = (coord_max.first - coord_min.first)/2;
            dy = (coord_max.second - coord_min.second)/2;

            int beneficiari_max = -1, dir = 0;
            // La fiecare pas, impart distanta pe x si pe y la 2
            for (int iter = 0; iter < ITERATIONS; ) {
                // Daca gasesc o directie mai buna decat solutia curenta, tin minte
                bool found = false;

                // pentru fiecare directie, calculez numaru de beneficiari daca m-as muta in acea directie
                // cu dx pe OX sau dy pe OY.
                for (int k = 0; k < 4; k++) {
                    int beneficiari = 0;
                    for (int i = 0; i < vector_users.size(); i++) {
                        double dist = euclidean_distance(vector_users[i].x, vector_users[i].y,
                                                          x + dx * dirx[k], y + dy * diry[k]);

                        if (dist < vector_users[i].dist_min_to_store)
                            beneficiari++;
                    }

                    // updatez maximul, retin directia
                    if (beneficiari > beneficiari_max) {
                        found = true;
                        dir = k;
                        beneficiari_max = beneficiari;
                        ans = pair<double, double>(x + dx * dirx[k], y + dy * diry[k]);
                    }
                }


                // Daca am gasit solutie mai buna, ma misc si pastrez pasul pe OX si pe OY si directia,
                // altfel scad numarul de iteratii si impart la 2.
                if (found) {
                    x += dirx[dir] * dx;
                    y += diry[dir] * dy;
                } else {
                    iter++;
                    dx /= 2;
                    dy /= 2;
                }
            }

            return ans;
        }
};
