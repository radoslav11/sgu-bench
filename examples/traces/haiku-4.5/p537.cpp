#include<bits/stdc++.h>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

vector<long long> getDivisors(long long n) {
    vector<long long> divisors;
    for (long long i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            divisors.push_back(i);
            if (i != n / i) {
                divisors.push_back(n / i);
            }
        }
    }
    sort(divisors.begin(), divisors.end());
    return divisors;
}

long long stringToNumber(const string& s, const map<char, int>& mapping) {
    long long num = 0;
    for (char c : s) {
        num = num * 10 + mapping.at(c);
    }
    return num;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int t;
    cin >> t;
    
    for (int tc = 1; tc <= t; tc++) {
        string pattern;
        cin >> pattern;
        
        // Get unique characters
        set<char> uniqueChars(pattern.begin(), pattern.end());
        vector<char> chars(uniqueChars.begin(), uniqueChars.end());
        
        // Generate all permutations of digits for these characters
        vector<int> digits;
        for (int i = 0; i < 10; i++) {
            digits.push_back(i);
        }
        
        long long g = 0;
        bool first = true;
        
        // Try all permutations of digits
        sort(digits.begin(), digits.end());
        do {
            // Check if first character maps to 0
            if (digits[0] == 0) continue;
            
            // Create mapping
            map<char, int> mapping;
            for (int i = 0; i < chars.size(); i++) {
                mapping[chars[i]] = digits[i];
            }
            
            // Convert string to number
            long long num = stringToNumber(pattern, mapping);
            
            if (first) {
                g = num;
                first = false;
            } else {
                g = gcd(g, num);
            }
        } while (next_permutation(digits.begin(), digits.end()) && digits.size() >= chars.size());
        
        // Better approach: generate permutations of chars.size() digits from 0-9
        vector<int> perm(chars.size());
        for (int i = 0; i < chars.size(); i++) {
            perm[i] = i;
        }
        
        g = 0;
        first = true;
        
        function<void(int, vector<bool>&)> generateMappings = [&](int idx, vector<bool>& used) {
            if (idx == chars.size()) {
                // Create mapping
                map<char, int> mapping;
                for (int i = 0; i < chars.size(); i++) {
                    mapping[chars[i]] = perm[i];
                }
                
                // Check no leading zero
                if (mapping[pattern[0]] == 0) return;
                
                long long num = stringToNumber(pattern, mapping);
                
                if (first) {
                    g = num;
                    first = false;
                } else {
                    g = gcd(g, num);
                }
                return;
            }
            
            for (int digit = 0; digit < 10; digit++) {
                if (!used[digit]) {
                    used[digit] = true;
                    perm[idx] = digit;
                    generateMappings(idx + 1, used);
                    used[digit] = false;
                }
            }
        };
        
        vector<bool> used(10, false);
        generateMappings(0, used);
        
        vector<long long> divisors = getDivisors(g);
        
        cout << "Case " << tc << ":";
        for (long long d : divisors) {
            cout << " " << d;
        }
        cout << "\n";
    }
    
    return 0;
}
