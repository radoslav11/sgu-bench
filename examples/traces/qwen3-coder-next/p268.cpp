#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

bool is_hyper_almost_permutative(const std::string& s, int N) {
    int len = s.length();
    if (len < N + 1) return false;
    
    for (int i = 0; i <= len - N - 1; i++) {
        std::vector<bool> seen(26, false);
        int distinct = 0;
        for (int j = i; j < i + N + 1; j++) {
            int c = s[j] - 'A';
            if (seen[c]) {
                // already seen this character, but we need exactly one repeat in N+1 chars
                // if more than one repeat, it's invalid
                distinct--;
                // This window has a duplicate, but we need to check if exactly N distinct
                // Let's just count distinct properly
            }
            seen[c] = true;
        }
        // Count distinct characters
        distinct = 0;
        for (int j = 0; j < 26; j++) {
            if (seen[j]) distinct++;
        }
        if (distinct != N) {
            return false;
        }
    }
    return true;
}

int main() {
    int N;
    std::cin >> N;
    std::string S1, S2;
    std::cin >> S1;
    std::cin >> S2;
    
    // Try all possible overlaps from largest to smallest
    for (int overlap = N - 1; overlap >= 0; overlap--) {
        std::string candidate = S1 + S2.substr(overlap);
        if (candidate.length() >= N && is_hyper_almost_permutative(candidate, N)) {
            std::cout << candidate << std::endl;
            return 0;
        }
    }
    
    // Fallback: just concatenate (should always work by problem constraints)
    std::cout << S1 + S2 << std::endl;
    return 0;
}
