#include <bits/stdc++.h>
using namespace std;
const long long MOD=1000000009LL;
int main(){
    int n; long long k;
    scanf("%d %lld",&n,&k);
    vector<long long> B(n),Y(n);
    for(auto&x:B)scanf("%lld",&x);
    for(auto&x:Y)scanf("%lld",&x);
    if((n+k)%2!=0){printf("0\n");return 0;}
    long long w=(n+k)/2;
    if(w<0||w>n){printf("0\n");return 0;}
    sort(B.begin(),B.end());
    sort(Y.begin(),Y.end());
    // c_i = number of Byteland weaker than B[i]
    vector<int> c(n);
    for(int i=0;i<n;i++){
        // count Y < B[i]
        c[i]=upper_bound(Y.begin(),Y.end(),B[i])-Y.begin();
    }
    // dp[j] = number of ways
    vector<long long> dp(n+1,0);
    dp[0]=1;
    for(int i=0;i<n;i++){
        // process B[i] with c[i]
        for(int j=i+1;j>=1;j--){
            long long avail = c[i]-(j-1);
            if(avail>0){
                dp[j]=(dp[j]+dp[j-1]*avail)%MOD;
            }
        }
    }
    // dp[m] = g[m]
    // factorials
    vector<long long> fact(n+1);
    fact[0]=1;
    for(int i=1;i<=n;i++)fact[i]=fact[i-1]*i%MOD;
    // binomials C(m,w)
    // precompute
    vector<vector<long long>> C(n+1,vector<long long>(n+1,0));
    for(int i=0;i<=n;i++){C[i][0]=1;for(int j=1;j<=i;j++)C[i][j]=(C[i-1][j-1]+C[i-1][j])%MOD;}
    long long ans=0;
    for(int m=w;m<=n;m++){
        long long term=C[m][w]*dp[m]%MOD*fact[n-m]%MOD;
        if((m-w)%2==0) ans=(ans+term)%MOD;
        else ans=(ans-term+MOD)%MOD;
    }
    printf("%lld\n",ans%MOD);
    return 0;
}
