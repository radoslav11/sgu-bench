#include <bits/stdc++.h>
using namespace std;
int main(){
    long long N,M;
    cin>>N>>M;
    if(N==1&&M==1){cout<<0;return 0;}
    long long A=N-1,B=M-1;
    int lim=max(A,B);
    vector<int> mu(lim+1,0);
    vector<bool> notprime(lim+1,false);
    vector<int> primes;
    mu[1]=1;
    for(int i=2;i<=lim;i++){
        if(!notprime[i]){primes.push_back(i);mu[i]=-1;}
        for(int j=0;j<(int)primes.size()&&(long long)i*primes[j]<=lim;j++){
            notprime[i*primes[j]]=true;
            if(i%primes[j]==0){mu[i*primes[j]]=0;break;}
            else mu[i*primes[j]]=-mu[i];
        }
    }
    long long ans=0;
    long long mn=min(A,B);
    for(long long d=1;d<=mn;d++){
        if(mu[d]!=0) ans+=mu[d]*(A/d)*(B/d);
    }
    ans+=(N>=2?1:0)+(M>=2?1:0);
    cout<<ans<<endl;
}
