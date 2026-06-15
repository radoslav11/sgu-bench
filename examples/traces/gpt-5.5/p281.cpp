#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<string> name(N);
    vector<int> a(N), b(N);
    unordered_map<string, int> id;
    id.reserve(N * 2);

    for (int i = 0; i < N; ++i) {
        cin >> name[i];
        id[name[i]] = i;
        a[i] = i;
    }

    for (int i = 0; i < N; ++i) {
        string s;
        cin >> s;
        b[i] = id[s];
    }

    vector<int> diff(N, 0);
    int nonzero = 0;

    auto add = [&](int x, int delta) {
        if (diff[x] == 0) ++nonzero;
        diff[x] += delta;
        if (diff[x] == 0) --nonzero;
    };

    vector<int> boundaries;
    for (int i = 0; i < N; ++i) {
        add(a[i], 1);
        add(b[i], -1);

        if (nonzero == 0) {
            boundaries.push_back(i + 1);
        }
    }

    int prev = 0;
    for (int bound : boundaries) {
        vector<int> block;
        for (int i = prev; i < bound; ++i) {
            block.push_back(a[i]);
        }

        sort(block.begin(), block.end(), [&](int x, int y) {
            return name[x] < name[y];
        });

        for (int x : block) {
            cout << name[x] << '\n';
        }

        prev = bound;
    }

    return 0;
}
