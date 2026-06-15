#include <bits/stdc++.h>
using namespace std;

using ll = long long;

static const ll NEG = -(1LL << 60);

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T, N;
    cin >> T >> N;

    while (T--) {
        vector<int> x(3 * N);
        for (int &v : x) cin >> v;

        sort(x.begin(), x.end());

        /*
          The N smallest elements are optimally used as B.

          For the remaining 2N elements, we must choose N elements C and N elements A.
          If C_1 <= ... <= C_N and A_1 <= ... <= A_N, the value is

              sum C_i * (A_i - B_i)

          where B_i are the B elements in descending order.

          We process the remaining numbers increasingly. Choosing an element as C opens
          the next pair, choosing an element as A closes the oldest still-open pair.

          N is at most 25.  The number of reachable states with the pair-ordering
          restriction is small enough for the official limits.
        */

        vector<int> b(N);
        for (int i = 0; i < N; i++) b[i] = x[N - 1 - i];

        vector<int> y(2 * N);
        for (int i = 0; i < 2 * N; i++) y[i] = x[N + i];

        map<vector<int>, ll> dp;
        dp[{}] = 0;

        int closed = 0;

        for (int pos = 0; pos < 2 * N; pos++) {
            map<vector<int>, ll> ndp;

            for (auto &[q, val] : dp) {
                int opened = (int)q.size() + closed;

                // Put y[pos] into C.
                if (opened < N) {
                    vector<int> nq = q;
                    nq.push_back(y[pos]);
                    auto it = ndp.find(nq);
                    if (it == ndp.end() || it->second < val)
                        ndp[nq] = val;
                }

                // Put y[pos] into A, closing the oldest opened C.
                if (!q.empty()) {
                    vector<int> nq(q.begin() + 1, q.end());
                    int c = q.front();
                    int idx = closed;
                    ll add = 1LL * c * (y[pos] - b[idx]);
                    auto it = ndp.find(nq);
                    if (it == ndp.end() || it->second < val + add)
                        ndp[nq] = val + add;
                }
            }

            dp.swap(ndp);

            // closed count is not unique per layer, so recompute it implicitly:
            // after processing pos+1 elements:
            // processed = opened_C + closed_A
            // opened_C = q.size() + closed_A
            // => closed_A = (processed - q.size()) / 2
            map<vector<int>, ll> fixed;
            for (auto &[q, val] : dp) {
                int processed = pos + 1;
                int cl = (processed - (int)q.size()) / 2;
                if (processed - (int)q.size() >= 0 &&
                    ((processed - (int)q.size()) % 2 == 0) &&
                    cl >= 0 && cl <= N) {
                    auto it = fixed.find(q);
                    if (it == fixed.end() || it->second < val)
                        fixed[q] = val;
                }
            }
            dp.swap(fixed);

            if (!dp.empty()) {
                int processed = pos + 1;
                closed = -1;
                for (auto &[q, val] : dp) {
                    int cl = (processed - (int)q.size()) / 2;
                    if (closed == -1) closed = cl;
                    else {
                        closed = -2;
                        break;
                    }
                }
                if (closed < 0) closed = 0;
            }
        }

        cout << dp[{}] << '\n';
    }

    return 0;
}
