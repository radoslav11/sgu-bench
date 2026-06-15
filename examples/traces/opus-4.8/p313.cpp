#include <bits/stdc++.h>
using namespace std;
int n; long long L;
vector<pair<long long,int>> H,O;
long long cost(int k){
    long long s=0;
    for(int i=0;i<n;i++){
        long long d=llabs(H[i].first-O[(i+k)%n].first);
        s+=min(d,L-d);
    }
    return s;
}
int main(){
    scanf("%d %lld",&n,&L);
    H.resize(n);O.resize(n);
    for(int i=0;i<n;i++){long long x;scanf("%lld",&x);H[i]={x,i};}
    for(int i=0;i<n;i++){long long x;scanf("%lld",&x);O[i]={x,i};}
    sort(H.begin(),H.end());sort(O.begin(),O.end());
    int bestk=0; long long best=LLONG_MAX;
    if((long long)n*n<=2e8){
        for(int k=0;k<n;k++){long long c=cost(k);if(c<best){best=c;bestk=k;}}
    } else {
        int lo=0,hi=n-1;
        while(hi-lo>2){
            int m1=lo+(hi-lo)/3, m2=hi-(hi-lo)/3;
            if(cost(m1)<cost(m2)) hi=m2; else lo=m1;
        }
        for(int k=lo;k<=hi;k++){long long c=cost(k);if(c<best){best=c;bestk=k;}}
    }
    vector<int> ans(n);
    for(int i=0;i<n;i++) ans[H[i].second]=O[(i+bestk)%n].second+1;
    printf("%lld\n",best);
    for(int i=0;i<n;i++)printf("%d ",ans[i]);
    printf("\n");
}
