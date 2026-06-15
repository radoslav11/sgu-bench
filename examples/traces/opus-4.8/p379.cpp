#include <bits/stdc++.h>
using namespace std;

int N; long long C,P,T;
vector<long long> A;
vector<long long> pre; // prefix sums, pre[i]=sum A[1..i], pre[0]=0

// compute sum of floors at chunk-top positions for given k, return as __int128 cost = 2*P*sumFloors
// return true if cost <= T
bool feasible(long long k){
    if(k<=0) return true;
    __int128 sumF=0;
    for(int i=1;i<=N;i++){
        long long a=pre[i-1]; // exclusive lower
        long long b=pre[i];   // inclusive upper
        if(A[i]==0) continue;
        // positions p in (a,b], p=k-j*C, j>=0, p>=1, p<=k
        // j >= ceil((k-b)/C)  and j>=0
        // jc in [k-b, k-a) and jc <= k-1
        long long lowVal = k-b; // jC >= lowVal
        long long jlow;
        if(lowVal<=0) jlow=0;
        else jlow=(lowVal + C -1)/C;
        // jC < k-a  => jC <= k-a-1
        long long up1 = k-a-1;
        long long up2 = k-1;
        long long upVal = min(up1,up2);
        if(upVal<0) continue;
        long long jhigh = upVal / C;
        if(jhigh<0) continue;
        if(jhigh<jlow) continue;
        long long cnt = jhigh - jlow +1;
        sumF += (__int128)cnt * i;
    }
    __int128 cost = sumF * (__int128)2 * (__int128)P;
    return cost <= (__int128)T;
}

int main(){
    cin>>N>>C>>P>>T;
    A.assign(N+1,0);
    pre.assign(N+1,0);
    long long total=0;
    for(int i=1;i<=N;i++){cin>>A[i]; pre[i]=pre[i-1]+A[i]; total+=A[i];}
    // binary search max k feasible
    long long lo=0, hi=total, ans=0;
    while(lo<=hi){
        long long mid=(lo+hi)/2;
        if(feasible(mid)){ans=mid; lo=mid+1;}
        else hi=mid-1;
    }
    cout<<ans<<endl;
}
