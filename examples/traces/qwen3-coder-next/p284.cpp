#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
    int N;
    cin >> N;
    string S;
    cin >> S;
    
    int m = S.length();
    
    // Precompute prefix function for S
    vector<int> pi(m, 0);
    if (m > 0) {
        for (int i = 1; i < m; i++) {
            int j = pi[i-1];
            while (j > 0 && S[i] != S[j]) {
                j = pi[j-1];
            }
            if (S[i] == S[j]) {
                j++;
            }
            pi[i] = j;
        }
    }
    
    // For non-terminals 1 to N: store cnt and end_state
    vector<vector<long long>> cnt(N+1);
    vector<vector<int>> end_state(N+1);
    
    // Read descriptions for non-terminals 1 to N
    for (int k = 1; k <= N; k++) {
        int L;
        cin >> L;
        vector<string> tokens;
        for (int i = 0; i < L; i++) {
            string token;
            cin >> token;
            tokens.push_back(token);
        }
        
        // Initialize for this non-terminal
        cnt[k] = vector<long long>(m, 0);
        end_state[k] = vector<int>(m, 0);
        
        if (L == 0) {
            // Empty description: for any start state s, count=0, end state = s
            for (int s = 0; s < m; s++) {
                cnt[k][s] = 0;
                end_state[k][s] = s;
            }
        } else {
            // For each start state s (0 to m-1)
            for (int s = 0; s < m; s++) {
                int state = s;
                long long count = 0;
                
                for (string& t : tokens) {
                    if (t == "a" || t == "b") {
                        // Process terminal character
                        int new_state = state;
                        // KMP: find longest prefix which is also suffix
                        while (new_state > 0 && t[0] != S[new_state]) {
                            new_state = pi[new_state-1];
                        }
                        if (t[0] == S[new_state]) {
                            new_state++;
                        }
                        // Check if we matched the whole pattern
                        int add = 0;
                        if (new_state == m) {
                            add = 1;
                            if (m > 0) {
                                new_state = pi[new_state-1];
                            }
                        }
                        count += add;
                        state = new_state;
                    } else {
                        // Non-terminal: convert token to integer
                        int j = stoi(t);
                        count += cnt[j][state];
                        state = end_state[j][state];
                    }
                }
                cnt[k][s] = count;
                end_state[k][s] = state;
            }
        }
    }
    
    // The final string is expand(N), starting from state 0
    if (m == 0) {
        cout << 0 << endl;
    } else {
        cout << cnt[N][0] << endl;
    }
    
    return 0;
}
