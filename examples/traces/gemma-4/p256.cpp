#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int M, N;
int A[10], B[10];
int pow5[11];
unsigned char dp[9765625];
unsigned char next_dp[9765625];
bool visited[9765625];

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    if (!(cin >> M >> N)) return 0;
    for (int i = 0; i < N; ++i) cin >> A[i] >> B[i];

    pow5[0] = 1;
    for (int i = 1; i <= 10; ++i) pow5[i] = pow5[i - 1] * 5;

    vector<int> current_states;
    current_states.push_back(0);
    dp[0] = 0;

    for (int t = 1; t <= 500; ++t) {
        vector<int> next_states;
        
        for (int state : current_states) {
            int current_balloons = (int)dp[state];

            int next_state_none = 0;
            for (int i = 0; i < N; ++i) {
                int r = (state / pow5[i]) % 5;
                int next_r = (r > 0) ? r - 1 : 0;
                next_state_none += next_r * pow5[i];
            }
            if (next_dp[next_state_none] < (unsigned char)current_balloons) {
                if (!visited[next_state_none]) {
                    visited[next_state_none] = true;
                    next_states.push_back(next_state_none);
                }
                next_dp[next_state_none] = (unsigned char)current_balloons;
            }

            for (int i = 0; i < N; ++i) {
                if ((state / pow5[i]) % 5 == 0) {
                    int next_state_i = 0;
                    for (int j = 0; j < N; ++j) {
                        int r_j = (state / pow5[j]) % 5;
                        int next_r = (i == j) ? B[j] : ((r_j > 0) ? r_j - 1 : 0);
                        next_state_i += next_r * pow5[j];
                    }
                    int next_balloons = current_balloons + A[i];
                    if (next_dp[next_state_i] < (unsigned char)next_balloons) {
                        if (!visited[next_state_i]) {
                            visited[next_state_i] = true;
                            next_states.push_back(next_state_i);
                        }
                        next_dp[next_state_i] = (unsigned char)next_balloons;
                    }
                }
            }
        }

        bool found = false;
        for (int ns : next_states) {
            if (next_dp[ns] >= M) {
                cout << t << endl;
                found = true;
                break;
            }
        }
        if (found) return 0;

        current_states.clear();
        for (int ns : next_states) {
            dp[ns] = next_dp[ns];
            next_dp[ns] = 0;
            visited[ns] = false;
            current_states.push_back(ns);
        }
    }

    return 0;
}

