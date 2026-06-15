#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<array<double,4>> seg(n);
    vector<double> angs;
    for(auto&s:seg){cin>>s[0]>>s[1]>>s[2]>>s[3];
        for(int k=0;k<2;k++){double x=s[2*k],y=s[2*k+1];
            if(x==0&&y==0) continue;
            double a=atan2(y,x); if(a<0)a+=2*M_PI; angs.push_back(a);}}
    angs.push_back(0); angs.push_back(2*M_PI);
    sort(angs.begin(),angs.end());
    angs.erase(unique(angs.begin(),angs.end(),[](double a,double b){return fabs(a-b)<1e-12;}),angs.end());
    double total=0;
    bool covered=true;
    for(size_t i=0;i+1<angs.size();i++){
        double a1=angs[i],a2=angs[i+1];
        double best=-1;
        for(auto&s:seg){
            double px=s[0],py=s[1],qx=s[2],qy=s[3];
            double ex=qx-px,ey=qy-py;
            auto ptAt=[&](double ang,double&rx,double&ry)->bool{
                double cx=cos(ang),cy=sin(ang);
                double den=ex*cy-ey*cx;
                if(fabs(den)<1e-15) return false;
                double u=(py*cx-px*cy)/den;
                if(u<-1e-9||u>1+1e-9) return false;
                rx=px+u*ex; ry=py+u*ey;
                double t=rx*cx+ry*cy;
                if(t<1e-9) return false;
                return true;
            };
            double r1x,r1y,r2x,r2y;
            if(!ptAt(a1,r1x,r1y))continue;
            if(!ptAt(a2,r2x,r2y))continue;
            double len=hypot(r1x-r2x,r1y-r2y);
            best=max(best,len);
        }
        if(best<0) covered=false;
        else total+=best;
    }
    if(!covered) printf("0\n");
    else printf("%.6f\n",total);
    return 0;
}
