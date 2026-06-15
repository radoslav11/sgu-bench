#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

struct Item {
    int a, b, id;
};

struct Group {
    int S, D;
    vector<int> indices;
};

struct Checkpoint {
    int group_idx;
    short dp[21][2001];
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, K;
    if (!(cin >> N >> K)) return 0;

    vector<int> orig_A(N + 1), orig_B(N + 1);
    vector<Item> items(N);
    for (int i = 1; i <= N; ++i) {
        cin >> orig_A[i] >> orig_B[i];
        items[i - 1] = {orig_A[i], orig_B[i], i};
    }

    sort(items.begin(), items.end(), [](const Item& x, const Item& y) {
        if (x.a != y.a) return x.a < y.a;
        return x.b < y.b;
    });

    vector<Group> groups;
    for (int i = 0; i < N; ) {
        int j = i;
        Group g;
        g.S = items[i].a + items[i].b;
        g.D = items[i].a - items[i].b;
        while (j < N && items[j].a == items[i].a && items[j].b == items[i].b) {
            if ((int)g.indices.size() < K) {
                g.indices.push_back(items[j].id);
            }
            j++;
        }
        groups.push_back(g);
        i = j;
    }

    int M = groups.size();
    int B = 100;
    vector<Checkpoint> checkpoints;

    short dp[21][2001];
    memset(dp, -1, sizeof(dp));
    dp[0][1000] = 0;

    for (int g = 0; g < M; ++g) {
        short next_dp[21][2001];
        memcpy(next_dp, dp, sizeof(dp));
        
        int S_g = groups[g].S;
        int D_g = groups[g].D;
        int C_g = groups[g].indices.size();
        
        for (int k = 0; k <= K; ++k) {
            int cur_min_d = max(0, 1000 - 50 * k);
            int cur_max_d = min(2000, 1000 + 50 * k);
            for (int d = cur_min_d; d <= cur_max_d; ++d) {
                int val = dp[k][d];
                if (val == -1) continue;
                int max_c = min(C_g, K - k);
                int nk = k + 1;
                int nd = d + D_g;
                int nval = val + S_g;
                for (int c = 1; c <= max_c; ++c) {
                    if (nd >= 0 && nd <= 2000) {
                        if (nval > next_dp[nk][nd]) {
                            next_dp[nk][nd] = nval;
                        }
                    }
                    nk++;
                    nd += D_g;
                    nval += S_g;
                }
            }
        }
        memcpy(dp, next_dp, sizeof(dp));
        
        if (g % B == B - 1 || g == M - 1) {
            Checkpoint cp;
            cp.group_idx = g;
            memcpy(cp.dp, dp, sizeof(dp));
            checkpoints.push_back(cp);
        }
    }

    int current_k = K;
    int current_d = -1;
    int max_S = -1;

    for (int d = 0; d <= 2000; ++d) {
        if (dp[K][d] != -1) {
            int diff = abs(d - 1000);
            int S_val = dp[K][d];
            if (current_d == -1) {
                current_d = d;
                max_S = S_val;
            } else {
                int best_diff = abs(current_d - 1000);
                if (diff < best_diff) {
                    current_d = d;
                    max_S = S_val;
                } else if (diff == best_diff) {
                    if (S_val > max_S) {
                        current_d = d;
                        max_S = S_val;
                    }
                }
            }
        }
    }

    vector<int> chosen_indices;
    chosen_indices.reserve(K);

    int R = M - 1;
    vector<uint8_t> choice;
    choice.reserve(100 * 21 * 2001);

    for (int cp_idx = (int)checkpoints.size() - 1; cp_idx >= 0; --cp_idx) {
        int L = (cp_idx == 0) ? 0 : checkpoints[cp_idx - 1].group_idx + 1;
        int num_steps = R - L + 1;
        
        choice.assign(num_steps * 21 * 2001, 0);
        short cur_dp[21][2001];
        
        if (cp_idx == 0) {
            memset(cur_dp, -1, sizeof(cur_dp));
            cur_dp[0][1000] = 0;
        } else {
            memcpy(cur_dp, checkpoints[cp_idx - 1].dp, sizeof(cur_dp));
        }
        
        for (int step = 0; step < num_steps; ++step) {
            int g = L + step;
            short next_dp[21][2001];
            memcpy(next_dp, cur_dp, sizeof(cur_dp));
            
            int S_g = groups[g].S;
            int D_g = groups[g].D;
            int C_g = groups[g].indices.size();
            
            for (int k = 0; k <= K; ++k) {
                int cur_min_d = max(0, 1000 - 50 * k);
                int cur_max_d = min(2000, 1000 + 50 * k);
                for (int d = cur_min_d; d <= cur_max_d; ++d) {
                    int val = cur_dp[k][d];
                    if (val == -1) continue;
                    int max_c = min(C_g, K - k);
                    int nk = k + 1;
                    int nd = d + D_g;
                    int nval = val + S_g;
                    for (int c = 1; c <= max_c; ++c) {
                        if (nd >= 0 && nd <= 2000) {
                            if (nval > next_dp[nk][nd]) {
                                next_dp[nk][nd] = nval;
                                choice[(step * 21 + nk) * 2001 + nd] = c;
                            }
                        }
                        nk++;
                        nd += D_g;
                        nval += S_g;
                    }
                }
            }
            memcpy(cur_dp, next_dp, sizeof(cur_dp));
        }
        
        for (int step = num_steps - 1; step >= 0; --step) {
            int c = choice[(step * 21 + current_k) * 2001 + current_d];
            int g = L + step;
            for (int i = 0; i < c; ++i) {
                chosen_indices.push_back(groups[g].indices[i]);
            }
            current_k -= c;
            current_d -= c * groups[g].D;
        }
        
        R = L - 1;
    }

    sort(chosen_indices.begin(), chosen_indices.end());

    int final_A = 0, final_B = 0;
    for (int id : chosen_indices) {
        final_A += orig_A[id];
        final_B += orig_B[id];
    }

    cout << final_A << " " << final_B << "\n";
    for (int i = 0; i < K; ++i) {
        cout << chosen_indices[i] << (i == K - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}
