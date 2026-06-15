#include <bits/stdc++.h>
using namespace std;
int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    long long B,N; cin>>B>>N;
    vector<long long> X(N);
    vector<vector<pair<long long,long long>>> horses(N);
    for(int i=0;i<N;i++){long long mi;cin>>X[i]>>mi;horses[i].resize(mi);for(int j=0;j<(int)mi;j++)cin>>horses[i][j].first>>horses[i][j].second;}
    vector<int> idx(N);iota(idx.begin(),idx.end(),0);
    sort(idx.begin(),idx.end(),[&](int a,int b){return X[a]<X[b];});
    vector<double> dp(N,1e18);
    double ans=1e18;
    for(int ii=0;ii<N;ii++){int i=idx[ii];if(X[i]==0)dp[ii]=0;if(dp[ii]>=1e17)continue;for(auto [v,d]:horses[i]){if(X[i]+d>=B)ans=min(ans,dp[ii]+(double)(B-X[i])/v);for(int jj=ii+1;jj<N;jj++){int j=idx[jj];if(X[j]-X[i]>d)break;dp[jj]=min(dp[jj],dp[ii]+(double)(X[j]-X[i])/v);}}}
    if(ans>=1e17)cout<<-1<<endl;
    else cout<<fixed<<setprecision(8)<<ans<<endl;
}
