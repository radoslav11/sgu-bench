#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

struct Candidate {
    int id;
    long long rem;
    long long L;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    vector<long long> a(n);
    long long sum_a = 0;
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
        sum_a += a[i];
    }

    if (sum_a == 0) {
        cout << "No solution\n";
        return 0;
    }

    vector<Candidate> cands(n);
    long long sum_L = 0;
    for (int i = 0; i < n; ++i) {
        cands[i].id = i;
        cands[i].L = (100LL * a[i]) / sum_a;
        cands[i].rem = (100LL * a[i]) % sum_a;
        sum_L += cands[i].L;
    }

    long long diff = 100 - sum_L;

    if (diff > 0) {
        sort(cands.begin(), cands.end(), [](const Candidate& x, const Candidate& y) {
            return x.rem > y.rem;
        });

        for (int i = 0; i < diff; ++i) {
            cands[i].L += 1;
        }
    }

    vector<long long> ans(n);
    for (int i = 0; i < n; ++i) {
        ans[cands[i].id] = cands[i].L;
    }

    for (int i = 0; i < n; ++i) {
        cout << ans[i] << (i == n - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
