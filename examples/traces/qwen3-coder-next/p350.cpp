#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long long ll;

int main() {
    int M;
    cin >> M;
    vector<ll> B(M);
    for (int i = 0; i < M; i++) {
        cin >> B[i];
    }
    int N = (1 + (int)sqrt(1 + 8 * M)) / 2;
    sort(B.begin(), B.end());
    set<ll> need(B.begin(), B.end());
    vector<ll> A;
    A.push_back(0);

    for (int i = 0; i < N - 1; i++) {
        ll chosen = -1;
        for (ll x : need) {
            bool valid = true;
            for (ll a : A) {
                ll cur = a ^ x;
                if (need.find(cur) == need.end()) {
                    valid = false;
                    break;
                }
            }
            if (valid) {
                chosen = x;
                break;
            }
        }
        if (chosen == -1) {
            chosen = *need.begin();
        }
        A.push_back(chosen);
        for (ll a : A) {
            ll cur = a ^ chosen;
            auto it = need.find(cur);
            if (it != need.end()) {
                need.erase(it);
            }
        }
    }

    for (int i = 0; i < N; i++) {
        if (i) cout << " ";
        cout << A[i];
    }
    cout << endl;
    return 0;
}
