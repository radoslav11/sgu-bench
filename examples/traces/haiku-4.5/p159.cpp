#include<bits/stdc++.h>
using namespace std;

string toBase(long long num, int base) {
    if (num == 0) return "0";
    string result = "";
    while (num > 0) {
        int digit = num % base;
        if (digit < 10) {
            result = char('0' + digit) + result;
        } else {
            result = char('A' + digit - 10) + result;
        }
        num /= base;
    }
    return result;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int b, n;
    cin >> b >> n;
    
    vector<long long> solutions;
    
    // Find initial solutions modulo b
    vector<long long> current;
    for (long long x = 0; x < b; x++) {
        long long sq = (x * x) % b;
        if (sq == x % b) {
            current.push_back(x);
        }
    }
    
    // Lift solutions from modulo b^k to modulo b^(k+1)
    long long modulus = b;
    for (int k = 1; k < n; k++) {
        vector<long long> next;
        long long new_modulus = modulus * b;
        
        for (long long x : current) {
            // Try x + j * modulus for j = 0 to b-1
            for (int j = 0; j < b; j++) {
                long long candidate = x + (long long)j * modulus;
                long long sq = (candidate * candidate) % new_modulus;
                if (sq == candidate % new_modulus) {
                    next.push_back(candidate);
                }
            }
        }
        
        current = next;
        modulus = new_modulus;
    }
    
    // Filter for n-digit numbers
    long long min_val = 1;
    for (int i = 1; i < n; i++) min_val *= b;
    long long max_val = min_val * b;
    
    for (long long x : current) {
        if (x >= min_val && x < max_val) {
            solutions.push_back(x);
        }
    }
    
    sort(solutions.begin(), solutions.end());
    
    cout << solutions.size() << "\n";
    for (long long sol : solutions) {
        cout << toBase(sol, b) << "\n";
    }
    
    return 0;
}
