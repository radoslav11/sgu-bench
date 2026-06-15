#include <bits/stdc++.h>
using namespace std;

struct Item {
    int s, idx;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    const int MIN_D = -50, MAX_D = 50;
    const int G = 101;
    const int OFF = 1000;
    const int R = 2001;
    const short NEG = -30000;

    vector<vector<Item>> groups(G);

    for (int i = 1; i <= N; ++i) {
        int a, b;
        cin >> a >> b;
        int d = a - b;
        int s = a + b;
        groups[d - MIN_D].push_back({s, i});
    }

    for (auto &v : groups) {
        sort(v.begin(), v.end(), [](const Item &x, const Item &y) {
            if (x.s != y.s) return x.s > y.s;
            return x.idx < y.idx;
        });
        if ((int)v.size() > K) v.resize(K);
    }

    vector<vector<int>> pref(G);
    for (int g = 0; g < G; ++g) {
        pref[g].assign(groups[g].size() + 1, 0);
        for (int i = 0; i < (int)groups[g].size(); ++i) {
            pref[g][i + 1] = pref[g][i] + groups[g][i].s;
        }
    }

    int layers = G + 1;
    int SZ = layers * (K + 1) * R;
    vector<short> dp(SZ, NEG);

    auto id = [&](int g, int k, int diff) {
        return (g * (K + 1) + k) * R + (diff + OFF);
    };

    dp[id(0, 0, 0)] = 0;

    for (int g = 1; g <= G; ++g) {
        int d = MIN_D + (g - 1);
        int m = groups[g - 1].size();

        for (int cnt = 0; cnt <= K; ++cnt) {
            for (int diff = -50 * cnt; diff <= 50 * cnt; ++diff) {
                short cur = dp[id(g - 1, cnt, diff)];
                if (cur == NEG) continue;

                int maxTake = min(m, K - cnt);
                for (int t = 0; t <= maxTake; ++t) {
                    int ncnt = cnt + t;
                    int ndiff = diff + t * d;
                    int val = cur + pref[g - 1][t];

                    short &to = dp[id(g, ncnt, ndiff)];
                    if (val > to) to = (short)val;
                }
            }
        }
    }

    int bestDiff = 0;
    int bestSum = -1;

    for (int diff = -1000; diff <= 1000; ++diff) {
        int val = dp[id(G, K, diff)];
        if (val == NEG) continue;

        if (bestSum == -1 ||
            abs(diff) < abs(bestDiff) ||
            (abs(diff) == abs(bestDiff) && val > bestSum)) {
            bestDiff = diff;
            bestSum = val;
        }
    }

    int A = (bestSum + bestDiff) / 2;
    int B = (bestSum - bestDiff) / 2;

    vector<int> answer;

    int cnt = K;
    int diff = bestDiff;

    for (int g = G; g >= 1; --g) {
        int d = MIN_D + (g - 1);
        int m = groups[g - 1].size();
        int curVal = dp[id(g, cnt, diff)];

        for (int t = 0; t <= min(m, cnt); ++t) {
            int pcnt = cnt - t;
            int pdiff = diff - t * d;

            if (pdiff < -1000 || pdiff > 1000) continue;

            int prevVal = dp[id(g - 1, pcnt, pdiff)];
            if (prevVal == NEG) continue;

            if (prevVal + pref[g - 1][t] == curVal) {
                for (int i = 0; i < t; ++i) {
                    answer.push_back(groups[g - 1][i].idx);
                }
                cnt = pcnt;
                diff = pdiff;
                break;
            }
        }
    }

    sort(answer.begin(), answer.end());

    cout << A << ' ' << B << '\n';
    for (int i = 0; i < (int)answer.size(); ++i) {
        if (i) cout << ' ';
        cout << answer[i];
    }
    cout << '\n';

    return 0;
}
