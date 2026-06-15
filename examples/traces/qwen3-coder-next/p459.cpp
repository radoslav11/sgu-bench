#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <algorithm>
using namespace std;

int n, k;
vector<string> final_path;

bool dfs(int state, vector<bool>& visited, vector<string>& path) {
    int num_non_empty = 0;
    for (int i = 1; i < (1 << n); i++) {
        if (__builtin_popcount(i) <= k) {
            num_non_empty++;
        }
    }

    if (path.size() == num_non_empty) {
        if (state == 0) {
            final_path = path;
            return true;
        }
        if (__builtin_popcount(state) == 1) {
            int bit = __builtin_ctz(state);
            path.push_back("-" + to_string(bit + 1));
            final_path = path;
            return true;
        }
        return false;
    }

    for (int i = 0; i < n; i++) {
        if (!(state & (1 << i))) {
            int next_state = state | (1 << i);
            if (__builtin_popcount(next_state) <= k) {
                if (!visited[next_state]) {
                    visited[next_state] = true;
                    path.push_back("+" + to_string(i + 1));
                    if (dfs(next_state, visited, path)) return true;
                    path.pop_back();
                    visited[next_state] = false;
                }
            }
        }

        if (state & (1 << i)) {
            int next_state = state ^ (1 << i);
            if (next_state == 0) {
                if (path.size() == num_non_empty) {
                    path.push_back("-" + to_string(i + 1));
                    final_path = path;
                    return true;
                }
            } else {
                if (!visited[next_state]) {
                    visited[next_state] = true;
                    path.push_back("-" + to_string(i + 1));
                    if (dfs(next_state, visited, path)) return true;
                    path.pop_back();
                    visited[next_state] = false;
                }
            }
        }

        if ((state & (1 << i)) && (i + 1 < n) && !(state & (1 << (i + 1)))) {
            int next_state = state ^ (1 << i);
            next_state |= (1 << (i + 1));
            if (!visited[next_state]) {
                visited[next_state] = true;
                path.push_back("++" + to_string(i + 1));
                if (dfs(next_state, visited, path)) return true;
                path.pop_back();
                visited[next_state] = false;
            }
        }

        if ((state & (1 << i)) && (i - 1 >= 0) && !(state & (1 << (i - 1)))) {
            int next_state = state ^ (1 << i);
            next_state |= (1 << (i - 1));
            if (!visited[next_state]) {
                visited[next_state] = true;
                path.push_back("--" + to_string(i + 1));
                if (dfs(next_state, visited, path)) return true;
                path.pop_back();
                visited[next_state] = false;
            }
        }
    }
    return false;
}

int main() {
    cin >> n >> k;

    if (k == 1) {
        if (n == 1) {
            cout << "+1-1" << endl;
        } else {
            cout << "+1";
            for (int i = 1; i <= n - 1; i++) {
                cout << "++" << i;
            }
            cout << "-" << n << endl;
        }
        return 0;
    }

    if (k == n) {
        int total_states = 1 << n;
        vector<string> moves;
        int state = 0;
        for (int i = 1; i < total_states; i++) {
            int next_state = i ^ (i >> 1);
            int diff = state ^ next_state;
            int bit = __builtin_ctz(diff);
            string move;
            if (state & (1 << bit)) {
                move = "-" + to_string(bit + 1);
            } else {
                move = "+" + to_string(bit + 1);
            }
            moves.push_back(move);
            state = next_state;
        }
        if (n > 0) {
            int bit = __builtin_ctz(state);
            moves.push_back("-" + to_string(bit + 1));
        }
        for (int i = 0; i < moves.size(); i++) {
            if (i > 0) cout << " ";
            cout << moves[i];
        }
        cout << endl;
        return 0;
    }

    int num_non_empty = 0;
    for (int size = 1; size <= k; size++) {
        if (size <= n) {
            num_non_empty += comb(n, size);
        }
    }

    vector<bool> visited(1 << n, false);
    vector<string> path;
    visited[0] = true;

    if (dfs(0, visited, path)) {
        for (int i = 0; i < final_path.size(); i++) {
            if (i > 0) cout << " ";
            cout << final_path[i];
        }
        cout << endl;
    } else {
        cout << 0 << endl;
    }

    return 0;
}

int comb(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    int res = 1;
    for (int i = 0; i < k; i++) {
        res *= (n - i);
        res /= (i + 1);
    }
    return res;
}
