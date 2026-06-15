#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,k;
    cin>>n>>k;
    vector<int> valid;
    for(int m=0;m<(1<<n);m++) if((m&(m>>1))==0) valid.push_back(m);
    vector<vector<long long>> dp(1<<n,vector<long long>(k+1,0));
    for(int m:valid){int c=__builtin_popcount(m);if(c<=k)dp[m][c]++;}
    for(int row=1;row<n;row++){
        vector<vector<long long>> ndp(1<<n,vector<long long>(k+1,0));
        for(int pm:valid) for(int nm:valid){
            if(pm&nm) continue;
            if((pm>>1)&nm) continue;
            if((pm<<1)&nm) continue;
            int add=__builtin_popcount(nm);
            for(int cnt=0;cnt+add<=k;cnt++) if(dp[pm][cnt]) ndp[nm][cnt+add]+=dp[pm][cnt];
        }
        dp=ndp;
    }
    long long ans=0;
    for(int m=0;m<(1<<n);m++) ans+=dp[m][k];
    cout<<ans<<endl;
}
