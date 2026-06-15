#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    scanf("%d",&N);
    while(N--){
        double x0,y0,R,xa,ya,xb,yb;
        scanf("%lf%lf%lf%lf%lf%lf%lf",&x0,&y0,&R,&xa,&ya,&xb,&yb);
        // ternary search on angle
        double lo=0, hi=2*M_PI;
        for(int i=0;i<200;i++){
            double m1=lo+(hi-lo)/3, m2=hi-(hi-lo)/3;
            double cx1=x0+R*cos(m1), cy1=y0+R*sin(m1);
            double cx2=x0+R*cos(m2), cy2=y0+R*sin(m2);
            auto angle=[&](double cx,double cy){
                double ax=xa-cx,ay=ya-cy,bx=xb-cx,by=yb-cy;
                return acos((ax*bx+ay*by)/(sqrt(ax*ax+ay*ay)*sqrt(bx*bx+by*by)));
            };
            if(angle(cx1,cy1)<angle(cx2,cy2)) lo=m1; else hi=m2;
        }
        double mid=(lo+hi)/2;
        printf("%.6f %.6f\n",x0+R*cos(mid),y0+R*sin(mid));
    }
}
