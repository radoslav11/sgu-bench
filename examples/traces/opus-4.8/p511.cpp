#include <bits/stdc++.h>
using namespace std;
long long pw(long long a,long long e,long long m){long long r=1%m;a%=m;while(e){if(e&1)r=r*a%m;a=a*a%m;e>>=1;}return r;}
int main(){
    int t;scanf("%d",&t);
    while(t--){
        long long n,p;scanf("%lld %lld",&n,&p);
        if(p==2){printf("-1\n");continue;}
        long long e=n%(p-1);
        vector<int> root(p,0);
        for(long long x=1;x<p;x++){
            long long r=pw(x,e,p);
            if(root[r]==0) root[r]=(int)x;
        }
        bool found=false;
        for(long long u=1;u<p && !found;u++){
            if(root[u]==0) continue;
            long long v=(1-u+p)%p;
            if(v==0) continue;
            if(root[v]!=0){
                printf("%d %d %d\n",root[u],root[v],1);
                found=true;
            }
        }
        if(!found) printf("-1\n");
    }
}
