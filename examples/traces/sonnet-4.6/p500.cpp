#include <bits/stdc++.h>
using namespace std;
int main(){
    double r;cin>>r;
    int n;cin>>n;
    vector<double>jx(n),jy(n);
    for(int i=0;i<n;i++)cin>>jx[i]>>jy[i];
    int m;cin>>m;
    vector<double>sx(m),sy(m);
    for(int i=0;i<m;i++)cin>>sx[i]>>sy[i];
    auto area=[&](double d)->double{d=max(-r,min(r,d));return r*r*acos(d/r)-d*sqrt(max(0.0,r*r-d*d));};
    double mn=1e18,mx=-1e18;
    int steps=2000000;
    for(int i=0;i<steps;i++){
        double theta=M_PI*i/steps;
        double ct=cos(theta),st=sin(theta);
        double maxS=-1e18,minJ=1e18;
        for(int j=0;j<n;j++)minJ=min(minJ,jx[j]*ct+jy[j]*st);
        for(int j=0;j<m;j++)maxS=max(maxS,sx[j]*ct+sy[j]*st);
        if(maxS<=minJ+1e-9){mn=min(mn,area(minJ));mx=max(mx,area(maxS));}
    }
    printf("%.15f %.15f\n",mn,mx);
}
