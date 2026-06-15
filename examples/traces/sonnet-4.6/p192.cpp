#include <bits/stdc++.h>
using namespace std;
int main(){
    int N; cin>>N;
    vector<double>x1(N),y1(N),x2(N),y2(N); vector<char>col(N);
    for(int i=0;i<N;i++){cin>>x1[i]>>y1[i]>>x2[i]>>y2[i]>>col[i];if(x1[i]>x2[i]){swap(x1[i],x2[i]);swap(y1[i],y2[i]);}}
    set<double>crits;
    for(int i=0;i<N;i++){crits.insert(x1[i]);crits.insert(x2[i]);}
    for(int i=0;i<N;i++)for(int j=i+1;j<N;j++){
        if(x1[i]>=x2[i]||x1[j]>=x2[j])continue;
        double lo=max(x1[i],x1[j]),hi=min(x2[i],x2[j]);if(lo>=hi)continue;
        auto fy=[&](int k,double x){return y1[k]+(y2[k]-y1[k])/(x2[k]-x1[k])*(x-x1[k]);};
        double di=fy(i,lo)-fy(j,lo),dh=fy(i,hi)-fy(j,hi);
        double df=dh-di;
        if(abs(df)>1e-12){double t=-di/df;if(t>0&&t<1)crits.insert(lo+t*(hi-lo));}
    }
    vector<double>cv(crits.begin(),crits.end());
    double SR=0,SG=0,SB=0;
    auto getY=[&](int i,double x){return y1[i]+(y2[i]-y1[i])/(x2[i]-x1[i])*(x-x1[i]);};
    for(int k=0;k+1<(int)cv.size();k++){
        double lo=cv[k],hi=cv[k+1];if(hi<=lo)continue;
        double mx=(lo+hi)/2,miny=1e18;int best=-1;
        for(int i=0;i<N;i++){
            if(x1[i]>=x2[i])continue;
            if(mx<x1[i]-1e-12||mx>x2[i]+1e-12)continue;
            double yy=getY(i,mx);if(yy<miny){miny=yy;best=i;}
        }
        if(best<0)continue;
        double len=hi-lo;
        if(col[best]=='R')SR+=len;else if(col[best]=='G')SG+=len;else SB+=len;
    }
    auto pr=[](char c,double v){
        long long h=llround(v*100);
        if(h%100==0)printf("%c %lld\n",c,h/100);
        else if(h%10==0)printf("%c %lld.%lld\n",c,h/100,(h%100)/10);
        else printf("%c %lld.%02lld\n",c,h/100,h%100);
    };
    pr('R',SR);pr('G',SG);pr('B',SB);
}
