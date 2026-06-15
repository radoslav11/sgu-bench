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

void read() {
    cin >> s;
}

void solve() {
    // Apply the plural rules in order of priority. A word ending in y drops the
    // y and gets "ies". A word ending in ch, x, s, or o gets "es". A word
    // ending in f or fe turns the final f into v and gets "ves" (we pop the
    // characters back to the f, replace it, and append "es"). Otherwise just
    // append "s".

    if(s.back() == 'y') {
        s.erase(prev(s.end()));
        cout << s << "ies" << '\n';
    } else if((s.back() == 'h' && s[s.size() - 2] == 'c') || s.back() == 'o' ||
              s.back() == 'x' || s.back() == 's') {
        cout << s << "es" << '\n';
    } else if(s.back() == 'f' || (s[s.size() - 2] == 'f' && s.back() == 'e')) {
        while(s.back() != 'f') {
            s.pop_back();
        }

        s[s.size() - 1] = 'v';
        s += "es";
        cout << s << '\n';
    } else {
        cout << s << "s" << '\n';
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
