#include <bits/stdc++.h>
using namespace std;
int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    long long total = 0;
    for (int i = 0; i < n; i++) { cin >> a[i]; total += a[i]; }
    if (total == 0) { cout << "No solution" << endl; return 0; }
    vector<int> res(n);
    vector<double> frac(n);
    int S = 0;
    for (int i = 0; i < n; i++) {
        double p = (double)a[i] * 100.0 / total;
        res[i] = (int)floor(p);
        frac[i] = p - res[i];
        S += res[i];
    }
    int need = 100 - S;
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int x, int y) { return frac[x] > frac[y]; });
    int canRound = 0;
    for (int i = 0; i < n; i++) if (frac[i] > 1e-12) canRound++;
    if (need < 0 || need > canRound) { cout << "No solution" << endl; return 0; }
    for (int i = 0; i < need; i++) res[idx[i]]++;
    for (int i = 0; i < n; i++) { if (i > 0) cout << " "; cout << res[i]; }
    cout << endl;
    return 0;
}
