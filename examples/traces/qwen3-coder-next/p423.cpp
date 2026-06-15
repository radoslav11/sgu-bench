#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
using namespace std;

int main() {
    int n, s0, t0;
    cin >> n >> s0 >> t0;
    s0--; t0--;

    vector<string> adj_str(n);
    for (int i = 0; i < n; i++) {
        cin >> adj_str[i];
    }

    vector<int> pop(n);
    for (int i = 0; i < n; i++) {
        cin >> pop[i];
    }

    vector<int> pow3(n);
    pow3[0] = 1;
    for (int i = 1; i < n; i++) {
        pow3[i] = pow3[i-1] * 3;
    }

    int total_states = 1;
    for (int i = 0; i < n; i++) {
        total_states *= 3;
    }

    vector<vector<int>> states_by_independent(n+1);
    for (int s = 0; s < total_states; s++) {
        int count = 0;
        int temp = s;
        for (int i = 0; i < n; i++) {
            if (temp % 3 == 0) {
                count++;
            }
            temp /= 3;
        }
        states_by_independent[count].push_back(s);
    }

    vector<vector<int>> dp(total_states, vector<int>(2, 0));

    vector<vector<bool>> adj(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (adj_str[i][j] == '1') {
                adj[i][j] = true;
            }
        }
    }

    for (int count = 0; count <= n; count++) {
        for (int s : states_by_independent[count]) {
            int popA = 0, popB = 0;
            int temp = s;
            for (int i = 0; i < n; i++) {
                int d = temp % 3;
                if (d == 1) {
                    popA += pop[i];
                } else if (d == 2) {
                    popB += pop[i];
                }
                temp /= 3;
            }
            int diff = popA - popB;

            if (count == 0) {
                dp[s][0] = diff;
                dp[s][1] = diff;
            } else {
                vector<int> state_of_city(n);
                temp = s;
                for (int i = 0; i < n; i++) {
                    state_of_city[i] = temp % 3;
                    temp /= 3;
                }

                vector<int> N_list0;
                for (int i = 0; i < n; i++) {
                    if (state_of_city[i] == 1) {
                        for (int j = 0; j < n; j++) {
                            if (adj[i][j] && state_of_city[j] == 0) {
                                N_list0.push_back(j);
                            }
                        }
                    }
                }
                sort(N_list0.begin(), N_list0.end());
                auto last = unique(N_list0.begin(), N_list0.end());
                N_list0.erase(last, N_list0.end());
                int k0 = N_list0.size();

                vector<int> N_list1;
                for (int i = 0; i < n; i++) {
                    if (state_of_city[i] == 2) {
                        for (int j = 0; j < n; j++) {
                            if (adj[i][j] && state_of_city[j] == 0) {
                                N_list1.push_back(j);
                            }
                        }
                    }
                }
                sort(N_list1.begin(), N_list1.end());
                last = unique(N_list1.begin(), N_list1.end());
                N_list1.erase(last, N_list1.end());
                int k1 = N_list1.size();

                if (k0 == 0) {
                    dp[s][0] = diff;
                } else {
                    vector<int> sum_arr0(1 << k0, 0);
                    vector<int> delta0(1 << k0, 0);
                    for (int m = 1; m < (1 << k0); m++) {
                        int bit = __builtin_ctz(m);
                        int prev = m ^ (1 << bit);
                        sum_arr0[m] = sum_arr0[prev] + pop[N_list0[bit]];
                        delta0[m] = delta0[prev] + pow3[N_list0[bit]];
                    }
                    int total_sum0 = sum_arr0[(1 << k0) - 1];
                    int best0 = INT_MIN;
                    for (int m = 0; m < (1 << k0); m++) {
                        if (2 * sum_arr0[m] >= total_sum0) {
                            int next_state = s + delta0[m];
                            if (dp[next_state][1] > best0) {
                                best0 = dp[next_state][1];
                            }
                        }
                    }
                    dp[s][0] = best0;
                }

                if (k1 == 0) {
                    dp[s][1] = diff;
                } else {
                    vector<int> sum_arr1(1 << k1, 0);
                    vector<int> delta1(1 << k1, 0);
                    for (int m = 1; m < (1 << k1); m++) {
                        int bit = __builtin_ctz(m);
                        int prev = m ^ (1 << bit);
                        sum_arr1[m] = sum_arr1[prev] + pop[N_list1[bit]];
                        delta1[m] = delta1[prev] + pow3[N_list1[bit]];
                    }
                    int total_sum1 = sum_arr1[(1 << k1) - 1];
                    int best1 = INT_MAX;
                    for (int m = 0; m < (1 << k1); m++) {
                        if (2 * sum_arr1[m] >= total_sum1) {
                            int next_state = s + delta1[m];
                            if (dp[next_state][0] < best1) {
                                best1 = dp[next_state][0];
                            }
                        }
                    }
                    dp[s][1] = best1;
                }
            }
        }
    }

    int start_state = pow3[s0] + 2 * pow3[t0];
    cout << dp[start_state][0] << endl;

    return 0;
}
