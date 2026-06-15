#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string A;
    int B;
    cin >> A >> B;

    if (B == 1) {
        cout << A << '\n';
        return 0;
    }

    const int DEC_BASE = 1000000000;
    vector<int> dec;

    int firstLen = (int)A.size() % 9;
    int pos = 0;
    if (firstLen) {
        dec.push_back(stoi(A.substr(0, firstLen)));
        pos = firstLen;
    }
    for (; pos < (int)A.size(); pos += 9) {
        dec.push_back(stoi(A.substr(pos, 9)));
    }

    vector<int> digits; // base-B digits, least significant first

    while (!dec.empty()) {
        long long rem = 0;
        vector<int> q;
        q.reserve(dec.size());

        for (int x : dec) {
            long long cur = rem * DEC_BASE + x;
            int divPart = (int)(cur / B);
            rem = cur % B;
            if (!q.empty() || divPart != 0) q.push_back(divPart);
        }

        digits.push_back((int)rem);
        dec.swap(q);
    }

    const long long INF = (1LL << 62);

    long long dp0 = 0, dp1 = INF;

    for (int d : digits) {
        long long ndp0 = INF, ndp1 = INF;

        if (dp0 < INF) {
            int x = d;
            ndp0 = min(ndp0, dp0 + x);
            ndp1 = min(ndp1, dp0 + (B - x));
        }

        if (dp1 < INF) {
            int x = d + 1;
            ndp0 = min(ndp0, dp1 + x);
            ndp1 = min(ndp1, dp1 + (B - x));
        }

        dp0 = ndp0;
        dp1 = ndp1;
    }

    long long ans = min(dp0, dp1 + 1);
    cout << ans << '\n';

    return 0;
}
