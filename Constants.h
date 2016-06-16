//
// Created by boaca on 25/04/16.
//

#ifndef SDCHECKER_CONSTANTS_H
#define SDCHECKER_CONSTANTS_H

#include <unordered_map>
#include <string>
using namespace std;

enum OP_CODE {
    ADD_STORE = 1,
    ADD_USER = 2,
    REGISTER_INVITE = 3,
    REGISTER_VISIT = 4,
    VISITS_IN_TIMEFRAME = 5,
    TOTAL_DISCOUNT_IN_TIMEFRAME = 6,
    USERS_WITH_BEST_BUY_DISCOUNT_RATE = 7,
    VISITS_IN_TIMEFRAME_OF_STORE = 8,
    BIGGEST_K_DISCOUNTS = 9,
    BIGGEST_K_CLIENT_DISTANCES = 10,
    MOST_K_CROWDED_DAYS = 11,
    DISTINCT_GROUPS_OF_USERS = 12,
    USER_WITH_MOST_INVITES = 13,
    LONGEST_INVITE_CHAIN_SIZE = 14,
    TOP_K_GROUPS_WITH_MOST_OVERALL_VISTS = 15,
    AVERAGE_VISITS_PER_USER = 16,
    NEW_STORE_COORDINATES = 17
};

std::unordered_map<int, string> name {
        {ADD_USER, "addUser"},
        {ADD_STORE, "addStore"},
        {REGISTER_INVITE, "registerInvite"},
        {REGISTER_VISIT, "registerVisit"},
        {VISITS_IN_TIMEFRAME, "visitsInTimeframe"},
        {TOTAL_DISCOUNT_IN_TIMEFRAME, "totalDiscountInTimeframe"},
        {USERS_WITH_BEST_BUY_DISCOUNT_RATE, "usersWithBestBuyDiscountRate"},
        {VISITS_IN_TIMEFRAME_OF_STORE, "visitsInTimeFrameOfStore"},
        {BIGGEST_K_DISCOUNTS, "biggestKDiscounts"},
        {BIGGEST_K_CLIENT_DISTANCES, "biggestKClientDistances"},
        {MOST_K_CROWDED_DAYS, "mostCrowdedKDays"},
        {DISTINCT_GROUPS_OF_USERS, "distinctGroupOfUsers"},
        {USER_WITH_MOST_INVITES, "userWithMostInvites"},
        {LONGEST_INVITE_CHAIN_SIZE, "longestInviteChainSize"},
        {TOP_K_GROUPS_WITH_MOST_OVERALL_VISTS, "topKGroupsWithMostVisitsOverall"},
        {AVERAGE_VISITS_PER_USER, "averageVisitsPerUserForEachGroup"},
        {NEW_STORE_COORDINATES, "newStoreCoordinates"}
};

#endif //SDCHECKER_CONSTANTS_H
