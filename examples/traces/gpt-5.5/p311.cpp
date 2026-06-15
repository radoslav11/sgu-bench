#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    int n;
    vector<long long> bit;

    Fenwick(int n = 0) : n(n), bit(n + 1, 0) {}

    void add(int idx, long long val) {
        for (; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }

    long long sumPrefix(int idx) const {
        long long res = 0;
        for (; idx > 0; idx -= idx & -idx)
            res += bit[idx];
        return res;
    }

    long long total() const {
        return sumPrefix(n);
    }

    int lower_bound(long long target) const {
        int idx = 0;
        int mask = 1;
        while ((mask << 1) <= n) mask <<= 1;

        for (int step = mask; step > 0; step >>= 1) {
            int next = idx + step;
            if (next <= n && bit[next] < target) {
                idx = next;
                target -= bit[next];
            }
        }

        return idx + 1;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAXC = 1000000;

    Fenwick cntTree(MAXC), sumTree(MAXC);
    map<int, long long> stock;

    string type;
    long long n, x;

    while (cin >> type >> n >> x) {
        if (type == "ARRIVE") {
            int c = (int)x;
            stock[c] += n;
            cntTree.add(c, n);
            sumTree.add(c, n * c);
        } else {
            long long t = x;

            if (cntTree.total() < n) {
                cout << "UNHAPPY\n";
                continue;
            }

            int price = cntTree.lower_bound(n);
            long long beforeCnt = cntTree.sumPrefix(price - 1);
            long long beforeSum = sumTree.sumPrefix(price - 1);
            long long cost = beforeSum + (n - beforeCnt) * price;

            if (cost > t) {
                cout << "UNHAPPY\n";
                continue;
            }

            cout << "HAPPY\n";

            long long need = n;
            while (need > 0) {
                auto it = stock.begin();
                int c = it->first;
                long long take = min(need, it->second);

                cntTree.add(c, -take);
                sumTree.add(c, -take * c);

                it->second -= take;
                need -= take;

                if (it->second == 0)
                    stock.erase(it);
            }
        }
    }

    return 0;
}
