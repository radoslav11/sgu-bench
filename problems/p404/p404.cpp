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

int petals, phrases;
vector<string> phrase;

void read() {
    cin >> petals >> phrases;
    phrase.resize(phrases);
    cin >> phrase;
}

void solve() {
    // Phrases are pronounced cyclically, one per petal, so the phrase spoken on
    // the last (petals-th) petal is at index (petals - 1) mod phrases.

    cout << phrase[(petals - 1) % phrases] << '\n';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    read();
    solve();

    return 0;
}
