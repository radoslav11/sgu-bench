#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef long long ll;

ll modpow(ll base, ll exp, ll modulus) {
    base %= modulus;
    ll result = 1;
    while (exp > 0) {
        if (exp & 1) 
            result = (result * base) % modulus;
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
}

int main() {
    int n, p;
    cin >> n >> p;
    vector<vector<int>> a(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> a[i][j];
            a[i][j] %= p;
            if (a[i][j] < 0) 
                a[i][j] += p;
        }
    }

    ll det = 1;
    for (int i = 0; i < n; i++) {
        int pivot = -1;
        for (int j = i; j < n; j++) {
            if (a[j][i] != 0) {
                pivot = j;
                break;
            }
        }
        if (pivot == -1) {
            det = 0;
            break;
        }
        if (pivot != i) {
            swap(a[i], a[pivot]);
            det = (p - det) % p;
        }
        det = (det * a[i][i]) % p;
        ll inv = modpow(a[i][i], p-2, p);
        for (int j = i + 1; j < n; j++) {
            ll factor = (ll)a[j][i] * inv % p;
            for (int k = i; k < n; k++) {
                a[j][k] = (a[j][k] - factor * a[i][k] % p + p) % p;
            }
        }
    }

    ll two_pow = 1;
    for (int i = 0; i < n; i++) {
        two_pow = (two_pow * 2) % p;
    }
    ll ans = (two_pow * det) % p;
    if (ans < 0) 
        ans += p;
    cout << ans << endl;

    return 0;
}
