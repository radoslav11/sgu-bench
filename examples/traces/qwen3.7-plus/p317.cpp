#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

const long double INF = 1e18;

struct Horse {
    long long d;
    long long v;
};

struct Stable {
    vector<Horse> horses;
    vector<Horse> useful;
};

struct InputStable {
    long long x;
    vector<pair<long long, long long>> horses; // v, d
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long B;
    int N;
    if (!(cin >> B >> N)) return 0;

    vector<InputStable> input_stables(N);
    for (int i = 0; i < N; ++i) {
        int m;
        cin >> input_stables[i].x >> m;
        input_stables[i].horses.resize(m);
        for (int j = 0; j < m; ++j) {
            cin >> input_stables[i].horses[j].first >> input_stables[i].horses[j].second;
        }
    }

    vector<long long> positions;
    positions.push_back(0);
    positions.push_back(B);
    for (int i = 0; i < N; ++i) {
        if (input_stables[i].x >= 0 && input_stables[i].x <= B) {
            positions.push_back(input_stables[i].x);
        }
    }
    sort(positions.begin(), positions.end());
    positions.erase(unique(positions.begin(), positions.end()), positions.end());

    int m_pos = positions.size();
    vector<Stable> stables(m_pos);

    for (int i = 0; i < N; ++i) {
        long long x = input_stables[i].x;
        if (x >= 0 && x <= B) {
            int idx = lower_bound(positions.begin(), positions.end(), x) - positions.begin();
            for (auto& h : input_stables[i].horses) {
                stables[idx].horses.push_back({h.second, h.first}); // d, v
            }
        }
    }

    for (int i = 0; i < m_pos; ++i) {
        if (stables[i].horses.empty()) continue;

        sort(stables[i].horses.begin(), stables[i].horses.end(), [](const Horse& a, const Horse& b) {
            if (a.d != b.d) return a.d < b.d;
            return a.v > b.v;
        });

        vector<Horse> filtered;
        for (const auto& h : stables[i].horses) {
            if (!filtered.empty() && filtered.back().d == h.d) {
                continue;
            }
            filtered.push_back(h);
        }

        long long max_v = -1;
        for (int j = (int)filtered.size() - 1; j >= 0; --j) {
            if (filtered[j].v > max_v) {
                max_v = filtered[j].v;
                stables[i].useful.push_back({filtered[j].d, max_v});
            }
        }
        reverse(stables[i].useful.begin(), stables[i].useful.end());
    }

    vector<long double> dp(m_pos, INF);
    dp[0] = 0.0L;

    for (int i = 0; i < m_pos; ++i) {
        if (dp[i] == INF) continue;
        int ptr = 0;
        int sz = stables[i].useful.size();
        for (int j = i + 1; j < m_pos; ++j) {
            long long D = positions[j] - positions[i];
            while (ptr < sz && stables[i].useful[ptr].d < D) {
                ptr++;
            }
            if (ptr == sz) break;
            long double time = dp[i] + (long double)D / stables[i].useful[ptr].v;
            if (time < dp[j]) {
                dp[j] = time;
            }
        }
    }

    if (dp[m_pos - 1] == INF) {
        cout << -1 << "\n";
    } else {
        cout << fixed << setprecision(8) << dp[m_pos - 1] << "\n";
    }

    return 0;
}
