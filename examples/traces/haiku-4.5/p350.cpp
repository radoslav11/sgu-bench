#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int M;
    cin >> M;
    
    vector<long long> B(M);
    set<long long> B_set;
    for(int i = 0; i < M; i++){
        cin >> B[i];
        B_set.insert(B[i]);
    }
    
    int n = 1;
    while(n * (n - 1) / 2 < M) n++;
    
    vector<long long> A;
    A.push_back(0);
    
    set<long long> remaining = B_set;
    
    for(int i = 1; i < n; i++){
        for(long long candidate : remaining){
            bool valid = true;
            vector<long long> new_xors;
            
            for(int j = 0; j < i; j++){
                long long xor_val = A[j] ^ candidate;
                new_xors.push_back(xor_val);
                if(remaining.find(xor_val) == remaining.end()){
                    valid = false;
                    break;
                }
            }
            
            if(valid){
                A.push_back(candidate);
                remaining.erase(candidate);
                for(long long x : new_xors){
                    remaining.erase(x);
                }
                break;
            }
        }
    }
    
    for(int i = 0; i < n; i++){
        if(i > 0) cout << " ";
        cout << A[i];
    }
    cout << "\n";
    
    return 0;
}
