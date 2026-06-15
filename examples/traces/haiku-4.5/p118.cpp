#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K;
    cin >> K;
    
    while(K--){
        int N;
        cin >> N;
        
        vector<long long> A(N);
        for(int i = 0; i < N; i++){
            cin >> A[i];
        }
        
        long long result = 0;
        long long product = 1;
        
        for(int i = 0; i < N; i++){
            product = (product * (A[i] % 9)) % 9;
            result = (result + product) % 9;
        }
        
        int digitalRoot = result == 0 ? 9 : result;
        
        cout << digitalRoot << "\n";
    }
    
    return 0;
}
