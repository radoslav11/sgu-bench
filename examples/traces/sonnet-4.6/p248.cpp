#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    vector<long long>c(N);
    for(auto&x:c)cin>>x;
    long long V; cin>>V;
    long long ans=LLONG_MAX;
    if(N==1){if(V%c[0]==0)ans=V/c[0];}
    else if(N==2){for(long long x0=0;x0*c[0]<=V;x0++){long long r=V-x0*c[0];if(r%c[1]==0)ans=min(ans,x0+r/c[1]);}}
    else{for(long long x0=0;x0*c[0]<=V;x0++){long long R=V-x0*c[0];for(long long x1=0;x1*c[1]<=R;x1++){long long r=R-x1*c[1];if(r%c[2]==0)ans=min(ans,x0+x1+r/c[2]);}}}
    cout<<(ans==LLONG_MAX?-1:ans)<<endl;
}
