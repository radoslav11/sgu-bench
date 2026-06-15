#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cctype>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<string> words(n);
    for (int i = 0; i < n; i++) {
        cin >> words[i];
    }

    // Precompute overlap[i][j]: maximum overlap when appending word j after word i
    // i.e., max k such that words[i].substr(len_i - k) == words[j].substr(0, k)
    vector<vector<int>> overlap(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                // For same word, we can't use full overlap (would be duplicate), but we'll set to 0
                // Actually, in permutation we won't use same word twice, so doesn't matter
                continue;
            }
            int len_i = words[i].size();
            int len_j = words[j].size();
            int max_overlap = min(len_i, len_j);
            for (int k = max_overlap; k >= 0; k--) {
                if (k == 0) {
                    overlap[i][j] = 0;
                    break;
                }
                if (words[i].substr(len_i - k) == words[j].substr(0, k)) {
                    overlap[i][j] = k;
                    break;
                }
            }
        }
    }

    string best_palindrome = "";
    
    // Generate all permutations of words
    vector<int> perm(n);
    for (int i = 0; i < n; i++) {
        perm[i] = i;
    }

    do {
        // Build superstring for this permutation with maximum overlaps
        string super = words[perm[0]];
        for (int i = 1; i < n; i++) {
            int prev_idx = perm[i-1];
            int curr_idx = perm[i];
            int o = overlap[prev_idx][curr_idx];
            super += words[curr_idx].substr(o);
        }

        // Find the largest p such that super[p:] is a palindrome
        int len = super.size();
        int p = 0;
        for (int candidate_p = len - 1; candidate_p >= 0; candidate_p--) {
            bool is_palindrome = true;
            int left = candidate_p;
            int right = len - 1;
            while (left < right) {
                if (super[left] != super[right]) {
                    is_palindrome = false;
                    break;
                }
                left++;
                right--;
            }
            if (is_palindrome) {
                p = candidate_p;
                break;
            }
        }
        
        string palindrome = super + string(super.substr(0, p).rbegin(), super.substr(0, p).rend());
        
        if (best_palindrome.empty() || palindrome.size() < best_palindrome.size()) {
            best_palindrome = palindrome;
        }
    } while (next_permutation(perm.begin(), perm.end()));

    cout << best_palindrome << endl;

    return 0;
}
