#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

int n;
vector<int64_t> a;

void read() {
    cin >> n;
    a.resize(n);
    cin >> a;
}

vector<vector<int>> pref_cnt_1;

void prepare() {
    pref_cnt_1.assign(61, vector<int>(n + 1, 0));

    vector<int> order(n);
    iota(order.begin(), order.end(), 0);

    for(int bit = 0; bit < 61; bit++) {
        vector<vector<int>> with_bit(2);
        for(int i = 0; i < n; i++) {
            int pos = order[i];
            int cbit = (a[pos] >> bit) & 1;
            with_bit[cbit].push_back(pos);
            pref_cnt_1[bit][i + 1] = pref_cnt_1[bit][i] + cbit;
        }

        order = std::move(with_bit[0]);
        order.insert(order.end(), with_bit[1].begin(), with_bit[1].end());
    }
}

vector<vector<int64_t>> dp;

int64_t rec(int bit, int carry_cnt) {
    if(bit == 61) {
        return carry_cnt == 0;
    }

    int64_t& memo = dp[bit][carry_cnt];
    if(memo != -1) {
        return memo;
    }

    memo = 0;
    for(int x = 0; x < 2; x++) {
        int xor_all = 0, new_carry_cnt = 0;
        for(int v = 0; v < 2; v++) {
            for(int c = 0; c < 2; c++) {
                int all_cnt = c ? carry_cnt : n - carry_cnt;
                int cnt = c ? pref_cnt_1[bit][carry_cnt]
                            : pref_cnt_1[bit][n] - pref_cnt_1[bit][carry_cnt];

                if(v == 0) {
                    cnt = all_cnt - cnt;
                }

                xor_all ^= (x ^ v ^ c) * (cnt & 1);
                if(v - x - c < 0) {
                    new_carry_cnt += cnt;
                }
            }
        }

        if(xor_all == 0) {
            memo += rec(bit + 1, new_carry_cnt);
        }
    }

    return memo;
}

void solve() {
    // Petya wins iff after subtracting x from every pile the Nim XOR of the
    // piles a_i - x is zero. We count, by a bitwise digit DP processing bits
    // from least to most significant, the number of x >= 0 for which all
    // a_i - x XOR to 0.
    //
    // The state is (bit, carry_cnt): carry_cnt is how many piles currently owe
    // a borrow into this bit position from the subtraction so far. The piles
    // are kept in an order where, at each bit, those with an incoming borrow
    // come first; pref_cnt_1[bit] is a prefix count of set bits in that order,
    // so for either borrow group we can read how many piles have bit value 0
    // or 1. For each choice of x's current bit we compute the parity of the XOR
    // of the resulting difference bits across all piles and only keep choices
    // where that parity is 0, accumulating into the next carry_cnt those piles
    // that now generate a borrow (v - x - c < 0).
    //
    // rec counts all x in [0, 2^61) with zero XOR. The valid range is
    // 0 <= x < min(a_i); the only counted x outside this range is x = min,
    // which makes the smallest pile 0 and the rest a_i - min, so we subtract 1
    // when that configuration also XORs to zero.

    prepare();

    dp.assign(61, vector<int64_t>(n + 1, -1));
    int64_t ans = rec(0, 0);

    int64_t mn = *min_element(a.begin(), a.end()), x = 0;
    for(int i = 0; i < n; i++) {
        x ^= a[i] - mn;
    }

    if(x == 0) {
        ans--;
    }

    cout << ans << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
