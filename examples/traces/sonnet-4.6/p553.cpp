#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int main(){
    int n,m,k;
    cin>>n>>m>>k;
    vector<ll>w(n+1),c(n+1);
    for(int i=1;i<=n;i++) cin>>w[i]>>c[i];
    vector<ll>pw(n+2,0),pc(n+2,0);
    for(int i=1;i<=n;i++){pw[i]=pw[i-1]+w[i];pc[i]=pc[i-1]+c[i];}
    ll bestVal=-1; int bestH=0,bestT=0;
    for(int h=0;h<=n-m;h++){
        ll wh=pw[n-h]-pw[n-m-h];
        ll wt_base=pw[n-m-h];
        if(wh>k*wt_base) break;
        int lo=0,hi=n-m-h,best_t=0;
        while(lo<=hi){int mid=(lo+hi)/2;if(wh<=k*(wt_base-pw[mid])){best_t=mid;lo=mid+1;}else hi=mid-1;}
        ll val=(pc[n]-pc[n-h])+pc[best_t];
        if(val>bestVal){bestVal=val;bestH=h;bestT=best_t;}
    }
    if(bestVal<=0){cout<<"0 0\n";return 0;}
    cout<<bestH+bestT<<" "<<bestVal<<"\n";
    string s(bestH,'H');s+=string(bestT,'T');
    cout<<s<<"\n";
}
