#include <bits/stdc++.h>
using namespace std;
int main(){
    string sigma,lam,mu;
    cin>>sigma>>lam>>mu;
    int S=sigma.size(),L=lam.size(),M=mu.size();
    map<char,int> idx;
    for(int i=0;i<S;i++) idx[sigma[i]]=i;
    vector<vector<int>> d(S,vector<int>(S));
    for(int i=0;i<S;i++) for(int j=0;j<S;j++) cin>>d[i][j];
    vector<int> minRow(S,INT_MAX),minRowChar(S),minCol(S,INT_MAX),minColChar(S);
    for(int i=0;i<S;i++) for(int j=0;j<S;j++){
        if(d[i][j]<minRow[i]){minRow[i]=d[i][j];minRowChar[i]=j;}
        if(d[i][j]<minCol[j]){minCol[j]=d[i][j];minColChar[j]=i;}
    }
    const int INF=1e9;
    vector<vector<int>> dp(L+1,vector<int>(M+1,INF));
    vector<vector<int>> from(L+1,vector<int>(M+1,-1));
    dp[0][0]=0;
    for(int i=0;i<=L;i++) for(int j=0;j<=M;j++){
        if(dp[i][j]==INF) continue;
        if(i<L&&j<M){int c=dp[i][j]+d[idx[lam[i]]][idx[mu[j]]];if(c<dp[i+1][j+1]){dp[i+1][j+1]=c;from[i+1][j+1]=0;}}
        if(i<L){int c=dp[i][j]+minRow[idx[lam[i]]];if(c<dp[i+1][j]){dp[i+1][j]=c;from[i+1][j]=1;}}
        if(j<M){int c=dp[i][j]+minCol[idx[mu[j]]];if(c<dp[i][j+1]){dp[i][j+1]=c;from[i][j+1]=2;}}
    }
    cout<<dp[L][M]<<"\n";
    string a,b;
    int i=L,j=M;
    while(i>0||j>0){
        int f=from[i][j];
        if(f==0){a+=lam[i-1];b+=mu[j-1];i--;j--;}
        else if(f==1){a+=lam[i-1];b+=sigma[minRowChar[idx[lam[i-1]]]];i--;}
        else{a+=sigma[minColChar[idx[mu[j-1]]]];b+=mu[j-1];j--;}
    }
    reverse(a.begin(),a.end());reverse(b.begin(),b.end());
    cout<<a<<"\n"<<b<<"\n";
}
