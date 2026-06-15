#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>
using namespace std;

unordered_map<long long, int> memo;

long long encode(const vector<int>& state) {
    long long h = 0;
    for (int x : state) {
        h = h * 21 + x;
    }
    return h;
}

int grundy(vector<int> state) {
    long long key = encode(state);
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }

    if (state.empty()) {
        memo[key] = 0;
        return 0;
    }

    set<int> nexts;

    for (int k = 1; k <= state[0]; k++) {
        vector<int> new_state;
        if (k < state[0]) {
            new_state.push_back(state[0] - k);
            for (int i = 1; i < state.size(); i++) {
                new_state.push_back(state[i]);
            }
        } else {
            for (int i = 1; i < state.size(); i++) {
                new_state.push_back(state[i]);
            }
        }
        int g_val = grundy(new_state);
        nexts.insert(g_val);
    }

    for (int k = 1; k <= state.back(); k++) {
        vector<int> new_state;
        for (int i = 0; i < (int)state.size() - 1; i++) {
            new_state.push_back(state[i]);
        }
        if (k < state.back()) {
            new_state.push_back(state.back() - k);
        }
        int g_val = grundy(new_state);
        nexts.insert(g_val);
    }

    int mex = 0;
    while (nexts.find(mex) != nexts.end()) {
        mex++;
    }
    memo[key] = mex;
    return mex;
}

int main() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }

    memo.clear();
    int g = grundy(a);
    if (g != 0) {
        cout << "FEDOR" << endl;
    } else {
        cout << "SERGEY" << endl;
    }
    return 0;
}
