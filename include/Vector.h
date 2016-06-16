#ifndef Vector_H
#define Vector_H

#include <cassert>
#include <cstddef>
#include <iostream>

using std::pair;

// Mod de utilizare:
// 
//  Declara un Vector.
//  Vector<int> v;
// 
//  v.push_back(x) insereaza un element la sfarsitul lui v.
//  v.push_front(x) insereaza un element la inceputul lui v. Complexitatea in timp este proportionala cu numarul de elemente din v. So... do not use it unless you have a really good reason.
//  v.pop_back() sterge un element de la sfarsitul lui v.
//  v.pop_front() sterge un element de la inceputul lui v. Aceeasi complexitate ca push_front.
//  v.clear() sterge toate elementele din vector Vector.
//  v.size() intoarce numarul de elemente din v.
//  v.remove_all(x) sterge toate elementele egale cu x.
//  v.remove_from_index(x) sterge elementul de la indexul x.
//  v.remove_element(x) sterge prima aparitie a elementului x.
// 
//  v.front() intoarce o referinta la primul element.
//  v.back() intoarce o referinta la ultimul element.
// 
//  Itereaza prin toate elementele dintr-un Vector:
//   for (int i = 0; i < v.size(); i++) {
//       int element = v[i];
//     // Do stuff with v[i]
//   }


template <typename T>
class Vector {
    private:

        std::size_t size_;
        std::size_t capacity;
        T *vect;

    public:

        Vector(int capacity = 0) {
            size_ = 0;
            this->capacity = capacity;
            if (capacity == 0)
                vect = NULL;
            else
                vect = new T[capacity];
        }

        Vector(const Vector& other) {
            size_ = other.size_;
            capacity = other.capacity;
            if (capacity == 0)
                vect = NULL;
            else
                vect = new T[capacity];

            for (std::size_t i = 0; i < size_; i++)
                vect[i] = other.vect[i];
        }

        Vector& operator=(const Vector& other) {
            delete[] vect;

            size_ = other.size_;
            capacity = other.capacity;
            if (capacity == 0)
                vect = NULL;
            else
                vect = new T[capacity];

            for (std::size_t i = 0; i < size_; i++)
                vect[i] = other.vect[i];
            return *this;
        }

        // Adauga un element la sfarsitul lui Vector

        void push_back(T elem) {
            // Daca vectorul e plin dubleaza capacitatea
            if (size_ == capacity) {
                double_capacity();
            }

            vect[size_] = elem;
            size_++;
        }

        // Adauga un element la inceputul lui Vector

        void push_front(T elem) {
            // Daca vectorul e gol
            if (!size_) {
                vect[size_] = elem;
                return;
            }

            if (size_ + 1 == capacity) {
                double_capacity();
            }

            for (std::size_t i = size_ + 1; i >= 1; i--) {
                vect[i] = vect[i - 1];
            }

            size_++;
            vect[0] = elem;
        }

        void pop_back() {
            size_--;
            if (size_ < (capacity+1)/2)
                shrink();
        }

        void pop_front() {
            for (std::size_t i = 1; i < size_; i++) {
                vect[i - 1] = vect[i];
            }

            size_--;
            if (size_ < (capacity+1)/2)
                shrink();
        }

        // Returneaza primul element

        T& front() {
            assert(size_ != 0);
            return vect[0];
        }

        const T& front() const {
            assert(size_ != 0);
            return vect[0];
        }

        // Returneaza ultimul element

        T& back() {
            assert(size_ != 0);
            return vect[size_ - 1];
        }

        const T& back() const {
            assert(size_ != 0);
            return vect[size_ - 1];
        }

        T& operator[](int idx) {
            return vect[idx];
        }

        const T& operator[](int idx) const {
            return vect[idx];
        }

        void remove_from_index(int index) {
            if (index < 0)
                return;

            for (std::size_t j = index; j + 1 < size_; j++) {
                vect[j] = vect[j + 1];
            }
            size_--;

            if (size_ < (capacity+1)/2)
                shrink();
        }

        // Sterge toate elementele egale cu elem

        void remove_all(T elem) {
            for (std::size_t i = 0; i < size_; i++) {
                if (vect[i] == elem) {
                    remove_from_index(i);
                    i--;
                }
            }
        }

        void remove_element(T elem) {
            for (std::size_t i = 0; i < size_; i++) {
                if (vect[i] == elem) {
                    remove_from_index(i);
                    break;
                }
            }
        }

        void sort() {
            mergesort_(0, size() - 1);
        }

        size_t size() const {
            return size_;
        }


        void clear() {
            while (size()) pop_back();
        }

        ~Vector() {
            delete[] vect;
        }

    private:

        void shrink() {
            T *tmp = new T[(capacity+1)/2];

            for (size_t i = 0; i < size_; i++) {
                tmp[i] = vect[i];
            }

            capacity = (capacity+1)/2;
            delete[] vect;
            vect = tmp;
        }

        void double_capacity() {
            if (capacity == 0) {
                capacity = 1;
                vect = new T[capacity];
            }

            T *tmp = new T[2 * capacity];
            for (size_t i = 0; i < size_; i++) {
                tmp[i] = vect[i];
            }

            delete[] vect;
            vect = tmp;
            capacity *= 2;
        }

        void mergesort_(int left, int right) {
            if (left == right) {
                return;
            }

            int mid = left + (right - left) / 2;

            mergesort_(left, mid);
            mergesort_(mid + 1, right);

            combine_(left, mid, right);
        }

        void combine_(int left, int mid, int right) {
            // TODO: in-place mergesort maybe?
            Vector<T> w;

            for (int i = left, j = mid + 1; i <= mid || j <= right; ) {
                if (i == mid + 1) {
                    w.push_back(vect[j]);
                    j++;
                } else if (j == right + 1) {
                    w.push_back(vect[i]);
                    i++;
                } else if (!(vect[j] < vect[i])) {
                    w.push_back(vect[i]);
                    i++;
                } else {
                    w.push_back(vect[j]);
                    j++;
                }
            }

            for (int i = left; i <= right; i++) {
                vect[i] = w[i - left];
            }
        }
};

#endif // Vector_H
