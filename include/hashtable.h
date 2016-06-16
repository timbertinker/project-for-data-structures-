#ifndef HASHTABLE_H
#define HASHTABLE_H

 // Utilizare:
 //
 // Sa presupunem ca vrem sa mapam int-uri la string-uri.
 //
 // Declararea hashtable-ului:
 //      hashtable<int, std::string> h;
 // Verificarea existentei unui element in hashtable:
 //      h.count(125); // returneaza 0 daca nu exista, 1 daca exista.
 // Stergerea unui element din hashtable:
 //      h.erase(125); // returneaza true daca se efectueaza cu succes, false in caz contrar.
 // Mapam int-ul 100 la string-ul "asdf":
 //      h[100] = "asdf";
 // Obtinerea valorii lui h[100]:
 //      std::string value = h[100];
 //      daca int-ul 100 nu exista in hashtable, va fi inserat datorita constructorului default de la std::string.
 //      Atentie daca vreti sa folositi hashtable-ul cu tipuri de date custom, au nevoie de un constructor default definit special.
 // Stergerea tuturor elementelor din hashtable:
 //      h.clear();
 // Iterarea prin elementele din hashtable:
 //      Daca hashtable-ul este const&, folositi const_iterator.
 //      const hashtable<int, std::string>& hr = h;
 //      for (hashtable<int, std::string>::const_iterator it = hr.begin(); it != hr.end(); ++it) {
 //          it este un const pair<int, std::string>*
 //          it->first este un int
 //          it->second este un std::string
 //      }
 //      Altfel, folositi iterator.
 //      hashtable<pair<int, int>, int> hpii;
 //      for (hashtable<int, std::string>::iterator it = h.begin(); it != h.end(); ++it) {
 //          it este un pair<int, std::string>*
 //          it->first este un int
 //          it->second este un std::string
 //      }
 //

#include <string>
#include <cassert>
#include <iostream>

using std::pair;
using std::make_pair;

#include "Vector.h"

#define NUMBER_OF_BUCKETS 997

template<typename T>
struct default_hash_policy {
    int operator()(const T& key);
};

template<typename Key, typename Value>
class hashtable {
    private:
        typedef pair<Key, Value> node;

        template<typename _Key, typename _Value, typename _Hashtable, typename _Pair>
        class Iterator {
            private:
                _Hashtable *ht_;
                size_t bucket_;
                size_t position_;

                friend class hashtable<_Key, _Value>;

                Iterator(_Hashtable *ht, int bucket, int position)
                    : ht_(ht), bucket_(bucket), position_(position)  {
                }

                bool equals(const Iterator& other) {
                    return position_ == other.position_ && bucket_ == other.bucket_;
                }

            public:

                Iterator(const Iterator& other)
                    : ht_(other.ht_), bucket_(other.bucket_), position_(other.position_) {
                }

                ~Iterator() {
                }

                void operator++() {
                    position_++;
                    while (position_ == ht_->h[bucket_].size() && bucket_ < ht_->buckets()) {
                        bucket_++;
                        position_ = 0;
                    }
                }

                bool operator==(const Iterator& other) {
                    return equals(other);
                }

                bool operator!=(const Iterator& other) {
                    return !equals(other);
                }

                _Pair* operator->() const {
                    return &ht_->h[bucket_][position_];
                }
        };

        Vector<node> h[NUMBER_OF_BUCKETS];

        size_t buckets() const {
            return NUMBER_OF_BUCKETS;
        }

        Value& value_find(const Key& key) {
            int hash = default_hash_policy<Key>()(key);

            for (size_t i = 0; i < h[hash].size(); i++) {
                if (h[hash][i].first == key)
                    return h[hash][i].second;
            }

            assert(false); // n-ar trebui sa se ajunga aici.
        }

        void insert_default(const Key& key) {
            int hash = default_hash_policy<Key>()(key);

            h[hash].push_back(node(key, Value()));
        }

    public:
        typedef Iterator<Key, Value, hashtable<Key, Value>, pair<Key, Value> > iterator;
        typedef Iterator<Key, Value, const hashtable<Key, Value>, const pair<Key, Value> > const_iterator;

        hashtable() {

        }

        ~hashtable() {
            for (size_t i = 0; i < buckets(); i++) {
                h[i].clear();
            }
        }

        hashtable(const hashtable<Key, Value>& other) {
            for (size_t i = 0; i < buckets(); i++) {
                h[i] = other.h[i];
            }
        }

        hashtable& operator=(const hashtable<Key, Value>& other) {
            for (size_t i = 0; i < buckets(); i++) {
                h[i] = other.h[i];
            }

            return *this;
        }

