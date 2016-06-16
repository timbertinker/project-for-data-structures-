#include <iostream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "CheckUtils.h"
#include "Constants.h"

using namespace std;

using checker_func = std::function<bool(Service&)>;

int line = 0;

#define endl "\n"

bool addStore(Service& service) {
    int id;
    double x, y;
    cin >> id >> x >> y;
    service.createStore(id, x, y);
    return true;
}

bool addUser(Service& service) {
    int id;
    double x, y;
    cin >> id >> x >> y;
    service.createUser(id, x, y);
    return true;
}

bool registerInvite(Service& service) {
    int invitor, invitee;
    cin >> invitor >> invitee;
    service.invite(invitor, invitee);
    return true;
}

bool registerVisit(Service& service) {
    int id, timestamp, visitor, shop, discount;
    cin >> id >> timestamp >> visitor >> shop >> discount;
    service.visit(timestamp, visitor, shop, discount);
    return true;
}

bool visitsInTimeframe(Service& service) {
    int start, end, reference;
    cin >> start >> end >> reference;

    int candidate = service.visitsInTimeframe(start, end);
    return equal(reference, candidate);
}

bool totalDiscountInTimeframe(Service& service) {
    int start, end, reference;
    cin >> start >> end >> reference;

    int candidate = service.totalDiscountInTimeframe(start, end);
    return equal(reference, candidate);
}

bool usersWithBestBuyDiscountRate(Service& service) {
    int K, size;
    cin >> K >> size;

    vector<int> reference(size);
    cin >> reference;

    vector<int> candidate = toVector(service.usersWithBestBuyToDiscountRate(K));
    return unorderedSame(reference, candidate);
}

bool visitsInTimeFrameOfStore(Service& service) {
    int start, end, store, reference;
    cin >> start >> end >> store >> reference;

    int candidate = service.visitsInTimeframeOfStore(start, end, store);
    return equal(reference, candidate);
}

bool biggestKDiscounts(Service& service) {
    int k, storeId, size;
    cin >> k >> storeId >> size;

    vector<int> reference(size);
    cin >> reference;

    vector<int> candidate = toVector(service.biggestKDiscounts(k, storeId));
    return orderedSame(reference, candidate);
}

bool biggestKClientDistances(Service& service) {
    int k, storeId, size;
    cin >> k >> storeId >> size;

    vector<double> reference(size);
    cin >> reference;

    vector<double> candidate = toVector(service.biggestKClientDistances(k, storeId));
    return orderedSame(reference, candidate, [](double a, double b) { return fabs(a - b) < 0.1; });
}

bool distinctGroupOfUsers(Service& service) {
    int size;
    cin >> size;

    vector<int> reference(size);
    cin >> reference;

    vector<int> candidate = toVector(service.distinctGroupsOfUsers());
    return unorderedSame(reference, candidate);
}

bool userWithMostInvites(Service& service) {
    int user;
    cin >> user;

    return equal(user, service.userWithMostInvites());
}

bool longestInviteChainSize(Service& service) {
    int reference;
    cin >> reference;

    return equal(reference, service.longestInvitesChainSize());
}

bool topKGroupsWithMostVisitsOverall(Service& service) {
    int k, size;
    cin >> k >> size;

    vector<int> reference(size);
    cin >> reference;

    return unorderedSame(reference, toVector(service.topKGroupsWithMostVisitsOverall(k)));
}

bool averageVisitsPerUserForEachGroup(Service& service) {
    int size;
    cin >> size;

    vector<pair<int, double>> reference(size);
    cin >> reference;

    vector<pair<int, double>> candidate = toVector(service.averageVisitsPerUser());

    // have to sort otherwise any checker is epsilon-error-prone
    auto comp = [](const pair<int, double> &p, const pair<int, double> &q) {
        return p.first < q.first;
    };

    sort(reference.begin(), reference.end(), comp);
    sort(candidate.begin(), candidate.end(), comp);

    return orderedSame(reference, candidate, [](const pair<int, double> &p1, const pair<int, double> &p2) {
        return p1.first == p2.first &&
               fabs(p1.second - p2.second) < 0.01;
    });
};

bool mostCrowdedKDays(Service& service) {
    int k, storeId, size;
    cin >> k >> storeId >> size;

    vector<int> reference(size);
    cin >> reference;

    return orderedSame(reference, toVector(service.mostCrowdedKDays(k, storeId)));
}

bool newStoreCoordinates(Service& service) {
    pair<double, double> coords = service.newStoreCoordinates();
    ofstream f("newStoreCoords.txt", std::ofstream::out | std::ofstream::app);
    f << coords.first << " " << coords.second << endl;
    f.close();

    return true;
}

std::unordered_map<int, checker_func> check {
        {ADD_USER, addUser},
        {ADD_STORE, addStore},
        {REGISTER_INVITE, registerInvite},
        {REGISTER_VISIT, registerVisit},
        {VISITS_IN_TIMEFRAME, visitsInTimeframe},
        {TOTAL_DISCOUNT_IN_TIMEFRAME, totalDiscountInTimeframe},
        {USERS_WITH_BEST_BUY_DISCOUNT_RATE, usersWithBestBuyDiscountRate},
        {VISITS_IN_TIMEFRAME_OF_STORE, visitsInTimeFrameOfStore},
        {BIGGEST_K_DISCOUNTS, biggestKDiscounts},
        {BIGGEST_K_CLIENT_DISTANCES, biggestKClientDistances},
        {MOST_K_CROWDED_DAYS, mostCrowdedKDays},
        {DISTINCT_GROUPS_OF_USERS, distinctGroupOfUsers},
        {USER_WITH_MOST_INVITES, userWithMostInvites},
        {LONGEST_INVITE_CHAIN_SIZE, longestInviteChainSize},
        {TOP_K_GROUPS_WITH_MOST_OVERALL_VISTS, topKGroupsWithMostVisitsOverall},
        {AVERAGE_VISITS_PER_USER, averageVisitsPerUserForEachGroup},
        {NEW_STORE_COORDINATES, newStoreCoordinates}
};

int main() {
    int T, opCode = 0;
    cin >> T;

    std::unordered_map<int, long long> durationPerOp;
    std::unordered_map<int, int> ops;
    std::unordered_map<int, int> ops_passed;

    long long totalDuration = 0;
    int passed = 0, failed = 0;

    Service service;

    ofstream f("newStoreCoords.txt"); f.close();

    while (T--) {
        cin >> opCode;
        ++line;

        long long duration;
        bool testPassed;

        if (!check.count(opCode)) {
            cout << "Invalid opCode. Malfored input!" << endl;
            return -1;
        }

        std::tie(duration, testPassed) = running_time::measure(check[opCode], ref(service));

        durationPerOp[opCode] += duration;
        ops[opCode]++;
        totalDuration += duration;

        if (testPassed) {
            passed++;
            ops_passed[opCode]++;
        } else {
            failed++;
        }
    }

    cout << "Passed: " << passed << endl;
    cout << "Failed: " << failed << endl;
    cout << "Average microseconds per instruction: " << static_cast<double>(totalDuration) / line << endl;
    cout << "Average microseconds per operation type: " << endl;

    for (const auto& p : ops) {
        int op = p.first;
        cout << "STATS FOR " << name[op] << ": " << ops_passed[op] << "/" << ops[op] << "\n";
    }
    for (const auto& p : durationPerOp) {
        int op = p.first;
        int count = ops[op];
        cout << "Average microseconds for " << name[op] << ": " << static_cast<double>(p.second) / count << endl;
    }

    return 0;
}
