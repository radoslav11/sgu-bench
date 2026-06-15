#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <algorithm>

using namespace std;

/**
 * Problem: 466. Parking at Secret Object
 * Objective: Implement a circular parking system management.
 * Key concepts: Circular clusters, minimum size >= S, minimum head index,
 *               allot S contiguous slots clockwise from head.
 * Complexity Requirements: N, Q <= 10^5, time limit 0.25s.
 * Data Structures: std::set for managing free clusters by head, end, and size.
 */

int N, Q;

// Cluster management using three sets for efficient lookup
struct ClusterManager {
    set<pair<int, int>> free_by_head; // {head, size}
    set<pair<int, int>> free_by_end;  // {end, head}
    set<pair<int, int>> free_by_size; // {size, head}

    void add_cluster(int h, int s) {
        if (s <= 0) return;
        free_by_head.insert({h, s});
        int e = (h + s - 1);
        if (e > N) e -= N;
        free_by_end.insert({e, h});
        free_by_size.insert({s, h});
    }

    void remove_cluster(int h, int s) {
        if (s <= 0) return;
        free_by_head.erase({h, s});
        int e = (h + s - 1);
        if (e > N) e -= N;
        free_by_end.erase({e, h});
        free_by_size.erase({s, h});
    }

    void park(int S, int q_idx, map<int, pair<int, int>>& query_to_range) {
        auto it = free_by_size.lower_bound({S, -1});
        if (it == free_by_size.end()) {
            cout << "NO ROOM" << "\n";
        } else {
            int s = it->first;
            int h = it->second;
            remove_cluster(h, s);
            if (s > S) {
                int new_h = (h + S - 1) % N + 1;
                int new_s = s - S;
                add_cluster(new_h, new_s);
            }
            query_to_range[q_idx] = {h, S};
            print_range(h, S);
        }
    }

    void leave(int h, int S, map<int, pair<int, int>>& query_to_range) {
        int next_slot = (h + S - 1) % N + 1;
        int prev_slot = (h - 2 + N) % N + 1;

        int ph = -1, ps = 0;
        auto it_end = free_by_end.lower_bound({prev_slot, 0});
        if (it_end != free_by_end.end() && it_end->first == prev_slot) {
            ph = it_end->second;
            auto it_h = free_by_head.lower_bound({ph, 0});
            ps = it_h->second;
        }

        int nh = -1, ns = 0;
        auto it_head = free_by_head.lower_bound({next_slot, 0});
        if (it_head != free_by_head.end() && it_head->first == next_slot) {
            nh = it_head->first;
            ns = it_head->second;
        }

        if (ph != -1 && nh != -1) {
            remove_cluster(ph, ps);
            remove_cluster(nh, ns);
            if (ps + S + ns == N) {
                add_cluster(1, N);
            } else {
                // Re-calculate head: if we merge ph and nh, ph is the head 
                // unless ph is part of a cluster that wraps around.
                // But ph is the head of the cluster ending at prev_slot.
                // Since we want the head of the combined cluster (clockwise),
                // if it's a single circular cluster of size < N, its head
                // is the smallest index in the contiguous block.
                // However, the merged cluster's head can be tricky.
                // A better way: the new cluster will start at 'ph' but if 'ph'
                // is not the smallest index, we need to handle that.
                // Wait, ph is the head, meaning it's the smallest index in its cluster.
                // But if the cluster wraps around, ph might be e.g., 8 in {8,9,10,1,2}.
                // The smallest index in {8,9,10,1,2} is 1. But in our set, 
                // head is the 'first parking slot in clockwise direction' 
                // starting from the block. Let's re-read.
                // "the number of the leftmost parking slot in the cluster (the first 
                // parking slot when you look over all parking slots of the cluster 
                // in clockwise direction)".
                // So if the cluster is {8, 9, 10, 1, 2}, its head is 8.
                // This makes our merge logic easier:
                // If we merge prev, current, and next, the new cluster's head
                // is the head of the 'prev' cluster if it doesn't wrap. 
                // Actually, it's just 'ph'.
                // Wait, if the new cluster wraps, its head is ph.
                // The only case is if the merged cluster becomes the entire N slots.
                add_cluster(ph, ps + S + ns);
            }
        } else if (ph != -1) {
            remove_cluster(ph, ps);
            add_cluster(ph, ps + S);
        } else if (nh != -1) {
            remove_cluster(nh, ns);
            add_cluster(h, S + ns);
        } else {
            add_cluster(h, S);
        }
    }

    void print_range(int h, int S) {
        vector<pair<int, int>> intervals;
        if (h + S - 1 <= N) {
            intervals.push_back({h, h + S - 1});
        } else {
            intervals.push_back({h, N});
            intervals.push_back({1, (h + S - 1) % N});
        }
        sort(intervals.begin(), intervals.end());
        for (int i = 0; i < (int)intervals.size(); ++i) {
            if (i > 0) cout << ",";
            if (intervals[i].first == intervals[i].second) {
                cout << intervals[i].first;
            } else {
                cout << intervals[i].first << "-" << intervals[i].second;
            }
        }
        cout << "\n";
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> N >> Q)) return 0;
    string s;
    cin >> s;

    ClusterManager cm;
    vector<int> x_pos;
    for (int i = 0; i < N; ++i) {
        if (s[i] == 'X') x_pos.push_back(i + 1);
    }

    if (x_pos.empty()) {
        cm.add_cluster(1, N);
    } else {
        for (int i = 0; i < (int)x_pos.size(); ++i) {
            int start = x_pos[i] + 1;
            int end = (i + 1 < (int)x_pos.size()) ? x_pos[i + 1] - 1 : x_pos[0] - 1;
            if (start <= end) {
                cm.add_cluster(start, end - start + 1);
            } else if (start > end) {
                if (start <= N) cm.add_cluster(start, N - start + 1);
                if (end >= 1) cm.add_cluster(1, end);
            }
        }
    }

    map<int, pair<int, int>> query_to_range;
    for (int i = 1; i <= Q; ++i) {
        string type;
        cin >> type;
        if (type == "PARK") {
            int Si;
            cin >> Si;
            cm.park(Si, i, query_to_range);
        } else {
            int Qi;
            cin >> Qi;
            auto range = query_to_range[Qi];
            cm.leave(range.first, range.second, query_to_range);
        }
    }

    return 0;
}

