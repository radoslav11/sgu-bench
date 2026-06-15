#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Point {
    long long x, y, w;
    long long u, v;
};

struct Event {
    long long u;
    int type;
    int val1;
    int val2;
    int id;
    bool operator<(const Event& other) const {
        if (u != other.u) return u < other.u;
        return type < other.type;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<Point> pts(N + 1);
    vector<long long> comp_v;
    comp_v.reserve(3 * N);

    for (int i = 1; i <= N; ++i) {
        cin >> pts[i].x >> pts[i].y >> pts[i].w;
        pts[i].u = pts[i].x + pts[i].y;
        pts[i].v = pts[i].x - pts[i].y;
        comp_v.push_back(pts[i].v);
        comp_v.push_back(pts[i].v - pts[i].w);
        comp_v.push_back(pts[i].v + pts[i].w + 1);
    }

    sort(comp_v.begin(), comp_v.end());
    comp_v.erase(unique(comp_v.begin(), comp_v.end()), comp_v.end());

    auto get_idx = [&](long long val) {
        return lower_bound(comp_v.begin(), comp_v.end(), val) - comp_v.begin() + 1;
    };

    vector<Event> events;
    events.reserve(3 * N);

    for (int i = 1; i <= N; ++i) {
        int l_idx = get_idx(pts[i].v - pts[i].w);
        int r_idx = get_idx(pts[i].v + pts[i].w + 1);
        int v_idx = get_idx(pts[i].v);

        events.push_back({pts[i].u - pts[i].w, 1, l_idx, r_idx, i});
        events.push_back({pts[i].u + pts[i].w + 1, 2, l_idx, r_idx, i});
        events.push_back({pts[i].u, 3, v_idx, 0, i});
    }

    sort(events.begin(), events.end());

    int M = comp_v.size();
    vector<int> bit(M + 1, 0);

    auto add = [&](int idx, int val) {
        for (; idx <= M; idx += idx & -idx) {
            bit[idx] += val;
        }
    };

    auto query = [&](int idx) {
        int sum = 0;
        for (; idx > 0; idx -= idx & -idx) {
            sum += bit[idx];
        }
        return sum;
    };

    vector<int> ans;
    for (const auto& ev : events) {
        if (ev.type == 1) {
            add(ev.val1, 1);
            add(ev.val2, -1);
        } else if (ev.type == 2) {
            add(ev.val1, -1);
            add(ev.val2, 1);
        } else {
            int count = query(ev.val1);
            if (count >= K + 1) {
                ans.push_back(ev.id);
            }
        }
    }

    sort(ans.begin(), ans.end());

    cout << ans.size() << "\n";
    if (!ans.empty()) {
        for (int i = 0; i < (int)ans.size(); ++i) {
            cout << ans[i] << (i + 1 == (int)ans.size() ? "" : " ");
        }
        cout << "\n";
    }

    return 0;
}
