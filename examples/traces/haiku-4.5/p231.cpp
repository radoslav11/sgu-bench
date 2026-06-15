#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N;
    cin >> N;
    
    // Sieve of Eratosthenes
    vector<bool> is_prime(N + 1, true);
    is_prime[0] = is_prime[1] = false;
    
    for(int i = 2; i * i <= N; i++){
        if(is_prime[i]){
            for(int j = i * i; j <= N; j += i){
                is_prime[j] = false;
            }
        }
    }
    
    // Collect all primes
    vector<int> primes;
    for(int i = 2; i <= N; i++){
        if(is_prime[i]){
            primes.push_back(i);
        }
    }
    
    // Find valid pairs
    vector<pair<int, int>> result;
    
    for(int i = 0; i < primes.size(); i++){
        for(int j = i; j < primes.size(); j++){
            int sum = primes[i] + primes[j];
            if(sum <= N && is_prime[sum]){
                result.push_back({primes[i], primes[j]});
            }
        }
    }
    
    cout << result.size() << "\n";
    for(auto& p : result){
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
