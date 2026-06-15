#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N,M;
    cin>>N>>M;
    long long a=N-1, b=M-1; // ranges 1..a, 1..b
    long long limit=min(a,b);
    long long ans=0;
    if(limit>=1){
        int L=(int)limit;
        vector<int> mu(L+1);
        vector<int> primes;
        vector<bool> comp(L+1,false);
        mu[1]=1;
        for(int i=2;i<=L;i++){
            if(!comp[i]){primes.push_back(i);mu[i]=-1;}
            for(int p:primes){
                if((long long)i*p>L)break;
                comp[i*p]=true;
                if(i%p==0){mu[i*p]=0;break;}
                else mu[i*p]=-mu[i];
            }
        }
        for(int d=1;d<=L;d++){
            if(mu[d]!=0){
                ans+=(long long)mu[d]*(a/d)*(b/d);
            }
        }
    }
    // axes
    if(N>=2) ans+=1; // direction (1,0)
    if(M>=2) ans+=1; // direction (0,1)
    cout<<ans<<endl;
}
