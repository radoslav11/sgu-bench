#include <bits/stdc++.h>
using namespace std;
typedef __int128 ll;

ll C(int n,int r){
    if(r<0||r>n) return 0;
    ll res=1;
    for(int i=0;i<r;i++){
        res*=(n-i);
        res/=(i+1);
    }
    return res;
}

int main(){
    int n,l1,l2,p;
    cin>>n>>l1>>l2>>p;
    vector<ll> w(n+1);
    for(int k=0;k<=n;k++){
        w[k]=C(k,l1)*C(n-k,l2);
    }
    vector<ll> pre(n+2,0);
    for(int k=0;k<=n;k++) pre[k+1]=pre[k]+w[k];
    ll S=pre[n+1];
    int bestA=0,bestB=n;
    bool found=false;
    for(int width=0;width<=n && !found;width++){
        for(int a=0;a+width<=n;a++){
            int b=a+width;
            ll sum=pre[b+1]-pre[a];
            if((ll)100*sum >= (ll)p*S){
                bestA=a;bestB=b;found=true;break;
            }
        }
    }
    cout<<bestA<<" "<<bestB<<endl;
    return 0;
}
