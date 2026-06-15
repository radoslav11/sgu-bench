#include<bits/stdc++.h>
using namespace std;

int N;
vector<long long> X;
long long maxVal;

void solve(int idx, vector<long long>& A, vector<long long>& B, vector<long long>& C) {
    if(idx == 3*N) {
        if(A.size() == N && B.size() == N && C.size() == N) {
            vector<long long> AA = A, BB = B, CC = C;
            sort(AA.rbegin(), AA.rend());
            sort(BB.begin(), BB.end());
            sort(CC.rbegin(), CC.rend());
            
            long long sum = 0;
            for(int i = 0; i < N; i++) {
                sum += (AA[i] - BB[i]) * CC[i];
            }
            maxVal = max(maxVal, sum);
        }
        return;
    }
    
    if(A.size() < N) {
        A.push_back(X[idx]);
        solve(idx+1, A, B, C);
        A.pop_back();
    }
    if(B.size() < N) {
        B.push_back(X[idx]);
        solve(idx+1, A, B, C);
        B.pop_back();
    }
    if(C.size() < N) {
        C.push_back(X[idx]);
        solve(idx+1, A, B, C);
        C.pop_back();
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T >> N;
    
    while(T--) {
        X.resize(3*N);
        for(int i = 0; i < 3*N; i++) {
            cin >> X[i];
        }
        
        maxVal = LLONG_MIN;
        vector<long long> A, B, C;
        solve(0, A, B, C);
        
        cout << maxVal << "\n";
    }
    
    return 0;
}
