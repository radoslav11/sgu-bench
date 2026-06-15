#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int N, K;
    cin >> N >> K;
    
    vector<int> queries(K);
    for(int i = 0; i < K; i++) cin >> queries[i];
    
    // Find all self-numbers up to N
    // A number is NOT a self-number if it has a generator
    // d(n) = n + sum_of_digits(n)
    // For n in [1..N], d(n) can be at most N + 9*7 = N + 63 (for N up to 10^7)
    
    vector<bool> has_generator(N+1, false);
    
    // For each n from 1 to N, mark d(n) as having a generator
    // But d(n) might exceed N, so we only mark if d(n) <= N
    
    for(int n = 1; n <= N; n++){
        int s = n;
        int temp = n;
        while(temp > 0){
            s += temp % 10;
            temp /= 10;
        }
        if(s <= N){
            has_generator[s] = true;
        }
    }
    
    // Collect self-numbers
    vector<int> self_numbers;
    self_numbers.reserve(200000);
    for(int i = 1; i <= N; i++){
        if(!has_generator[i]){
            self_numbers.push_back(i);
        }
    }
    
    int count = self_numbers.size();
    cout << count << "\n";
    
    for(int i = 0; i < K; i++){
        int idx = queries[i] - 1; // 0-indexed
        if(i > 0) cout << " ";
        cout << self_numbers[idx];
    }
    cout << "\n";
    
    return 0;
}
