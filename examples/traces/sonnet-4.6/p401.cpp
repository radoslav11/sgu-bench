#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,u,t;
    cin>>N>>u>>t;
    vector<double> w(N),v(N);
    double minT=0;
    for(int i=0;i<N;i++){cin>>w[i]>>v[i]; minT+=w[i]/u;}
    if(minT>t+1e-9){cout<<-1;return 0;}
    double sumW=0; for(auto x:w) sumW+=x;
    auto calcT=[&](double lam)->vector<double>{
        vector<double> ti(N);
        for(int i=0;i<N;i++){double d=lam-v[i];if(d<=u)ti[i]=1e18;else ti[i]=w[i]*d/(u*sqrt(d*d-u*u));}
        return ti;
    };
    double vmax=*max_element(v.begin(),v.end());
    double lo=vmax+u+1e-12,hi=1e12;
    for(int iter=0;iter<200;iter++){double mid=(lo+hi)/2;auto ti=calcT(mid);double s=0;for(auto x:ti)s+=x;if(s>t)lo=mid;else hi=mid;}
    auto ti=calcT((lo+hi)/2);
    double north=0;
    for(int i=0;i<N;i++)north+=sqrt(u*u*ti[i]*ti[i]-w[i]*w[i])+v[i]*ti[i];
    double dist=sqrt(sumW*sumW+north*north);
    cout<<fixed<<setprecision(10)<<dist<<"\n";
    for(int i=0;i<N;i++)cout<<fixed<<setprecision(10)<<ti[i]<<(i<N-1?" ":"\n");
}