        Value& operator[](const Key& key) {
            if (!count(key))
                insert_default(key);

            return value_find(key);
        }

        const Value& operator[](const Key& key) const {
            if (!count(key))
                insert_default(key);

            return value_find(key);
        }

        int count(const Key& key) const {
            int hash = default_hash_policy<Key>()(key);

            for (size_t i = 0; i < h[hash].size(); i++) {
                if (h[hash][i].first == key)
                    return 1;
            }

            return 0;
        }

        bool erase(const Key& key) {
            if (!count(key))
                return false;

            int hash = default_hash_policy<Key>()(key);

            for (size_t i = 0; i < h[hash].size(); i++) {
                if (h[hash][i].first == key) {
                    h[hash].remove_from_index(i);
                    return true;
                }
            }

            return false;
        }

        void clear() {
            for (size_t i = 0; i < buckets(); i++)
                h[i].clear();
        }

        iterator begin() {
            size_t first_bucket = 0;
            while (h[first_bucket].size() == 0 && first_bucket < buckets())
                first_bucket++;
            return iterator(this, first_bucket, 0);
        }

        iterator end() {
            return iterator(this, buckets(), 0);
        }

        const_iterator begin() const {
            size_t first_bucket = 0;
            while (h[first_bucket].size() == 0 && first_bucket < buckets())
                first_bucket++;
            return const_iterator(this, first_bucket, 0);
        }

        const_iterator end() const {
            return const_iterator(this, buckets(), 0);
        }
};

// implementarea pentru default_hash_policy<int>
template<>
int default_hash_policy<int>::operator()(const int& key) {
    return key % NUMBER_OF_BUCKETS;
}

// implementarea pentru default_hash_policy<string> 
// Va folosi baza (26 + 26 + 10) pentru a hash-ui string-urile. (26 lowercase, 26 uppercase, 10 cifre)
template<>
int default_hash_policy<std::string>::operator()(const std::string& key) {
    int hash = 0, base = 62;
    for (size_t i = 0; i < key.size(); i++) {
        // valoarea lui char key[i] este in baza 62.
        // va mapa valorile urmatoare: '0' - '9' la 0 - 9
        //                             'a' - 'z' la 10 - 35
        //                             'A' - 'Z' la 36 - 61
        int char_in_base = 0;
        if (key[i] >= '0' && key[i] <= '9') char_in_base = key[i] - '0';
        if (key[i] >= 'a' && key[i] <= 'z') char_in_base = key[i] - 'a' + 10;
        if (key[i] >= 'A' && key[i] <= 'Z') char_in_base = key[i] - 'A' + 36;

        hash = (hash * base + char_in_base) % NUMBER_OF_BUCKETS;
    }

    return hash;
}

// implementarea pentru default_hash_policy<pair<string, string> >
// Va hash-ui string-ul rezultat prin concatenarea celor doua string-uri din pair<string, string>
template<>
int default_hash_policy<pair<std::string, std::string> >::operator()(const pair<std::string, std::string>& key) {
    std::string _key = key.first + " " + key.second;
    int hash = 0, base = 62;
    for (size_t i = 0; i < _key.size(); i++) {
        // valoarea lui char key[i] este in baza 62.
        // va mapa valorile urmatoare: '0' - '9' la 0 - 9
        //                             'a' - 'z' la 10 - 35
        //                             'A' - 'Z' la 36 - 61
        int char_in_base = 0;
        if (_key[i] >= '0' && _key[i] <= '9') char_in_base = _key[i] - '0';
        if (_key[i] >= 'a' && _key[i] <= 'z') char_in_base = _key[i] - 'a' + 10;
        if (_key[i] >= 'A' && _key[i] <= 'Z') char_in_base = _key[i] - 'A' + 36;

        hash = (hash * base + char_in_base) % NUMBER_OF_BUCKETS;
    }

    return hash;
}

// implementarea pentru default hash_policy pair<int, int> 
template<>
int default_hash_policy<pair<int, int> >::operator()(const pair<int, int>& key) {
    // se multiplica key.first cu o valoare prima constanta mai mica decat numarul de bucket-uri
    long long tmp = 1LL * key.first * 829 + key.second;

    tmp %= NUMBER_OF_BUCKETS; // // in C++, tinem cont ca operatia modulo pentru numere negative returneaza tot un numar negativ
    if (tmp < 0) tmp += NUMBER_OF_BUCKETS;
    
    return (int)tmp;
}


#endif /* HASHTABLE_H */
