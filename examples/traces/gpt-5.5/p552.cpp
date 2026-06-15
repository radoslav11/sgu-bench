#include <bits/stdc++.h>
using namespace std;

struct CustomHash {
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }

    size_t operator()(const string& s) const {
        static const uint64_t FIXED_RANDOM =
            chrono::steady_clock::now().time_since_epoch().count();

        uint64_t h = FIXED_RANDOM;
        for (unsigned char c : s) {
            h = splitmix64(h ^ c);
        }
        return h;
    }
};

string make_key(vector<string>& props) {
    sort(props.begin(), props.end());

    string res;
    for (const string& p : props) {
        res += p;
        res += '#';
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    unordered_map<string, int, CustomHash> cnt;
    cnt.reserve(n * 15 * 2);
    cnt.max_load_factor(0.7);

    for (int i = 0; i < n; ++i) {
        int k;
        cin >> k;

        vector<string> props(k);
        for (int j = 0; j < k; ++j) {
            cin >> props[j];
        }

        sort(props.begin(), props.end());

        for (int mask = 1; mask < (1 << k); ++mask) {
            vector<string> subset;
            for (int j = 0; j < k; ++j) {
                if (mask & (1 << j)) {
                    subset.push_back(props[j]);
                }
            }

            string key = make_key(subset);
            ++cnt[key];
        }
    }

    int m;
    cin >> m;

    while (m--) {
        int l;
        cin >> l;

        vector<string> query(l);
        for (int i = 0; i < l; ++i) {
            cin >> query[i];
        }

        string key = make_key(query);

        auto it = cnt.find(key);
        if (it == cnt.end()) {
            cout << 0 << '\n';
        } else {
            cout << it->second << '\n';
        }
    }

    return 0;
}
