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
vector<vector<pair<string, string>>> objects;

vector<pair<string, string>> get_object(int cnt) {
    vector<pair<string, string>> obj;
    while(cnt--) {
        string s;
        cin >> s;
        vector<string> xy = {""};
        for(char c: s) {
            if(c == '=') {
                xy.push_back("");
                continue;
            }
            xy.back().push_back(c);
        }

        obj.push_back({xy[0], xy[1]});
    }
    return obj;
}

vector<pair<string, string>> canonize(vector<pair<string, string>> obj) {
    sort(obj.begin(), obj.end());
    return obj;
}

void read() {
    cin >> n;
    objects.resize(n);
    for(int i = 0; i < n; i++) {
        int cnt;
        cin >> cnt;
        objects[i] = canonize(get_object(cnt));
    }
}

string total_canonize(vector<pair<string, string>> obj) {
    string res;
    for(auto [x, y]: obj) {
        res += x + '=' + y + ',';
    }
    return res;
}

void solve() {
    // Each object has at most 4 properties, so it has at most 2^4 = 16 subsets
    // of properties. For every object we enumerate all subsets, canonicalize
    // each subset (sort its key=value pairs, then join with separators into a
    // single string key), and increment a hash map counting how many objects
    // contain that exact subset.
    //
    // A query lists a set of key=value constraints; an object matches iff that
    // set is one of its property subsets. So we canonicalize the query the
    // same way and answer it with one lookup into the precomputed map.

    unordered_map<string, int> cnt;
    for(int i = 0; i < n; i++) {
        for(int mask = 0; mask < (1 << objects[i].size()); mask++) {
            vector<pair<string, string>> obj;
            for(int j = 0; j < objects[i].size(); j++) {
                if(mask & (1 << j)) {
                    obj.push_back(objects[i][j]);
                }
            }
            cnt[total_canonize(obj)]++;
        }
    }

    int q;
    cin >> q;
    while(q--) {
        int c;
        cin >> c;
        vector<pair<string, string>> obj = get_object(c);
        obj = canonize(obj);
        cout << cnt[total_canonize(obj)] << '\n';
    }
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
