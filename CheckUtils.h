//
// Created by boaca on 25/04/16.
//

#ifndef SDCHECKER_CHECKUTILS_H
#define SDCHECKER_CHECKUTILS_H

#include "API.h"
#include <vector>
#include <set>
#include "IOUtils.h"
#include <functional>
#include <utility>
#include <chrono>
#include <cmath>
using namespace std;

extern int line;

template<class T>
vector<T> toVector(const Array<T>& array) {
    return vector<T>(array.resultData, array.resultData + array.resultSize);
}

template<class T>
bool equal(const T& expected, const T& actual,
        const string& expectedLabel = "expected",
        const string& actualLabel = "actual"
        ) {
    if (expected != actual) {
        cerr << "-------" << endl;
        cerr << "Query " << line << " failed:" << endl;
        cerr << expectedLabel << " " << expected << endl;
        cerr << actualLabel << " " << actual << endl;
        cerr << endl;
        return false;
    }
    return true;
}

template<class T, class Predicate = std::function<bool(const T&, const T&)>>
bool orderedSame(const vector<T> &v1, const vector<T> &v2,
        Predicate predicate) {

    if (v1.size() != v2.size()) {
        cerr << "-------" << endl;
        cerr << "Query " << line << " failed:" << endl;
        cerr << "expected size = " << v1.size() << endl;
        cerr << "actual size = " << v2.size() << endl;
        cerr << endl;
        return false;
    }

    unsigned long size = v1.size();

    bool result = true;
    for (int i = 0; i < size; ++i) {
        if (!predicate(v1[i], v2[i])) {
            result = false;
        }
    }

    if (!result) {
        cerr << "-------" << endl;
        cerr << "Query " << line << " failed:" << endl;
        for (int i = 0; i < size; ++i) {
            cerr << "expected = " << v1[i] << endl;
            cerr << "actual  " << v2[i] << endl;
        }
    }
    return result;
};

template<class T>
bool orderedSame(const vector<T> &v1, const vector<T> &v2) {
    return orderedSame(v1, v2, [](const T& t1, const T& t2) -> bool {
                return t1 == t2;
            });
};


template<class T>
bool unorderedSame(const vector<T>& v1, const vector<T>& v2) {
    multiset<T> v1set(v1.begin(), v1.end());
    multiset<T> v2set(v2.begin(), v2.end());
    return equal(v1set, v2set);
}

struct running_time {
    template<class Fn, class... Args>
    static tuple<long long, bool> measure(Fn fn, Args&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        bool result = fn(std::forward<Args>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return make_tuple(duration.count(), result);
    }
};

#endif //SDCHECKER_CHECKUTILS_H
