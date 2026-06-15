#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

/**
 * Problem Analysis:
 * - We have N resources and M users.
 * - Each user i has a set of resources they need, denoted by Si.
 * - User groups are formed by sets of users (Ug) and sets of resources (Rg).
 * - Rules:
 *   1. Each resource can belong to at most one user group.
 *   2. A user has access to a resource if they belong to a user group that contains it.
 *   3. Each user must have access to exactly the resources in their Si.
 *   4. A user can belong to multiple user groups.
 * 
 * Let R1, R2, ..., Rk be the resource sets for the k user groups.
 * Rule 1 implies these R_g sets are disjoint.
 * Rule 2 and 3 imply that for any user i, the set of resources they can access, 
 * which is the union of Rg for all groups g where i belongs to Ug, must be exactly Si.
 * 
 * This means for each resource r, let U(r) be the set of users who need it. 
 * If r is in group g, then all users in group g must have r in their Si, 
 * and all users in U(r) must be in group g.
 * Therefore, for every resource r in a group g, U(r) must be equal to the set of users in group g (Ug).
 * 
 * Consequently, all resources in the same group must be needed by the exact same set of users.
 * The minimal number of groups needed is therefore the number of distinct, non-empty sets U(r) 
 * for all r from 1 to N.
 */

int main() {
    // Optimize input and output speed
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    // Read N (number of resources) and M (number of users)
    if (!(cin >> N >> M)) return 0;

    // resource_users[r] will store the list of users who require resource r.
    // We'll use 1-based indexing for both resources and users.
    vector<vector<int>> resource_users(N + 1);

    for (int i = 1; i <= M; ++i) {
        int size;
        if (!(cin >> size)) break;
        for (int j = 0; j < size; ++j) {
            int r;
            if (!(cin >> r)) break;
            // According to problem, r is between 1 and N.
            if (r >= 1 && r <= N) {
                // Since we iterate through users i from 1 to M,
                // resource_users[r] will automatically be sorted by user ID.
                resource_users[r].push_back(i);
            }
        }
    }

    // We need to count how many unique, non-empty sets of users exist across all resources.
    set<vector<int>> distinct_user_sets;
    for (int r = 1; r <= N; ++r) {
        // If a resource is needed by one or more users, we add its user set to our collection.
        if (!resource_users[r].empty()) {
            distinct_user_sets.insert(resource_users[r]);
        }
    }

    // The result is the number of distinct user sets identified.
    cout << distinct_user_sets.size() << endl;

    return 0;
}

