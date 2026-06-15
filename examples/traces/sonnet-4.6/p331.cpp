#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,d; double c;
    scanf("%d %d %lf",&N,&d,&c);
    vector<double>a(N),b(N),delta(N);
    for(int i=0;i<N;i++) scanf("%lf %lf %lf",&a[i],&b[i],&delta[i]);
    auto speed=[&](int lane,double t){return b[lane]+a[lane]*sin(t+delta[lane]);};
    double t=0,dist=0;int cur=0;
    vector<pair<int,double>>changes;
    double dt=1e-5;
    while(dist<d){
        int best=cur;double bs=speed(cur,t);
        for(int j=0;j<N;j++){double s=speed(j,t);if(s>bs+1e-9){bs=s;best=j;}}
        if(best!=cur){double sw=c*abs(best-cur);t+=sw;changes.push_back({best+1,t-sw});cur=best;}
        else{double ns=speed(cur,t);double rem=d-dist;double step=min(dt,rem/ns);dist+=ns*step;t+=step;}
    }
    printf("%.15f\n%d\n",t,(int)changes.size());
    for(auto&p:changes)printf("%d %.15f\n",p.first,p.second);
}
