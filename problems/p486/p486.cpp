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

string s1, s2;

void read() { cin >> s1 >> s2; }

void solve() {
    // Bulls are positions where the secret and guess share the same digit.
    // Since all digits within a number are distinct, the digits present in both
    // numbers (in any position) are bulls plus cows, so we collect that set and
    // subtract the bull count to get the number of cows. c1 marks which digits
    // appear in the secret.

    map<char, int> c1;
    set<char> cows_and_bulls;
    int bulls = 0;

    for(int i = 0; i < (int)s1.size(); i++) {
        if(s1[i] == s2[i]) {
            bulls++;
        }

        c1[s1[i]] = 1;
    }

    for(int i = 0; i < (int)s1.size(); i++) {
        if(c1[s2[i]]) {
            cows_and_bulls.insert(s2[i]);
        }
    }

    cout << bulls << " " << cows_and_bulls.size() - bulls << '\n';
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
