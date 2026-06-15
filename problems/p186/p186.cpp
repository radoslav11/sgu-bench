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
vector<int> lengths;

void read() {
    cin >> n;
    lengths.resize(n);
    cin >> lengths;
}

void solve() {
    // To join all chains into one line we need N - 1 connections, and each
    // connection costs one minute of opening a link. The cheapest links to
    // sacrifice are those of the shortest chains: opening every link of a small
    // chain both spends those links as connectors and removes that chain from
    // the list to be attached. Sort the chains ascending and use two pointers:
    // left points at the chain we are dismantling for links, right at the
    // current largest chain still needing attachment. Each minute we open one
    // link from the left chain (decrementing its remaining length) and use it
    // to absorb the right chain (right--, time++); when the left chain is
    // exhausted we advance left. We stop once left meets right, i.e. all
    // chains are connected.

    sort(lengths.begin(), lengths.end());

    int left = 0, right = n - 1, time = 0;
    while(left < right) {
        if(lengths[left] > 0) {
            lengths[left]--;
            right--;
            time++;
            if(lengths[left] == 0) {
                left++;
            }
        } else {
            left++;
        }
    }

    cout << time << '\n';
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
