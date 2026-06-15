#include <bits/stdc++.h>
using namespace std;
int main(){
    int S; cin>>S;
    long long dp[10][2];
    for(int v=0;v<10;v++){dp[v][0]=1;dp[v][1]=0;}
    for(int pos=1;pos<S;pos++){
        long long ndp[10][2];
        memset(ndp,0,sizeof(ndp));
        for(int v=0;v<10;v++){
            long long a=dp[v][0];
            if(a){
                for(int nv=v;nv<10;nv++) ndp[nv][0]+=a;
                for(int nv=0;nv<v;nv++) ndp[nv][1]+=a;
            }
            long long b=dp[v][1];
            if(b){
                for(int nv=0;nv<=v;nv++) ndp[nv][1]+=b;
            }
        }
        memcpy(dp,ndp,sizeof(dp));
    }
    long long ans=0;
    for(int v=0;v<10;v++) ans+=dp[v][0]+dp[v][1];
    cout<<ans<<endl;
}
