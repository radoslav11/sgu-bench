#include <iostream>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    long long n, t1, t2;
    if (!(cin >> n >> t1 >> t2)) return 0;

    long long p = 0, v = 0;
    long long sp = 1, sv = 1;
    long long last_t = 0;

    long long INF = 1e18;

    while (true) {
        long long np = (sp > p) ? (sp * t1) : INF;
        long long nv = (sv > v) ? (sv * t2) : INF;

        long long t = min(np, nv);
        if (t == INF) break;

        last_t = t;

        if (np == t) p++;
        if (nv == t) v++;

        if (p + v < n) {
            if (np == t) sp++;
            if (nv == t) sv++;
        }
    }

    cout << p + v << " " << last_t << "\n";

    return 0;
}
