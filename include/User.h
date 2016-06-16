#ifndef USER_H
#define USER_H

#include <iostream>

using std::pair;

struct User {
    int id;
    double x;
    double y;
    int vizite;
    int vizite_cu_discount;
    int discount;
    double dist_min_to_store;

    User() {
    }

    User(int id_, double x_, double y_) {
        id = id_;
        x = x_;
        y = y_;
        vizite_cu_discount = vizite = 0;
        discount = 0;
        dist_min_to_store = 1.0 * 0x3f3f3f3f;
    }

    User(const User& other) {
        id = other.id;
        x = other.x;
        y = other.y;
        vizite = other.vizite;
        vizite_cu_discount = other.vizite_cu_discount;
        discount = other.discount;
        dist_min_to_store = other.dist_min_to_store;
    }

    User& operator=(const User& other) {
        id = other.id;
        x = other.x;
        y = other.y;
        vizite = other.vizite;
        vizite_cu_discount = other.vizite_cu_discount;
        discount = other.discount;
        dist_min_to_store = other.dist_min_to_store;

        return *this;
    }
};

#endif // USER_H
