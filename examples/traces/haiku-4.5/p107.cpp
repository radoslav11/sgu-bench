#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long N;
    cin >> N;
    
    // The target number
    long long target = 987654321LL;
    
    // We need to find solutions to x^2 ≡ target (mod 10^k)
    // Start with small moduli and use Hensel lifting
    
    vector<long long> solutions;
    long long mod = 1;
    
    // Build up solutions using Hensel lifting
    for(int k = 1; k <= 9 && k <= N; k++){
        mod *= 10;
        vector<long long> next_solutions;
        
        if(k == 1){
            // Find solutions mod 10
            long long target_mod = target % 10;
            for(long long x = 0; x < 10; x++){
                if((x * x) % 10 == target_mod){
                    next_solutions.push_back(x);
                }
            }
        } else {
            // Hensel lifting
            long long prev_mod = mod / 10;
            long long target_mod = target % mod;
            
            for(long long prev_sol : solutions){
                for(long long d = 0; d < 10; d++){
                    long long candidate = prev_sol + d * prev_mod;
                    if((candidate * candidate) % mod == target_mod){
                        next_solutions.push_back(candidate);
                    }
                }
            }
        }
        
        solutions = next_solutions;
        
        if(solutions.empty()) break;
    }
    
    if(solutions.empty()){
        cout << 0 << endl;
        return 0;
    }
    
    // Now we have solutions modulo 10^min(9, N)
    // Count how many N-digit numbers satisfy the condition
    
    long long count = 0;
    long long mod_final = 1;
    for(int i = 0; i < min(9LL, N); i++) mod_final *= 10;
    
    // For N-digit numbers: 10^(N-1) <= x < 10^N
    long long lower = 1;
    for(int i = 1; i < N; i++) lower *= 10;
    long long upper = lower * 10;
    
    if(N <= 9){
        // Direct count
        for(long long sol : solutions){
            if(sol >= lower && sol < upper){
                count++;
            }
        }
    } else {
        // For larger N, we need to count within the range
        // Numbers with the same last (min(9,N)) digits repeat
        long long period = mod_final;
        
        // Find the smallest and largest valid number
        long long first_valid = -1;
        for(long long sol : solutions){
            long long candidate = sol;
            while(candidate < lower) candidate += period;
            if(candidate < upper){
                first_valid = candidate;
                break;
            }
        }
        
        if(first_valid == -1){
            cout << 0 << endl;
        } else {
            // Count all solutions in range with this period
            count = (upper - first_valid) / period;
            long long remainder = (upper - first_valid) % period;
            if(remainder > 0) count++;
        }
    }
    
    cout << count << endl;
    
    return 0;
}
