#include <bits/stdc++.h>
using namespace std;
static int16_t trace[10001][1000];
int main(){
    int N,M;
    cin>>N>>M;
    vector<int>a(N+1);
    for(int i=1;i<=N;i++)cin>>a[i];
    vector<bool>dp(M,false);
    dp[1%M]=true;
    memset(trace,-1,sizeof(trace));
    for(int i=1;i<=N;i++){
        vector<bool>ndp=dp;
        for(int j=0;j<M;j++){
            if(dp[j]){
                int nj=(j*a[i])%M;
                if(!ndp[nj]){ndp[nj]=true;trace[i][nj]=(int16_t)j;}
            }
        }
        dp=ndp;
    }
    int best=-1;
    for(int j=M-1;j>=0;j--)if(dp[j]){best=j;break;}
    cout<<best<<"\n";
    vector<int>ans;
    int cur=best;
    for(int i=N;i>=1;i--){
        if(trace[i][cur]!=-1){ans.push_back(i);cur=trace[i][cur];}
    }
    sort(ans.begin(),ans.end());
    for(int x:ans)cout<<x<<" ";
    if(!ans.empty())cout<<"\n";
}
