#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, r;
    cin >> n >> r;

    int64 cnt3 = 0;   // number of blocks of size 3
    int64 cnt2 = 0;   // number of blocks of size 2

    for (int i = 0; i < n; ++i) {
        int a;
        cin >> a;
        if (a % 2 == 1) {
            cnt3++;
            cnt2 += (a - 3) / 2;
        } else {
            cnt2 += a / 2;
        }
    }

    int m = r / 3; // maximum number of size-3 blocks in one table

    auto feasible = [&](int64 k) -> bool {
        if (cnt3 > k * 1LL * m) return false;

        int mo = (m % 2 == 1 ? m : m - 1); // largest odd <= m
        int me = (m % 2 == 0 ? m : m - 1); // largest even <= m

        int64 oddLeftovers;

        if (r % 2 == 0) {
            // A table has odd remaining capacity iff it contains odd number of 3-blocks.
            int64 parity = cnt3 & 1LL;
            int64 limit = min(k, cnt3);
            int64 t;

            if (m % 2 == 0) {
                // capacity decreases with odd tables, so take the smallest possible t
                t = parity;
                if (t > limit) return false;
                if (k * 1LL * me + t * 1LL * (mo - me) < cnt3) return false;
            } else {
                // capacity increases with odd tables
                int64 need = cnt3 - k * 1LL * me;
                if (need < 0) need = 0;
                t = max(parity, need);
                if ((t & 1LL) != parity) ++t;
                if (t > limit) return false;
            }

            oddLeftovers = t;
        } else {
            // A table has odd remaining capacity iff it contains even number of 3-blocks.
            // So maximize the number of tables with odd number of 3-blocks.
            int64 parity = cnt3 & 1LL;
            int64 limit = min(k, cnt3);

            int64 t;
            if (m % 2 == 1) {
                t = limit;
                if ((t & 1LL) != parity) --t;
                if (t < 0) return false;
            } else {
                int64 capLimit = k * 1LL * me - cnt3;
                limit = min(limit, capLimit);
                t = limit;
                if ((t & 1LL) != parity) --t;
                if (t < 0) return false;
            }

            oddLeftovers = k - t;
        }

        int64 freeCapacity = k * 1LL * r - 3LL * cnt3;
        if (freeCapacity < 0) return false;

        int64 maxCnt2 = (freeCapacity - oddLeftovers) / 2;
        return maxCnt2 >= cnt2;
    };

    int64 totalPeople = 2LL * cnt2 + 3LL * cnt3;

    int64 low = 0;
    int64 high = cnt3 + (2LL * cnt2 + r - 1) / r + 10;
    high = max(high, (totalPeople + r - 1) / r + 10);

    while (!feasible(high)) high *= 2;

    while (low < high) {
        int64 mid = (low + high) / 2;
        if (feasible(mid)) high = mid;
        else low = mid + 1;
    }

    cout << low << '\n';
    return 0;
}
