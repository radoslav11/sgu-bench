#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

struct Zone {
    int Q;
    int cap;
    int orig_idx;
    Zone(int q, int c, int idx) : Q(q), cap(c), orig_idx(idx) {}
    bool operator<(const Zone& other) const {
        return Q > other.Q;
    }
};

struct Student {
    int P;
    int w;
    int idx;
    Student(int p, int w, int i) : P(p), w(w), idx(i) {}
    bool operator<(const Student& other) const {
        return P > other.P;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int K;
    cin >> K;

    vector<int> N_arr(K);
    for (int i = 0; i < K; i++) {
        cin >> N_arr[i];
    }

    vector<int> Q_arr(K);
    for (int i = 0; i < K; i++) {
        cin >> Q_arr[i];
    }

    int total_students = 0;
    for (int x : N_arr) {
        total_students += x;
    }

    vector<int> P_arr(total_students);
    for (int i = 0; i < total_students; i++) {
        cin >> P_arr[i];
    }

    vector<int> w_arr(total_students);
    for (int i = 0; i < total_students; i++) {
        cin >> w_arr[i];
    }

    // If there are no students, output nothing.
    if (total_students == 0) {
        return 0;
    }

    vector<Zone> zones;
    for (int i = 0; i < K; i++) {
        zones.push_back(Zone(Q_arr[i], N_arr[i], i+1));
    }
    sort(zones.begin(), zones.end());

    vector<Student> students;
    for (int i = 0; i < total_students; i++) {
        students.push_back(Student(P_arr[i], w_arr[i], i));
    }
    sort(students.begin(), students.end());

    vector<int> assignment(total_students, -1);
    priority_queue<pair<int, int>> heap; // (weight, student_index)

    vector<int> zone_remaining(K);
    for (int i = 0; i < K; i++) {
        zone_remaining[i] = zones[i].cap;
    }

    int j = 0;
    for (int i = 0; i < K; i++) {
        // Add all students with P > current zone's Q
        while (j < total_students && students[j].P > zones[i].Q) {
            heap.push({students[j].w, students[j].idx});
            j++;
        }
        int take = min(zone_remaining[i], (int)heap.size());
        for (int t = 0; t < take; t++) {
            auto top = heap.top();
            heap.pop();
            assignment[top.second] = zones[i].orig_idx;
        }
        zone_remaining[i] -= take;
    }

    // Collect unassigned students
    vector<int> unassigned;
    for (int i = 0; i < total_students; i++) {
        if (assignment[i] == -1) {
            unassigned.push_back(i);
        }
    }

    // Collect zones that need more students
    vector<pair<int, int>> needed_zones;
    for (int i = 0; i < K; i++) {
        if (zone_remaining[i] > 0) {
            needed_zones.push_back({zones[i].orig_idx, zone_remaining[i]});
        }
    }

    // Assign unassigned students to zones that have remaining capacity
    int idx_unassigned = 0;
    for (auto& zone_data : needed_zones) {
        int zone_idx = zone_data.first;
        int cap = zone_data.second;
        for (int t = 0; t < cap; t++) {
            assignment[unassigned[idx_unassigned]] = zone_idx;
            idx_unassigned++;
        }
    }

    // Output
    for (int i = 0; i < total_students; i++) {
        if (i > 0) {
            cout << " ";
        }
        cout << assignment[i];
    }
    cout << '\n';

    return 0;
}
