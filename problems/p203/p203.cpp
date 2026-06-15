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
vector<int64_t> freq;

void read() {
    cin >> n;
    freq.resize(n);
    cin >> freq;
}

void solve() {
    // - The encoded length equals the sum, over every internal node of the
    //   Huffman tree, of that node's mark; equivalently each merge adds the
    //   combined weight to the total.
    //
    // - Since the frequencies arrive already sorted, run the classic two-queue
    //   Huffman: q1 holds the original leaves in order and q2 holds the merged
    //   nodes in nondecreasing order. The two smallest active nodes are always
    //   at the fronts of q1 and q2, so each step pops the two smaller fronts,
    //   adds their sum to the total bit count, and pushes the sum onto q2 until
    //   a single node remains.

    queue<int64_t> q1, q2;
    for(int i = 0; i < n; i++) {
        q1.push(freq[i]);
    }

    int64_t total_bits = 0;
    while(q1.size() + q2.size() > 1) {
        int64_t picked[2];
        for(int i = 0; i < 2; i++) {
            if(q2.empty() || (!q1.empty() && q1.front() < q2.front())) {
                picked[i] = q1.front();
                q1.pop();
            } else {
                picked[i] = q2.front();
                q2.pop();
            }
        }

        int64_t combined = picked[0] + picked[1];
        total_bits += combined;
        q2.push(combined);
    }

    cout << total_bits << '\n';
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
