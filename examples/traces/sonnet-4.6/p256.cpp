#include <bits/stdc++.h>
using namespace std;
int main(){
    int M,N; cin>>M>>N;
    vector<int> A(N),B(N);
    for(int i=0;i<N;i++) cin>>A[i]>>B[i];
    // state: rest timers encoded, dp[state]=max balloons
    int S=1; for(int i=0;i<N;i++) S*=(B[i]+1);
    vector<int> mul(N); mul[0]=1; for(int i=1;i<N;i++) mul[i]=mul[i-1]*(B[i-1]+1);
    vector<int> dp(S,-1); dp[0]=0;
    for(int t=1;t<=500;t++){
        vector<int> ndp(S,-1);
        for(int s=0;s<S;s++){
            if(dp[s]<0) continue;
            int balls=dp[s];
            // decode timers
            vector<int> tim(N); int tmp=s;
            for(int i=0;i<N;i++){tim[i]=tmp%(B[i]+1);tmp/=(B[i]+1);}
            // advance timers by 1 (rest)
            vector<int> ntim(N); for(int i=0;i<N;i++) ntim[i]=max(0,tim[i]-1);
            // option: idle
            int ns=0; for(int i=0;i<N;i++) ns+=ntim[i]*mul[i];
            if(ndp[ns]<balls) ndp[ns]=balls;
            // option: volunteer i works (tim[i]==0)
            for(int i=0;i<N;i++){
                if(tim[i]==0){
                    vector<int> wt=ntim; wt[i]=B[i];
                    int ws=0; for(int j=0;j<N;j++) ws+=wt[j]*mul[j];
                    int nb=min(balls+A[i],M);
                    if(ndp[ws]<nb) ndp[ws]=nb;
                }
            }
        }
        dp=ndp;
        for(int s=0;s<S;s++) if(dp[s]>=M){cout<<t;return 0;}
    }
}
