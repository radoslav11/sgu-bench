#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Problem Breakdown:
 * The Manhattan distance |x1 - x2| + |y1 - y2| <= w can be converted to:
 * max(|(x1 + y1) - (x2 + y2)|, |(x1 - y1) - (x2 - y2)|) <= w
 * Let u = x + y and v = x - y.
 * The condition becomes: |u1 - u2| <= w AND |v1 - v2| <= w
 * 
 * This is equivalent to:
 * u2 - w <= u1 <= u2 + w AND v2 - w <= v1 <= v2 + w
 * 
 * For each hut A at (uA, vA), we want to count how many other huts B
 * satisfy the range [uB - wB, uB + wB] x [vB - wB, vB + wB] containing (uA, vA).
 * 
 * Since w_i >= 1, the hut i's own rectangle always contains (u_i, v_i).
 * Thus, the number of OTHER huts B that can reach A is:
 * (Total number of B whose rectangle contains (uA, vA)) - 1.
 * We need (Total - 1) >= K, which is Total >= K + 1.
 * 
 * This is a classic 2D range query problem: find how many rectangles cover each point.
 * We can use a sweep-line algorithm with a Fenwick tree.
 */

struct Event {
    long long x;
    int type; // 1: Rectangle start, 2: Point query, 3: Rectangle end
    long long v1, v2;
    int id;

    bool operator<(const Event& other) const {
        if (x != other.x) return x < other.x;
        return type < other.type;
    }
};

const int MAXN = 100005;
long long u[MAXN], v[MAXN], L[MAXN], R[MAXN], D[MAXN], U[MAXN];
int results[MAXN];
int bit[3 * MAXN + 10];
int bit_size;

void update(int idx, int val) {
    for (; idx <= bit_size; idx += idx & -idx) {
        bit[idx] += val;
    }
}

int query(int idx) {
    int res = 0;
    for (; idx > 0; idx -= idx & -idx) {
        res += bit[idx];
    }
    return res;
}

int main() {
    // Fast I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<long long> coords;
    for (int i = 1; i <= N; ++i) {
        long long x, y, w;
        cin >> x >> y >> w;
        u[i] = x + y;
        v[i] = x - y;
        L[i] = u[i] - w;
        R[i] = u[i] + w;
        D[i] = v[i] - w;
        U[i] = v[i] + w;
        coords.push_back(v[i]);
        coords.push_back(D[i]);
        coords.push_back(U[i]);
    }

    // Discretize v-coordinates
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    bit_size = coords.size();

    auto get_idx = [&](long long val) {
        return lower_bound(coords.begin(), coords.end(), val) - coords.begin() + 1;
    };

    vector<Event> events;
    events.reserve(3 * N);
    for (int i = 1; i <= N; ++i) {
        events.push_back({L[i], 1, D[i], U[i], i});
        events.push_back({u[i], 2, v[i], 0, i});
        events.push_back({R[i], 3, D[i], U[i], i});
    }

    sort(events.begin(), events.end());

    for (const auto& e : events) {
        if (e.type == 1) {
            // Add interval [D_i, U_i]
            update(get_idx(e.v1), 1);
            update(get_idx(e.v2) + 1, -1);
        } else if (e.type == 2) {
            // Query point v_i
            results[e.id] = query(get_idx(e.v1));
        } else if (e.type == 3) {
            // Remove interval [D_i, U_i]
            update(get_idx(e.v1), -1);
            update(get_idx(e.v2) + 1, 1);
        }
    }

    vector<int> closed_huts;
    for (int i = 1; i <= N; ++i) {
        if (results[i] >= K + 1) {
            closed_huts.push_back(i);
        }
    }

    cout << closed_huts.size() << "\n";
    for (int i = 0; i < (int)closed_huts.size(); ++i) {
        cout << closed_huts[i] << (i == (int)closed_huts.size() - 1 ? "" : " ");
    }
    cout << endl;

    return 0;
}

