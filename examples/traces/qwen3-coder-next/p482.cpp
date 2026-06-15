#include <iostream>
#include <vector>
#include <cstring>
#include <stack>
#include <algorithm>
#include <climits>

using namespace std;

const int MAXN = 50;
const int MAXS = 5000;
const int INF = 1e9;

short dp[MAXN][MAXN][MAXS+1];
char cnt[MAXN][MAXN][MAXS+1];
char par[MAXN][MAXN][MAXS+1];

int main() {
    int n;
    cin >> n;
    vector<int> h(n);
    for (int i = 0; i < n; i++) {
        cin >> h[i];
    }

    int total_initial = 0;
    for (int i = 0; i < n; i++) {
        total_initial += h[i];
    }

    int ini = 2 * n;
    for (int i = 0; i < n-1; i++) {
        ini += abs(h[i] - h[i+1]);
    }
    ini += h[0] + h[n-1];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int s = 0; s <= MAXS; s++) {
                dp[i][j][s] = INF;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        dp[i][i][0] = h[i];
        cnt[i][i][0] = 1;
        par[i][i][0] = -1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i+1; j < n; j++) {
            for (int k = i; k < j; k++) {
                for (int s = 0; s <= MAXS; s++) {
                    if (dp[i][k][s] == INF) continue;
                    int ns = s + abs(h[k] - h[j]);
                    if (ns > MAXS) continue;
                    int new_height = dp[i][k][s] + h[j];
                    if (new_height < dp[i][j][ns]) {
                        dp[i][j][ns] = new_height;
                        cnt[i][j][ns] = cnt[i][k][s] + 1;
                        par[i][j][ns] = k;
                    }
                }
            }
        }
    }

    int best_remaining = INF;
    int best_i = -1, best_j = -1, best_s = -1;

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            for (int s = 0; s <= MAXS; s++) {
                if (dp[i][j][s] == INF) continue;
                int boards = cnt[i][j][s];
                int perimeter = 2 * boards + h[i] + h[j] + s;
                if (2 * perimeter >= ini) {
                    if (dp[i][j][s] < best_remaining) {
                        best_remaining = dp[i][j][s];
                        best_i = i;
                        best_j = j;
                        best_s = s;
                    }
                }
            }
        }
    }

    if (best_remaining == INF) {
        for (int i = 0; i < n; i++) {
            if (dp[i][i][0] < best_remaining) {
                best_remaining = dp[i][i][0];
                best_i = i;
                best_j = i;
                best_s = 0;
            }
        }
    }

    vector<int> remaining_indices;
    int cur_i = best_i, cur_j = best_j, cur_s = best_s;
    stack<int> st;
    while (true) {
        st.push(cur_j);
        int k = par[cur_i][cur_j][cur_s];
        if (k == -1) {
            break;
        }
        int ns = cur_s - abs(h[k] - h[cur_j]);
        cur_j = k;
        cur_s = ns;
    }

    while (!st.empty()) {
        remaining_indices.push_back(st.top());
        st.pop();
    }

    vector<bool> removed(n, false);
    for (int idx : remaining_indices) {
        removed[idx] = true;
    }

    vector<int> extracted_indices;
    for (int i = 0; i < n; i++) {
        if (!removed[i]) {
            extracted_indices.push_back(i);
        }
    }

    int extracted_height = total_initial - best_remaining;
    cout << extracted_height << endl;
    cout << extracted_indices.size() << endl;
    for (int i = 0; i < extracted_indices.size(); i++) {
        if (i > 0) cout << " ";
        cout << extracted_indices[i] + 1;
    }
    if (extracted_indices.size() > 0) cout << endl;

    return 0;
}
