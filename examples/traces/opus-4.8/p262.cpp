#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M,K;
    cin>>N>>M>>K;
    vector<vector<string>> sym(K, vector<string>(N));
    for(int k=0;k<K;k++)for(int i=0;i<N;i++)cin>>sym[k][i];
    int P=N*M;
    int numPairs=K*(K-1)/2;
    int full=(1<<numPairs)-1;
    vector<int> pmask(P,0);
    for(int p=0;p<P;p++){
        int i=p/M, j=p%M;
        int pidx=0;
        for(int a=0;a<K;a++)for(int b=a+1;b<K;b++){
            if(sym[a][i][j]!=sym[b][i][j]) pmask[p]|=(1<<pidx);
            pidx++;
        }
    }
    vector<int> dp(1<<numPairs, INT_MAX);
    vector<int> choice(1<<numPairs,-1);
    dp[0]=0;
    for(int mask=0;mask<(1<<numPairs);mask++){
        if(dp[mask]==INT_MAX)continue;
        for(int p=0;p<P;p++){
            int nm=mask|pmask[p];
            if(dp[mask]+1<dp[nm]){ dp[nm]=dp[mask]+1; choice[nm]=p; }
        }
    }
    vector<int> used(P,0);
    int cur=full;
    while(cur!=0){
        int p=choice[cur];
        used[p]=1;
        cur=cur & ~pmask[p];
    }
    cout<<dp[full]<<"\n";
    for(int i=0;i<N;i++){
        for(int j=0;j<M;j++) cout<<used[i*M+j];
        cout<<"\n";
    }
}
