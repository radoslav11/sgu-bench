#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n; cin>>n;
    vector<vector<int>> top(n,vector<int>(n)),lft(n,vector<int>(n));
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) cin>>top[i][j];
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) cin>>lft[i][j];
    
    vector<vector<int>> cval(n,vector<int>(n)), rval(n,vector<int>(n));
    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        cval[i][j]=i-top[i][j];
        rval[i][j]=j-lft[i][j];
        if(cval[i][j]<0||rval[i][j]<0){cout<<0;return 0;}
    }
    
    // byRowR[i][rv] = list of j where rval[i][j]==rv
    // byColC[j][cv] = list of i where cval[i][j]==cv
    vector<vector<vector<int>>> byRowR(n, vector<vector<int>>(n+1));
    vector<vector<vector<int>>> byColC(n, vector<vector<int>>(n+1));
    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        byRowR[i][rval[i][j]].push_back(j);
        byColC[j][cval[i][j]].push_back(i);
    }
    
    vector<int> cc(n,0),rc(n,0);
    vector<vector<int>> A(n,vector<int>(n,0));
    
    set<pair<int,int>> S;
    for(int i=0;i<n;i++) for(int j=0;j<n;j++){
        if(cval[i][j]==0 && rval[i][j]==0) S.insert({i,j});
    }
    
    for(int v=1;v<=n*n;v++){
        if(S.empty()){cout<<0;return 0;}
        auto [i,j]=*S.begin();
        S.erase(S.begin());
        A[i][j]=v;
        
        int old_rv=rc[i];
        for(int j2: byRowR[i][old_rv]) if(j2!=j) S.erase({i,j2});
        rc[i]++;
        for(int j2: byRowR[i][rc[i]]) if(cc[j2]==cval[i][j2]) S.insert({i,j2});
        
        int old_cv=cc[j];
        for(int i2: byColC[j][old_cv]) if(i2!=i) S.erase({i2,j});
        cc[j]++;
        for(int i2: byColC[j][cc[j]]) if(rc[i2]==rval[i2][j]) S.insert({i2,j});
    }
    
    if(!S.empty()){cout<<0;return 0;}
    for(int i=0;i<n;i++) for(int j=0;j<n;j++) cout<<A[i][j]<<" \n"[j==n-1];
}
