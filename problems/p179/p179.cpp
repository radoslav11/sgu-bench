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

string s;

void read() { cin >> s; }

void solve() {
    // Next balanced bracket sequence in lexicographic order, with '(' < ')'.
    // Scan from the right past the trailing "()" pairs; the first position we
    // can increment is an '(' that we flip to ')'. Among the positions before
    // and including it we count how many '(' and ')' remain, then fill the
    // suffix with the lexicographically smallest valid completion: all the
    // remaining '(' first, then all the remaining ')'. If no such '(' exists
    // there is no greater sequence and we report "No solution".

    int n = s.size();

    int i = n - 2;
    while(i >= 0 && s.substr(i, 2) == "()") {
        i -= 2;
    }

    if(i < 0) {
        cout << "No solution\n";
        return;
    }

    while(i >= 0 && s[i] == ')') {
        i--;
    }

    if(i < 0) {
        cout << "No solution\n";
        return;
    }

    s[i] = ')';

    int open = 0, close = 0;
    for(int j = 0; j <= i; j++) {
        if(s[j] == '(') {
            open++;
        } else {
            close++;
        }
    }

    int remaining_open = n / 2 - open;
    int remaining_close = n / 2 - close;

    string result = s.substr(0, i + 1);
    for(int j = 0; j < remaining_open; j++) {
        result += '(';
    }

    for(int j = 0; j < remaining_close; j++) {
        result += ')';
    }

    cout << result << '\n';
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
