#include <bits/stdc++.h>
using namespace std;

int main(){
    long long n, m;
    cin >> n >> m;
    if(m==1){ cout<<0<<endl; return 0;}
    if(n==1){ cout<<(1%m)<<endl; return 0;}
    // Entringer numbers
    // E(0,0)=1, E(n,0)=0 (n>0), E(n,k)=E(n,k-1)+E(n-1,n-k)
    // zigzag = E(n,n)
    int N = (int)n;
    vector<long long> prev(N+1,0), cur(N+1,0);
    prev[0]=1%m;
    for(int i=1;i<=N;i++){
        cur.assign(N+1,0);
        cur[0]=0;
        for(int k=1;k<=i;k++){
            cur[k]=(cur[k-1]+prev[i-k])%m;
        }
        prev=cur;
    }
    long long ans = (2*prev[N])%m;
    cout<<ans<<endl;
    return 0;
}
