#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

typedef long long ll;

ll fact[17];

void precompute_factorials() {
    fact[0] = 1;
    for (int i = 1; i <= 16; ++i) {
        fact[i] = fact[i - 1] * i;
    }
}

vector<int> get_digits(ll n) {
    if (n == 0) return {0};
    vector<int> digits;
    while (n > 0) {
        digits.push_back(n % 10);
        n /= 10;
    }
    reverse(digits.begin(), digits.end());
    return digits;
}

ll count_le(int counts[10], ll X, int k, const vector<int>& x_digits) {
    if (x_digits.size() < (size_t)k) return 0;
    ll total = 0;
    int current_counts[10];
    for (int i = 0; i < 10; ++i) current_counts[i] = counts[i];
    ll den = 1;
    for (int i = 0; i < 10; ++i) {
        if (current_counts[i] > 1) den *= fact[current_counts[i]];
    }

    for (int i = 0; i < k; ++i) {
        int digit_X = x_digits[i];
        for (int d = 0; d < digit_X; ++d) {
            if (i == 0 && d == 0 && k > 1) continue;
            if (current_counts[d] > 0) {
                total += (fact[k - 1 - i] * current_counts[d]) / den;
            }
        }
        if (current_counts[digit_X] > 0) {
            den /= current_counts[digit_X];
            current_counts[digit_X]--;
        } else {
            return total;
        }
    }
    return total + 1;
}

ll count_in_range(int counts[10], ll L, ll R, int k) {
    if (L > R) return 0;
    vector<int> l_digits = get_digits(L - 1);
    vector<int> r_digits = get_digits(R);
    return count_le(counts, R, k, r_digits) - count_le(counts, L - 1, k, l_digits);
}

void generate_multisets(int digit, int remaining, int counts[10], int k, ll L, ll R, ll& ans) {
    if (digit == 10) {
        if (remaining == 0) {
            ll cnt = count_in_range(counts, L, R, k);
            if (cnt == 2) ans += 2;
        }
        return;
    }
    for (int i = 0; i <= remaining; ++i) {
        counts[digit] = i;
        generate_multisets(digit + 1, remaining - i, counts, k, L, R, ans);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll l, r;
    if (!(cin >> l >> r)) return 0;

    precompute_factorials();

    ll total_ans = 0;
    vector<int> l_digits = get_digits(l);
    vector<int> r_digits = get_digits(r);
    int len_l = l_digits.size();
    int len_r = r_digits.size();

    for (int k = len_l; k <= len_r; ++k) {
        ll L_k = 1;
        for (int i = 0; i < k - 1; ++i) L_k *= 10;
        if (k == 1) L_k = 1;
        
        ll R_k = 0;
        if (k < 16) {
            R_k = 1;
            for (int i = 0; i < k; ++i) R_k *= 10;
            R_k -= 1;
        } else {
            R_k = 1000000000000000LL;
        }

        ll actual_L = max(l, L_k);
        ll actual_R = min(r, R_k);

        if (actual_L <= actual_R) {
            int counts[10] = {0};
            generate_multisets(0, k, counts, k, actual_L, actual_R, total_ans);
        }
    }

    cout << total_ans << endl;

    return 0;
}

