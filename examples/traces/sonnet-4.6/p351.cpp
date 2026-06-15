#include<bits/stdc++.h>
using namespace std;
const double PI=acos(-1.0);
double XA,YA,XB,YB,XC,YC,RC,D;
double dst(double x1,double y1,double x2,double y2){return hypot(x1-x2,y1-y2);}
bool segIntersectsCircle(double ax,double ay,double bx,double by,double r=0){
    if(r==0)r=RC;
    double dx=bx-ax,dy=by-ay;
    double fx=ax-XC,fy=ay-YC;
    double a=dx*dx+dy*dy;
    double b=2*(fx*dx+fy*dy);
    double c=fx*fx+fy*fy-r*r;
    double disc=b*b-4*a*c;
    if(disc<0)return false;
    double sq=sqrt(max(0.0,disc));
    double t1=(-b-sq)/(2*a),t2=(-b+sq)/(2*a);
    return t1<1-1e-9 && t2>1e-9;
}
