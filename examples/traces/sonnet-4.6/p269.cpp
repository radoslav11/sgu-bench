#include <bits/stdc++.h>
using namespace std;
typedef vector<long long> Big;
const long long BASE=1000000000LL;
Big fi(long long x){Big r;if(x==0){r.push_back(0);return r;}while(x>0){r.push_back(x%BASE);x/=BASE;}return r;}
Big add(Big a,Big b){Big r;long long c=0;int n=max(a.size(),b.size());for(int i=0;i<n||c;i++){long long s=c+(i<(int)a.size()?a[i]:0)+(i<(int)b.size()?b[i]:0);r.push_back(s%BASE);c=s/BASE;}return r;}
Big mul(Big a,long long x){if(x==0)return fi(0);Big r;long long c=0;for(int i=0;i<(int)a.size()||c;i++){long long s=c+(i<(int)a.size()?a[i]*x:0);r.push_back(s%BASE);c=s/BASE;}return r;}
void pr(Big a){printf("%lld",a.back());for(int i=(int)a.size()-2;i>=0;i--)printf("%09lld",a[i]);}
int main(){int n,k;scanf("%d%d",&n,&k);vector<int>b(n);for(int i=0;i<n;i++)scanf("%d",&b[i]);sort(b.begin(),b.end());vector<vector<Big>>dp(n+1,vector<Big>(k+1,fi(0)));dp[0][0]=fi(1);for(int i=1;i<=n;i++){for(int j=0;j<=min(i,k);j++){dp[i][j]=dp[i-1][j];if(j>0&&b[i-1]-(j-1)>0){Big t=mul(dp[i-1][j-1],(long long)(b[i-1]-j+1));dp[i][j]=add(dp[i][j],t);}}}pr(dp[n][k]);printf("\n");}
