#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int bitLen(ll m) {
    int b = 0;
    while (m > 0) { b++; m >>= 1; }
    return b;
}

ll rotateLeft(ll m, int i, int b) {
    ll mask = (1LL << b) - 1;
    return ((m << i) | (m >> (b - i))) & mask;
}

bool isCanonical(ll m) {
    int b = bitLen(m);
    if (b == 1) return true;
    ll minRot = m;
    for (int i = 1; i < b; i++) {
        ll r = rotateLeft(m, i, b);
        if (r >= (1LL << (b - 1))) {
            if (r < minRot) minRot = r;
        }
    }
    return minRot == m;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    ll n;
    cin >> n;
    
    for (ll m = n; m >= 1; m--) {
        if (isCanonical(m)) {
            cout << m << endl;
            return 0;
        }
    }
    return 0;
}
