#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const ll MOD1 = 1000000007;
const ll MOD2 = 1000000009;
const ll base = 131;
const int MAX = 2010;

ll pow1[MAX], pow2[MAX];

void precompute_powers() {
    pow1[0] = 1;
    pow2[0] = 1;
    for (int i = 1; i < MAX; i++) {
        pow1[i] = (pow1[i-1] * base) % MOD1;
        pow2[i] = (pow2[i-1] * base) % MOD2;
    }
}

vector<ll> get_hash(const string& s, ll mod) {
    int n = s.size();
    vector<ll> h(n+1, 0);
    for (int i = 0; i < n; i++) {
        h[i+1] = (h[i] * base + (s[i] - 'a' + 1)) % mod;
    }
    return h;
}

ll get_subhash(const vector<ll>& h, ll* pow_arr, ll mod, int l, int r) {
    int len = r - l + 1;
    ll res = (h[r+1] - h[l] * pow_arr[len] % mod + mod) % mod;
    return res;
}

int main() {
    precompute_powers();
    string s1, s2;
    cin >> s1 >> s2;
    int n1 = s1.size(), n2 = s2.size();

    vector<ll> h1_s2 = get_hash(s2, MOD1);
    vector<ll> h2_s2 = get_hash(s2, MOD2);

    set<pair<ll, ll>> set2;
    for (int i = 0; i < n2; i++) {
        for (int j = i; j < n2; j++) {
            ll hash1 = get_subhash(h1_s2, pow1, MOD1, i, j);
            ll hash2 = get_subhash(h2_s2, pow2, MOD2, i, j);
            set2.insert({hash1, hash2});
        }
    }

    vector<ll> h1_s1 = get_hash(s1, MOD1);
    vector<ll> h2_s1 = get_hash(s1, MOD2);

    int best_len = 0;
    int best_start = 0;

    for (int center = 0; center < 2 * n1; center++) {
        int left = center / 2;
        int right = left + (center % 2);
        while (left >= 0 && right < n1 && s1[left] == s1[right]) {
            int len = right - left + 1;
            ll hash1 = get_subhash(h1_s1, pow1, MOD1, left, right);
            ll hash2 = get_subhash(h2_s1, pow2, MOD2, left, right);
            if (set2.find({hash1, hash2}) != set2.end()) {
                if (len > best_len) {
                    best_len = len;
                    best_start = left;
                }
            }
            left--;
            right++;
        }
    }

    cout << s1.substr(best_start, best_len) << endl;

    return 0;
}
