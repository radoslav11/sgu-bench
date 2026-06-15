#include <iostream>
#include <string>
#include <climits>
#include <cctype>
#include <cmath>
#include <algorithm>
using namespace std;

typedef long long ll;

bool is_minimal_generator(ll m, int b) {
    ll low = 1LL << b;
    string s = "";
    for (int i = b; i >= 0; i--) {
        if (m & (1LL << i)) {
            s += '1';
        } else {
            s += '0';
        }
    }
    string t = s + s;
    int len = b + 1;
    ll min_val = (1LL << (b + 2));
    for (int i = 0; i < len; i++) {
        if (t[i] == '1') {
            ll num = 0;
            for (int j = 0; j < len; j++) {
                num = num * 2 + (t[i + j] - '0');
            }
            if (num >= low) {
                if (num < min_val) {
                    min_val = num;
                }
            }
        }
    }
    return (min_val == m);
}

int main() {
    ll n;
    cin >> n;
    if (n == 1) {
        cout << 1 << endl;
        return 0;
    }
    int leading_zeros = __builtin_clzll((unsigned long long)n);
    int b = 63 - leading_zeros;
    ll low = 1LL << b;
    ll ans = low - 1;
    for (ll m = n; m >= low; m--) {
        if (is_minimal_generator(m, b)) {
            ans = m;
            break;
        }
    }
    cout << ans << endl;
    return 0;
}
