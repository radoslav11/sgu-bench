#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; double vw,vc;
    cin>>n>>vw>>vc;
    vector<double>x(n),y(n);
    for(int i=0;i<n;i++) cin>>x[i]>>y[i];
    // dp[i] = min time to reach vertex i
    vector<double>dp(n,1e18);
    dp[0]=0;
    for(int i=0;i<n;i++){
        if(dp[i]==1e18) continue;
        // walk segment by segment
        double dist=0,t=dp[i];
        for(int j=i+1;j<n;j++){
            double dx=x[j]-x[j-1],dy=y[j]-y[j-1];
            dist+=sqrt(dx*dx+dy*dy);
            dp[j]=min(dp[j],dp[i]+dist/vw);
        }
        // tunnel from i to j
        for(int j=i+1;j<n;j++){
            double minh=min(y[i],y[j]);
            bool ok=true;
            for(int k=i+1;k<j;k++) if(y[k]<=minh){ok=false;break;}
            // also check segments
            if(ok){
                double tdist=(x[j]-x[i])/vc;
                dp[j]=min(dp[j],dp[i]+tdist);
            }
        }
    }
    printf("%.6f\n",dp[n-1]);
}
