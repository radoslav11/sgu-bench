#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Fenwick {
    int n;
    vector<int> tree;
    Fenwick(int n) : n(n), tree(n + 1, 0) {}
    void add(int i, int delta) {
        for (; i <= n; i += i & -i) tree[i] += delta;
    }
    int query(int i) {
        int sum = 0;
        for (; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string S;
    if (!(cin >> S)) return 0;

    int n = S.length();
    vector<vector<int>> pos(26);
    for (int i = 0; i < n; ++i) {
        pos[S[i] - 'A'].push_back(i);
    }

    int odd_count = 0;
    int odd_idx = -1;
    for (int c = 0; c < 26; ++c) {
        if (pos[c].size() % 2 != 0) {
            odd_count++;
            odd_idx = pos[c][pos[c].size() / 2];
        }
    }

    if (odd_count > 1) {
        cout << -1 << "\n";
        return 0;
    }

    vector<pair<int, int>> pairs;
    for (int c = 0; c < 26; ++c) {
        int sz = pos[c].size();
        for (int k = 0; k < sz / 2; ++k) {
            pairs.push_back({pos[c][k], pos[c][sz - 1 - k]});
        }
    }

    sort(pairs.begin(), pairs.end());

    Fenwick fenw(n);
    for (int i = 1; i <= n; ++i) {
        fenw.add(i, 1);
    }

    long long total_swaps = 0;
    int total_unused = n;

    for (auto& p : pairs) {
        int i = p.first;
        int j = p.second;

        long long swaps_i = fenw.query(i + 1) - 1;
        fenw.add(i + 1, -1);
        total_unused--;

        long long swaps_j = total_unused - fenw.query(j + 1);
        fenw.add(j + 1, -1);
        total_unused--;

        total_swaps += swaps_i + swaps_j;
    }

    if (odd_idx != -1) {
        long long swaps_odd = fenw.query(odd_idx + 1) - 1;
        total_swaps += swaps_odd;
    }

    cout << total_swaps << "\n";

    return 0;
}
