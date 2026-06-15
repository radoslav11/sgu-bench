#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    cin >> N;
    
    vector<bool> is_prime(N+1, true);
    if(N >= 0) is_prime[0] = false;
    if(N >= 1) is_prime[1] = false;
    for(int i = 2; i <= N; i++){
        if(is_prime[i]){
            for(long long j = (long long)i*i; j <= N; j += i){
                is_prime[j] = false;
            }
        }
    }
    
    vector<pair<int,int>> results;
    
    // A must be 2, B >= 2 prime, 2+B prime, 2+B <= N
    if(N >= 4){
        for(int B = 2; B <= N-2; B++){
            if(is_prime[B] && is_prime[2+B]){
                results.push_back({2, B});
            }
        }
    }
    
    cout << results.size() << "\n";
    for(auto& p : results){
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
