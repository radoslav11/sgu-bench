#include <iostream>
#include <string>
#include <queue>
#include <unordered_set>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int N;
    std::cin >> N;
    std::string s;
    std::cin >> s;
    
    // We need to find the shortest string (made of 'a' and 'b') that is not a substring of s.
    // Approach: BFS by length. Start with strings of length 1, then 2, etc.
    // Since N can be up to 500,000, but the shortest missing string is expected to be very short (logarithmic in N),
    // BFS up to reasonable length (e.g., up to 20) should be efficient.
    
    // We'll check for missing strings of increasing length.
    // For length L, there are 2^L possible strings. We can generate all and check if any is missing.
    // But checking each string naively (O(N) per string) would be too slow.
    // Instead, we can use a sliding window to collect all substrings of length L in s, store them in a hash set.
    // Then iterate through all possible strings of length L and check which is not in the set.
    
    // Since the shortest missing string is expected to be small (in worst case, length ~ log2(N)+1),
    // we can try L = 1, 2, 3, ... until we find one.
    
    for (int len = 1; len <= 20; len++) {
        // Collect all substrings of length len from s
        std::unordered_set<std::string> substrings;
        if (len <= N) {
            for (int i = 0; i <= N - len; i++) {
                substrings.insert(s.substr(i, len));
            }
        }
        
        // Generate all possible strings of length len and check
        // We can use BFS or iterative generation. Since len is small, we can use bit manipulation.
        // Total strings: 2^len. For len=20, that's 1e6 which might be borderline in worst case,
        // but note: in practice, the missing string is usually very short (len is small, like 1-20, but rarely 20).
        // Actually, worst-case N=500,000, then the shortest missing string length is at most 20 because 2^20 > 500,000.
        // So worst-case len=20, 2^20 = 1,048,576 strings, which is acceptable in C++ if implemented carefully.
        
        bool found = false;
        std::string result;
        
        // Generate all strings of length 'len' in lexicographic order (a < b)
        for (int mask = 0; mask < (1 << len); mask++) {
            std::string candidate(len, 'a');
            for (int i = 0; i < len; i++) {
                // if the i-th bit is set, use 'b', else 'a'
                if (mask & (1 << (len - 1 - i))) {
                    candidate[i] = 'b';
                } else {
                    candidate[i] = 'a';
                }
            }
            
            if (substrings.find(candidate) == substrings.end()) {
                result = candidate;
                found = true;
                break;
            }
        }
        
        if (found) {
            std::cout << len << "\n";
            std::cout << result << "\n";
            return 0;
        }
    }
    
    // According to the problem, there must be a solution. 
    // In worst-case, the shortest missing string has length at most N+1 (since there are N - len + 1 substrings of length len, 
    // and when len = N+1, there are 0 substrings, so any string of length N+1 is missing).
    // But our loop goes up to 20, and since 2^20 > 500000, we should always find a solution by len=20.
    
    // Fallback (should not be reached):
    std::cout << N+1 << "\n";
    for (int i = 0; i <= N; i++) std::cout << 'a';
    std::cout << "\n";
    
    return 0;
}
