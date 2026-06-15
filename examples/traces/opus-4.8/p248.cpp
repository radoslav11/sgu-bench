#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin>>n;
    vector<long long> c(n);
    for(auto&x:c)cin>>x;
    long long V;
    cin>>V;
    
    const long long INF=LLONG_MAX/2;
    vector<long long> dp(V+1, INF);
    dp[0]=0;
    for(int i=0;i<n;i++){
        for(long long v=c[i];v<=V;v++){
            if(dp[v-c[i]]+1<dp[v]) dp[v]=dp[v-c[i]]+1;
        }
    }
    if(dp[V]>=INF) cout<<-1<<endl;
    else cout<<dp[V]<<endl;
    return 0;
}
