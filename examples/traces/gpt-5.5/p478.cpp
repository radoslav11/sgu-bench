#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int a, b;
    if (!(cin >> a >> b)) return 0;

    int n;
    cin >> n;

    vector<int> s(n);
    for (int i = 0; i < n; ++i) cin >> s[i];

    vector<vector<int>> parent(n, vector<int>(a + 1, -1));
    vector<int> prevValid(a + 1, 0), curValid(a + 1, 0);

    for (int u = 0; u <= a; ++u) {
        int v = s[0] - u;
        if (v < 0 || v > b) continue;

        if (0 <= u && b >= v) {
            curValid[u] = 1;
            parent[0][u] = 0;
        }
    }

    for (int i = 1; i < n; ++i) {
        prevValid = curValid;
        fill(curValid.begin(), curValid.end(), 0);

        for (int u = 0; u <= a; ++u) {
            int v = s[i] - u;
            if (v < 0 || v > b) continue;

            for (int pu = 0; pu <= a; ++pu) {
                if (!prevValid[pu]) continue;

                int pv = s[i - 1] - pu;

                if (pu <= u && pv >= v) {
                    curValid[u] = 1;
                    parent[i][u] = pu;
                    break;
                }
            }
        }
    }

    int lastU = -1;
    for (int u = 0; u <= a; ++u) {
        if (curValid[u]) {
            lastU = u;
            break;
        }
    }

    if (lastU == -1) {
        cout << "ERROR\n";
        return 0;
    }

    vector<int> boys(n), girls(n);
    int u = lastU;

    for (int i = n - 1; i >= 0; --i) {
        boys[i] = u;
        girls[i] = s[i] - u;
        u = parent[i][u];
    }

    int prevBoys = 0;
    int prevGirls = b;

    for (int i = 0; i < n; ++i) {
        int joinedBoys = boys[i] - prevBoys;
        int separatedGirls = prevGirls - girls[i];

        cout << joinedBoys << ' ' << separatedGirls << '\n';

        prevBoys = boys[i];
        prevGirls = girls[i];
    }

    return 0;
}
