#include <bits/stdc++.h>
using namespace std;

// Counts labeled connected unicyclic graphs ("Mescher trees"):
//   T_n = (n-1)!/2 * sum_{j=0..n-3} n^j / j!  (every term integral)
// computed with a base-1e9 bignum supporting small-int mul/div and add.

using Big = vector<uint32_t>;  // little-endian limbs, base 1e9
const uint64_t BASE = 1000000000ULL;

void mul_small(Big& a, uint64_t m) {
    uint64_t carry = 0;
    for(auto& limb: a) {
        uint64_t cur = (uint64_t)limb * m + carry;
        limb = (uint32_t)(cur % BASE);
        carry = cur / BASE;
    }
    while(carry) {
        a.push_back((uint32_t)(carry % BASE));
        carry /= BASE;
    }
}

void div_small(Big& a, uint64_t d) {
    uint64_t rem = 0;
    for(int i = (int)a.size() - 1; i >= 0; i--) {
        uint64_t cur = rem * BASE + a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    while(a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
}

void add_to(Big& a, const Big& b) {
    if(b.size() > a.size()) {
        a.resize(b.size(), 0);
    }
    uint64_t carry = 0;
    for(size_t i = 0; i < a.size(); i++) {
        uint64_t cur = (uint64_t)a[i] + (i < b.size() ? b[i] : 0) + carry;
        a[i] = (uint32_t)(cur % BASE);
        carry = cur / BASE;
    }
    if(carry) {
        a.push_back((uint32_t)carry);
    }
}

int main() {
    int n;
    cin >> n;

    Big f = {1};
    for(int i = 2; i < n; i++) {
        mul_small(f, i);
    }
    div_small(f, 2);

    Big ans = f;
    for(int j = 1; j <= n - 3; j++) {
        mul_small(f, n);
        div_small(f, j);
        add_to(ans, f);
    }

    string out = to_string(ans.back());
    char buf[10];
    for(int i = (int)ans.size() - 2; i >= 0; i--) {
        snprintf(buf, sizeof(buf), "%09u", ans[i]);
        out += buf;
    }
    cout << out << "\n";
    return 0;
}
