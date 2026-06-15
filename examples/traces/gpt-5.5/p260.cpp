#include <bits/stdc++.h>
using namespace std;

const int MAXN = 205;

int n;
vector<bitset<MAXN>> baseMat;

bool solveSystem(const vector<int>& rhs, vector<int>& sol) {
    vector<bitset<MAXN>> a = baseMat;

    for (int i = 0; i < n; ++i) {
        a[i][n] = rhs[i];
    }

    vector<int> where(n, -1);
    int row = 0;

    for (int col = 0; col < n && row < n; ++col) {
        int sel = -1;
        for (int i = row; i < n; ++i) {
            if (a[i][col]) {
                sel = i;
                break;
            }
        }

        if (sel == -1) continue;

        swap(a[sel], a[row]);
        where[col] = row;

        for (int i = 0; i < n; ++i) {
            if (i != row && a[i][col]) {
                a[i] ^= a[row];
            }
        }

        ++row;
    }

    for (int i = 0; i < n; ++i) {
        bool allZero = true;
        for (int j = 0; j < n; ++j) {
            if (a[i][j]) {
                allZero = false;
                break;
            }
        }

        if (allZero && a[i][n]) {
            return false;
        }
    }

    sol.assign(n, 0);
    for (int col = 0; col < n; ++col) {
        if (where[col] != -1) {
            sol[col] = a[where[col]][n];
        }
    }

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    baseMat.assign(n, bitset<MAXN>());

    for (int click = 0; click < n; ++click) {
        int k;
        cin >> k;

        for (int j = 0; j < k; ++j) {
            int cell;
            cin >> cell;
            --cell;
            baseMat[cell][click] = 1;
        }
    }

    vector<int> initial(n);
    for (int i = 0; i < n; ++i) {
        cin >> initial[i];
    }

    vector<int> sol;

    if (!solveSystem(initial, sol)) {
        vector<int> rhs(n);
        for (int i = 0; i < n; ++i) {
            rhs[i] = initial[i] ^ 1;
        }

        if (!solveSystem(rhs, sol)) {
            cout << -1 << '\n';
            return 0;
        }
    }

    vector<int> clicks;
    for (int i = 0; i < n; ++i) {
        if (sol[i]) {
            clicks.push_back(i + 1);
        }
    }

    cout << clicks.size() << '\n';
    for (int x : clicks) {
        cout << x << ' ';
    }
    cout << '\n';

    return 0;
}
