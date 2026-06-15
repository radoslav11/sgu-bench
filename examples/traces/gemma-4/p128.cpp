#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

/**
 * Problem Summary:
 * We need to find a closed broken line using all N given points as vertices.
 * Each segment must be horizontal or vertical, and consecutive segments must be perpendicular.
 * The broken line must have no self-crossing or self-contact and minimal total length.
 *
 * Minimal length condition:
 * For each row and column, we pair adjacent points to minimize length.
 * For each row y, sort x-coordinates: x_1 < x_2 < ... < x_{2k}.
 * The horizontal segments are (x_1, x_2), (x_3, x_4), ..., (x_{2k-1}, x_{2k}).
 * This forms a collection of cycles. We must check if it's a single cycle of length N
 * and if it has any self-intersections or self-contacts.
 *
 * Self-intersection/contact check:
 * Since segments are minimal, horizontal segments within a row don't overlap.
 * Similarly for vertical segments. Intersection only between a horizontal segment (x1, x2) at y
 * and a vertical segment (y1, y2) at x if x1 < x < x2 and y1 < y < y2.
 * This can be checked using a sweep-line algorithm and a Fenwick tree.
 */

struct Point {
    int x, y;
};

struct Event {
    int x, type; // 1: start of horizontal, 2: vertical, 3: end of horizontal
    int y1, y2;
    bool operator<(const Event& other) const {
        if (x != other.x) return x < other.x;
        return type > other.type; // Order at same x: 3 (end), 2 (vertical), 1 (start)
    }
};

const int MAX_N = 10005;
int bit[MAX_N];
int max_y_count;

void update(int idx, int val) {
    for (; idx <= max_y_count; idx += idx & -idx)
        bit[idx] += val;
}

int query(int idx) {
    int sum = 0;
    for (; idx > 0; idx -= idx & -idx)
        sum += bit[idx];
    return sum;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;
    vector<Point> pts(N);
    map<int, vector<int>> col_map, row_map;
    for (int i = 0; i < N; ++i) {
        cin >> pts[i].x >> pts[i].y;
        col_map[pts[i].x].push_back(pts[i].y);
        row_map[pts[i].y].push_back(pts[i].x);
    }

    if (N % 2 != 0) {
        cout << 0 << endl;
        return 0;
    }

    for (auto& it : col_map) {
        if (it.second.size() % 2 != 0) {
            cout << 0 << endl;
            return 0;
        }
        sort(it.second.begin(), it.second.end());
    }
    for (auto& it : row_map) {
        if (it.second.size() % 2 != 0) {
            cout << 0 << endl;
            return 0;
        }
        sort(it.second.begin(), it.second.end());
    }

    map<pair<int, int>, vector<pair<int, int>>> adj;
    long long total_length = 0;

    for (auto const& [x, ys] : col_map) {
        for (size_t i = 0; i < ys.size(); i += 2) {
            int y1 = ys[i], y2 = ys[i+1];
            adj[{x, y1}].push_back({x, y2});
            adj[{x, y2}].push_back({x, y1});
            total_length += (y2 - y1);
        }
    }
    for (auto const& [y, xs] : row_map) {
        for (size_t i = 0; i < xs.size(); i += 2) {
            int x1 = xs[i], x2 = xs[i+1];
            adj[{x1, y}].push_back({x2, y});
            adj[{x2, y}].push_back({x1, y});
            total_length += (x2 - x1);
        }
    }

    // Check for single cycle that visits all N points
    int visited_count = 0;
    if (adj.empty()) {
        cout << 0 << endl;
        return 0;
    }
    map<pair<int, int>, bool> visited;
    pair<int, int> start = adj.begin()->first;
    pair<int, int> curr = start;
    pair<int, int> prev = {-20001, -20001};

    while (!visited[curr]) {
        visited[curr] = true;
        visited_count++;
        pair<int, int> next_p = adj[curr][0];
        if (next_p == prev) next_p = adj[curr][1];
        prev = curr;
        curr = next_p;
    }

    if (visited_count != N || curr != start) {
        cout << 0 << endl;
        return 0;
    }

    // Self-intersection check using sweep-line
    vector<Event> events;
    vector<int> all_y;
    for (auto const& [x, ys] : col_map) {
        for (int y : ys) all_y.push_back(y);
    }
    sort(all_y.begin(), all_y.end());
    all_y.erase(unique(all_y.begin(), all_y.end()), all_y.end());
    max_y_count = all_y.size();

    auto get_y_idx = [&](int y) {
        return lower_bound(all_y.begin(), all_y.end(), y) - all_y.begin() + 1;
    };

    for (auto const& [x, ys] : col_map) {
        for (size_t i = 0; i < ys.size(); i += 2) {
            events.push_back({x, 2, ys[i], ys[i+1]});
        }
    }
    for (auto const& [y, xs] : row_map) {
        for (size_t i = 0; i < xs.size(); i += 2) {
            events.push_back({xs[i], 1, y, 0});
            events.push_back({xs[i+1], 3, y, 0});
        }
    }

    sort(events.begin(), events.end());

    for (const auto& e : events) {
        if (e.type == 1) {
            update(get_y_idx(e.y1), 1);
        } else if (e.type == 3) {
            update(get_y_idx(e.y1), -1);
        } else {
            int y1_idx = get_y_idx(e.y1);
            int y2_idx = get_y_idx(e.y2);
            // Query range (y1_idx + 1, y2_idx - 1)
            if (y1_idx + 1 <= y2_idx - 1) {
                if (query(y2_idx - 1) - query(y1_idx) > 0) {
                    cout << 0 << endl;
                    return 0;
                }
            }
        }
    }

    cout << total_length << endl;

    return 0;
}

