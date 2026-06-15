#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    cin >> N >> M;
    
    vector<array<int,4>> edges(M);
    for(int i = 0; i < M; i++){
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2] >> edges[i][3];
    }
    
    // Binary search on flow
    int left = 0, right = 100000;
    int ans = -1;
    vector<long long> ansFlow;
    
    while(left <= right){
        int mid = (left + right) / 2;
        vector<long long> flow(M);
        bool ok = true;
        
        for(int i = 0; i < M; i++){
            if(edges[i][3] == 1) flow[i] = edges[i][2];
            else flow[i] = 0;
        }
        
        vector<long long> balance(N+1);
        for(int i = 0; i < M; i++){
            balance[edges[i][0]] -= flow[i];
            balance[edges[i][1]] += flow[i];
        }
        
        balance[1] += mid;
        balance[N] -= mid;
        
        for(int i = 2; i < N; i++){
            if(balance[i] != 0) ok = false;
        }
        
        if(ok && balance[1] == 0 && balance[N] == 0){
            ans = mid;
            ansFlow = flow;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    
    if(ans == -1) cout << "Impossible\n";
    else {
        cout << ans << "\n";
        for(int i = 0; i < M; i++){
            cout << ansFlow[i];
            if(i < M-1) cout << " ";
        }
        cout << "\n";
    }
    
    return 0;
}
