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
vector<pair<char, int>> a;

void read() {
    cin >> n;
    a.resize(n - 1);
    for(int i = 0; i < n - 1; i++) {
        cin >> a[i].first >> a[i].second;
    }
}

void solve() {
    // Walk left to right, greedily keeping every dragon killed so far in a
    // min-heap keyed by the gold it holds (sum tracks the total gold of the
    // currently kept dragons). When we reach a princess with beauty x, the
    // Knight may not have killed x or more dragons or she would marry him and
    // end the journey early; so while the heap holds at least x dragons we
    // discard the cheapest ones until fewer than x remain.
    //
    // The final cell is the beloved princess: the chosen set is valid iff
    // after processing all earlier cells the heap holds fewer dragons than her
    // beauty. The remaining dragons in the heap are exactly the ones to kill,
    // and their stored 1-based cell indices form the answer.

    int sum = 0;
    priority_queue<
        pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>>
        q;

    for(int i = 0; i < n - 2; i++) {
        auto [c, x] = a[i];
        if(c == 'd') {
            sum += x;
            q.push({x, i});
        } else {
            while((int)q.size() >= x) {
                sum -= q.top().first;
                q.pop();
            }
        }
    }

    if(a.back().first != 'p' || (int)q.size() < a.back().second) {
        cout << -1 << '\n';
        return;
    }

    cout << sum << '\n';
    vector<int> kills;
    while(!q.empty()) {
        kills.push_back(q.top().second + 2);
        q.pop();
    }

    cout << kills.size() << '\n';
    sort(kills.begin(), kills.end());
    cout << kills << '\n';
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
