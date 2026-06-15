#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

/**
 * Problem Analysis:
 * We want to maximize the total weight of students who qualify for their assigned zones.
 * A student (level P, weight w) qualifies for a zone (level Q) if P > Q.
 * This is a matroid problem (specifically, a transversal matroid), and a greedy strategy
 * by weight should work.
 * 
 * Greedy Strategy:
 * 1. Sort students by weight in descending order.
 * 2. For each student (in descending weight order):
 *    a. Find the zone with the largest level Q such that Q < P and the zone has remaining capacity.
 *    b. If such a zone exists, assign the student to it to maximize the chance of
 *       other students (with lower weights but potentially different P levels) qualifying.
 *    c. If no such zone exists, the student cannot qualify for any remaining zone. 
 *       To preserve the "easiest" zones (those with smaller Q) for future students,
 *       assign this student to the zone with the largest Q available.
 * 
 * This approach is O(N log N + N log K), which fits within the 0.25s time limit.
 */

struct Student {
    int p, w, id;
};

// Sort students by weight descending. If weights are equal, sort by level descending.
bool compareStudents(const Student& a, const Student& b) {
    if (a.w != b.w) {
        return a.w > b.w;
    }
    return a.p > b.p;
}

int main() {
    // Speed up I/O for strict time limits.
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int K;
    if (!(cin >> K)) return 0;

    // Read zone capacities.
    vector<int> N_val(K);
    int N_total = 0;
    for (int i = 0; i < K; ++i) {
        cin >> N_val[i];
        N_total += N_val[i];
    }

    // Read zone levels.
    vector<int> Q_val(K);
    for (int i = 0; i < K; ++i) {
        cin >> Q_val[i];
    }

    // Read student levels and store original indices.
    vector<Student> students(N_total);
    for (int i = 0; i < N_total; ++i) {
        cin >> students[i].p;
        students[i].id = i;
    }

    // Read student weights.
    for (int i = 0; i < N_total; ++i) {
        cin >> students[i].w;
    }

    // Sort students based on the greedy criteria.
    sort(students.begin(), students.end(), compareStudents);

    // Maintain a set of available zones. Each element is a pair {Q level, zone index}.
    // Using a set allows for efficient lookup via lower_bound.
    set<pair<int, int>> active_zones;
    vector<int> N_rem = N_val;
    for (int i = 0; i < K; ++i) {
        if (N_rem[i] > 0) {
            active_zones.insert({Q_val[i], i});
        }
    }

    // Store the zone assignment for each student.
    vector<int> assignment(N_total);
    for (int i = 0; i < N_total; ++i) {
        // Should not occur because sum N[i] = N.
        if (active_zones.empty()) break;

        // Find the first zone where Q >= P.
        auto it = active_zones.lower_bound({students[i].p, -1});

        if (it == active_zones.begin()) {
            // No available zone has Q < P.
            // Assign to the most difficult available zone (largest Q).
            it = prev(active_zones.end());
        } else {
            // There exists at least one zone with Q < P.
            // Pick the one with the largest Q < P.
            it = prev(it);
        }

        int zone_id = it->second;
        assignment[students[i].id] = zone_id + 1; // Store 1-based index
        N_rem[zone_id]--;

        // Remove the zone from the set if its capacity is exhausted.
        if (N_rem[zone_id] == 0) {
            active_zones.erase(it);
        }
    }

    // Output the zone assigned to each student.
    for (int i = 0; i < N_total; ++i) {
        cout << assignment[i] << (i == N_total - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

