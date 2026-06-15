#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cassert>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string alphabet;
    getline(cin, alphabet);
    
    string lambda, mu;
    getline(cin, lambda);
    getline(cin, mu);

    // Remove any trailing carriage return if present (for Windows-style line endings)
    if (!lambda.empty() && lambda.back() == '\r') lambda.pop_back();
    if (!mu.empty() && mu.back() == '\r') mu.pop_back();

    // Create mapping from char to index in alphabet
    vector<int> charToIndex(256, -1);
    for (int i = 0; i < (int)alphabet.size(); ++i) {
        charToIndex[(unsigned char)alphabet[i]] = i;
    }

    int n = alphabet.size();
    vector<vector<int>> d(n, vector<int>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> d[i][j];
        }
    }

    int len1 = lambda.size();
    int len2 = mu.size();

    // dp[i][j] = minimal dissimilarity to match lambda[0:i] with mu[0:j]
    // where we have processed i characters from lambda and j characters from mu
    // but we may have added extra characters before or between matched ones
    vector<vector<long long>> dp(len1 + 1, vector<long long>(len2 + 1, LLONG_MAX));
    // back[i][j] = pair of moves: 
    // (0,0): matched lambda[i-1] with mu[j-1]
    // (1,0): added extra char before matching lambda[i-1] (match lambda[i-1] with some extra char)
    // (0,1): added extra char before matching mu[j-1] (match mu[j-1] with some extra char)
    // But actually, we need to track how we got there more precisely
    // Let's use a simpler DP: we allow inserting extra characters arbitrarily
    // At each step, we can either:
    //   - Match lambda[i] with mu[j]
    //   - Insert a character in alpha (before matching lambda[i]) and pair it with something
    //   - Insert a character in beta (before matching mu[j]) and pair it with something
    // But actually, more standard is to consider:
    // We are building strings alpha and beta of the same length
    // such that lambda is subsequence of alpha and mu is subsequence of beta
    // We can think ofAligning lambda and mu with possible gaps (insertions)
    // This is like edit distance but we want to minimize the sum of d(alpha[k], beta[k])
    
    // Alternative: we can use DP where:
    // dp[i][j] = minimal total dissimilarity to process first i chars of lambda and first j chars of mu
    // and the last operation was matching lambda[i-1] with mu[j-1] (if i>0 and j>0), or with an inserted char in the other string.
    
    // Actually, standard approach: 
    // We'll allow:
    // - Match lambda[i] with mu[j]
    // - Match lambda[i] with a dummy character (but we don't have dummy, we have to pick a char for beta)
    // - Match a dummy character with mu[j] (pick a char for alpha)
    // But since we can choose any characters, when matching lambda[i] with a character in beta that is not mu[j], 
    // we would choose the character in beta that minimizes d(lambda[i], char) -> but note we have to later match mu[j] with something.
    
    // Actually, better: think of building the common length string position by position.
    // At each position, we can:
    //   - If we still have characters left in lambda and mu, match lambda[i] with mu[j]
    //   - If we still have characters left in lambda, we can output lambda[i] in alpha and choose a char for beta that minimizes d(lambda[i], char) 
    //     but note: this char won't be used for mu later — so this is like inserting a char in beta that doesn't help match mu.
    //   - Similarly for mu[j] in beta and choose a char for alpha that minimizes d(char, mu[j]).
    
    // However, a more efficient approach is known from the problem: it's a variant of longest common subsequence but with costs.
    // Actually, we can use DP where:
    // dp[i][j] = minimum cost to match lambda[i:] and mu[j:]
    // Transitions:
    // 1. Match lambda[i] with mu[j]: cost = d(lambda[i], mu[j]) + dp[i+1][j+1]
    // 2. Skip lambda[i]: match lambda[i] with some char c in beta, and we don't use any char of mu here.
    //    But we are free to choose c to minimize d(lambda[i], c). So cost = min_c d(lambda[i], c) + dp[i+1][j]
    // 3. Skip mu[j]: match some char c in alpha with mu[j], cost = min_c d(c, mu[j]) + dp[i][j+1]
    
    // But note: we are allowed to skip both? Actually, we can skip both by doing two separate skips? 
    // However, skipping both would be covered by skip lambda then skip mu, or vice versa.
    
    // However, the problem: the strings alpha and beta must be of the same length. So every position must have one char in alpha and one in beta.
    // Therefore, the above transitions are valid: at each step, we are adding one character to alpha and one to beta.
    // So:
    //   - If we skip lambda[i] (meaning we don't advance in lambda yet), we output alpha += lambda[i] and beta += some optimal char c for d(lambda[i], c)
    //   - Similarly for mu[j]
    //   - Or we match them: output alpha += lambda[i], beta += mu[j]
    
    // So:
    // dp[i][j] = min(
    //     d(charToIndex[lambda[i]], charToIndex[mu[j]]) + dp[i+1][j+1],   // if i < len1 and j < len2
    //     min_c d(charToIndex[lambda[i]], c) + dp[i+1][j],                // skip mu: match lambda[i] with a char that minimizes cost for this position, but mu[j] remains
    //     min_c d(c, charToIndex[mu[j]]) + dp[i][j+1]                     // skip lambda
    // )
    
    // We can precompute:
    //   minD_from[i] = min_c d(i, c)   for each i in [0, n-1]
    //   minD_to[j] = min_c d(c, j)      for each j in [0, n-1]

    vector<int> minD_from(n, INT_MAX);
    vector<int> minD_to(n, INT_MAX);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            minD_from[i] = min(minD_from[i], d[i][j]);
            minD_to[j] = min(minD_to[j], d[i][j]);
        }
    }

    // dp[i][j]: minimal cost for lambda[i:] and mu[j:]
    // We use 1-indexed for convenience? Actually, let's do 0-indexed from i=len1, j=len2 backwards.
    dp[len1][len2] = 0;
    for (int i = len1 - 1; i >= 0; --i) {
        dp[i][len2] = dp[i+1][len2] + minD_from[charToIndex[(unsigned char)lambda[i]]];
    }
    for (int j = len2 - 1; j >= 0; --j) {
        dp[len1][j] = dp[len1][j+1] + minD_to[charToIndex[(unsigned char)mu[j]]];
    }

    for (int i = len1 - 1; i >= 0; --i) {
        for (int j = len2 - 1; j >= 0; --j) {
            long long option1 = LLONG_MAX;
            long long option2 = LLONG_MAX;
            long long option3 = LLONG_MAX;
            
            // Option 1: match lambda[i] and mu[j]
            int idx1 = charToIndex[(unsigned char)lambda[i]];
            int idx2 = charToIndex[(unsigned char)mu[j]];
            if (dp[i+1][j+1] != LLONG_MAX) {
                option1 = (long long)d[idx1][idx2] + dp[i+1][j+1];
            }
            
            // Option 2: skip mu[j] (i.e., match lambda[i] with a char that minimizes cost, and mu[j] remains for later)
            if (dp[i+1][j] != LLONG_MAX) {
                option2 = (long long)minD_from[idx1] + dp[i+1][j];
            }
            
            // Option 3: skip lambda[i] (match mu[j] with a char that minimizes cost, lambda[i] remains for later)
            if (dp[i][j+1] != LLONG_MAX) {
                option3 = (long long)minD_to[idx2] + dp[i][j+1];
            }
            
            dp[i][j] = min({option1, option2, option3});
        }
    }

    long long ans = dp[0][0];
    
    // Now reconstruct the strings alpha and beta
    string alpha_str, beta_str;
    int i = 0, j = 0;
    while (i < len1 || j < len2) {
        if (i == len1) {
            // Only mu left: for each remaining mu[j], we skip lambda: match mu[j] with a char that minimizes d(c, mu[j])
            int idx2 = charToIndex[(unsigned char)mu[j]];
            int best_char_idx = -1;
            int min_cost = INT_MAX;
            for (int c = 0; c < n; ++c) {
                if (d[c][idx2] < min_cost) {
                    min_cost = d[c][idx2];
                    best_char_idx = c;
                }
            }
            alpha_str += alphabet[best_char_idx];
            beta_str += mu[j];
            ++j;
        } else if (j == len2) {
            // Only lambda left: skip mu: match lambda[i] with best char for d(lambda[i], c)
            int idx1 = charToIndex[(unsigned char)lambda[i]];
            int best_char_idx = -1;
            int min_cost = INT_MAX;
            for (int c = 0; c < n; ++c) {
                if (d[idx1][c] < min_cost) {
                    min_cost = d[idx1][c];
                    best_char_idx = c;
                }
            }
            alpha_str += lambda[i];
            beta_str += alphabet[best_char_idx];
            ++i;
        } else {
            int idx1 = charToIndex[(unsigned char)lambda[i]];
            int idx2 = charToIndex[(unsigned char)mu[j]];
            
            long long option1 = (long long)d[idx1][idx2] + dp[i+1][j+1];
            long long option2 = (long long)minD_from[idx1] + dp[i+1][j];
            long long option3 = (long long)minD_to[idx2] + dp[i][j+1];
            
            if (option1 <= option2 && option1 <= option3) {
                alpha_str += lambda[i];
                beta_str += mu[j];
                ++i; ++j;
            } else if (option2 <= option1 && option2 <= option3) {
                alpha_str += lambda[i];
                // Find best char for beta at this position to achieve minD_from[idx1]
                int best_char_idx = -1;
                int min_cost = INT_MAX;
                for (int c = 0; c < n; ++c) {
                    if (d[idx1][c] == minD_from[idx1]) {
                        best_char_idx = c;
                        break; // any one is fine
                    }
                }
                beta_str += alphabet[best_char_idx];
                ++i;
            } else {
                // option3 is chosen
                // Find best char for alpha at this position to achieve minD_to[idx2]
                int best_char_idx = -1;
                int min_cost = INT_MAX;
                for (int c = 0; c < n; ++c) {
                    if (d[c][idx2] == minD_to[idx2]) {
                        best_char_idx = c;
                        break;
                    }
                }
                alpha_str += alphabet[best_char_idx];
                beta_str += mu[j];
                ++j;
            }
        }
    }

    cout << ans << "\n";
    cout << alpha_str << "\n";
    cout << beta_str << "\n";

    return 0;
}
