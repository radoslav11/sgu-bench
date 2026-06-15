#include <bits/stdc++.h>
using namespace std;
struct Vec3{double x,y,z;};
Vec3 add(Vec3 a,Vec3 b){return {a.x+b.x,a.y+b.y,a.z+b.z};}
Vec3 sub(Vec3 a,Vec3 b){return {a.x-b.x,a.y-b.y,a.z-b.z};}
Vec3 scale(Vec3 a,double t){return {a.x*t,a.y*t,a.z*t};}
double dot(Vec3 a,Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}
int main(){
    int n;scanf("%d",&n);
    vector<double>cx(n),cy(n),cz(n),cr(n);
    for(int i=0;i<n;i++)scanf("%lf%lf%lf%lf",&cx[i],&cy[i],&cz[i],&cr[i]);
    double p1x,p1y,p1z,p2x,p2y,p2z;
    scanf("%lf%lf%lf%lf%lf%lf",&p1x,&p1y,&p1z,&p2x,&p2y,&p2z);
    Vec3 P={p1x,p1y,p1z},D={p2x-p1x,p2y-p1y,p2z-p1z};
    vector<int>result;
    for(int iter=0;iter<11;iter++){
        double bestT=1e18;int bestI=-1;
        for(int i=0;i<n;i++){
            Vec3 C={cx[i],cy[i],cz[i]},V=sub(P,C);
            double a=dot(D,D),b=2*dot(V,D),c=dot(V,V)-cr[i]*cr[i];
            double disc=b*b-4*a*c;
            if(disc<0)continue;
            double sq=sqrt(max(0.0,disc));
            double t1=(-b-sq)/(2*a),t2=(-b+sq)/(2*a),t=-1;
            if(t1>1e-9)t=t1;else if(t2>1e-9)t=t2;
            if(t>1e-9&&t<bestT){bestT=t;bestI=i;}
        }
        if(bestI==-1)break;
        result.push_back(bestI+1);
        if((int)result.size()>=11)break;
        Vec3 hit=add(P,scale(D,bestT)),C={cx[bestI],cy[bestI],cz[bestI]};
        Vec3 N=scale(sub(hit,C),1.0/cr[bestI]);
        D=sub(D,scale(N,2*dot(D,N)));
        P=hit;
    }
    bool etc=result.size()>10;
    int show=min((int)result.size(),10);
    for(int i=0;i<show;i++){if(i)printf(" ");printf("%d",result[i]);}
    if(etc)printf(" etc.");
    printf("\n");
}
