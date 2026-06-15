#include <bits/stdc++.h>
using namespace std;
int main(){
    string sigma; getline(cin, sigma);
    string lam, mu; getline(cin,lam); getline(cin,mu);
    int S=sigma.size();
    vector<vector<int>> d(S, vector<int>(S));
    for(int i=0;i<S;i++)for(int j=0;j<S;j++) cin>>d[i][j];
    map<char,int> idx;
    for(int i=0;i<S;i++) idx[sigma[i]]=i;
    vector<int> bestRowCost(S), bestRowCh(S), bestColCost(S), bestColCh(S);
    for(int i=0;i<S;i++){
        int bc=INT_MAX,bch=0;
        for(int c=0;c<S;c++) if(d[i][c]<bc){bc=d[i][c];bch=c;}
        bestRowCost[i]=bc; bestRowCh[i]=bch;
    }
    for(int j=0;j<S;j++){
        int bc=INT_MAX,bch=0;
        for(int c=0;c<S;c++) if(d[c][j]<bc){bc=d[c][j];bch=c;}
        bestColCost[j]=bc; bestColCh[j]=bch;
    }
    int n=lam.size(), m=mu.size();
    vector<int> li(n), mi(m);
    for(int i=0;i<n;i++) li[i]=idx[lam[i]];
    for(int j=0;j<m;j++) mi[j]=idx[mu[j]];
    
    vector<vector<long long>> dp(n+1, vector<long long>(m+1));
    for(int i=0;i<=n;i++) for(int j=0;j<=m;j++) dp[i][j]=LLONG_MAX/2;
    dp[0][0]=0;
    for(int i=0;i<=n;i++){
        for(int j=0;j<=m;j++){
            if(dp[i][j]==LLONG_MAX/2) continue;
            long long cur=dp[i][j];
            if(i<n) dp[i+1][j]=min(dp[i+1][j], cur+bestRowCost[li[i]]);
            if(j<m) dp[i][j+1]=min(dp[i][j+1], cur+bestColCost[mi[j]]);
            if(i<n&&j<m) dp[i+1][j+1]=min(dp[i+1][j+1], cur+(long long)d[li[i]][mi[j]]);
        }
    }
    string A,B;
    int i=n,j=m;
    while(i>0||j>0){
        long long cur=dp[i][j];
        if(i>0&&j>0 && cur==dp[i-1][j-1]+(long long)d[li[i-1]][mi[j-1]]){
            A.push_back(lam[i-1]); B.push_back(mu[j-1]); i--; j--;
        } else if(i>0 && cur==dp[i-1][j]+bestRowCost[li[i-1]]){
            A.push_back(lam[i-1]); B.push_back(sigma[bestRowCh[li[i-1]]]); i--;
        } else {
            B.push_back(mu[j-1]); A.push_back(sigma[bestColCh[mi[j-1]]]); j--;
        }
    }
    reverse(A.begin(),A.end()); reverse(B.begin(),B.end());
    cout<<dp[n][m]<<"\n"<<A<<"\n"<<B<<"\n";
}
