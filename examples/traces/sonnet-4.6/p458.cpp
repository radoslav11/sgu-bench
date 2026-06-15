#include <bits/stdc++.h>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);
    int n; cin>>n;
    vector<int>c(n);
    for(auto&x:c)cin>>x;
    vector<int>dp(n,1),par(n,-1);
    unordered_map<int,pair<int,int>>cb;
    vector<pair<int,int>>top;
    for(int i=0;i<n;i++){
        int best=0,bidx=-1;
        for(auto&[d,col]:top){
            if(col!=c[i]-1&&col!=c[i]+1){
                best=d; bidx=cb[col].second; break;
            }
        }
        dp[i]=best+1; par[i]=bidx;
        if(!cb.count(c[i])||cb[c[i]].first<dp[i]){
            cb[c[i]]={dp[i],i};
            bool found=false;
            for(auto&[d,col]:top)if(col==c[i]){d=dp[i];found=true;break;}
            if(!found)top.push_back({dp[i],c[i]});
            sort(top.begin(),top.end(),[](auto&a,auto&b){return a.first>b.first;});
            if((int)top.size()>4)top.pop_back();
        }
    }
    int best=0,bend=0;
    for(int i=0;i<n;i++)if(dp[i]>best){best=dp[i];bend=i;}
    vector<int>res;
    for(int i=bend;i!=-1;i=par[i])res.push_back(c[i]);
    reverse(res.begin(),res.end());
    cout<<n-best<<"\n";
    for(int i=0;i<(int)res.size();i++){
        if(i)cout<<' ';
        cout<<res[i];
    }
    cout<<"\n";
}
