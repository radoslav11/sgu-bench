#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    // Optimize standard I/O operations for performance
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string T, S;
    if (!(cin >> T >> S)) return 0;

    int N = T.length();
    int M = S.length();

    // next_pos[i * 26 + c] will store the smallest index j >= i such that T[j] == 'a' + c.
    // If no such index exists, it stores N.
    // Using a flattened 1D vector for better cache locality and performance.
    vector<int> next_pos((N + 1) * 26, N);
    
    for (int i = N - 1; i >= 0; --i) {
        for (int c = 0; c < 26; ++c) {
            next_pos[i * 26 + c] = next_pos[(i + 1) * 26 + c];
        }
        next_pos[i * 26 + (T[i] - 'a')] = i;
    }

    long long ans = 0;
    
    // For each possible starting index L of the remaining text
    for (int L = 0; L < N; ++L) {
        int curr = L;
        bool possible = true;
        
        // Greedily find the earliest end index `curr` such that S is a subsequence of T[L...curr]
        for (int k = 0; k < M; ++k) {
            curr = next_pos[curr * 26 + (S[k] - 'a')];
            if (curr == N) {
                possible = false;
                break;
            }
        }
        
        // If S is a subsequence, any end index from `curr` to `N - 1` is valid.
        // The number of such valid end indices is (N - curr).
        if (possible) {
            ans += (N - curr);
        }
    }

    cout << ans << "\n";

    return 0;
}
