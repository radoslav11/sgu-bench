#include <bits/stdc++.h>
using namespace std;
int main(){
    double c,b,m;
    cin>>c>>b>>m;
    double a2 = 2*b*b+2*c*c-4*m*m;
    if(a2 < -1e-9){ cout<<"Mission impossible\n"; return 0;}
    if(a2<0)a2=0;
    double a=sqrt(a2);
    // B=(0,0), C=(a,0)
    if(a<1e-9){
        // B=C at origin, M=origin, |AM|=m, |AB|=c=|AC|=b
        if(fabs(b-c)<1e-6 && fabs(m-c)<1e-6){
            // A at distance c, place at (0,c)? |AB|=c ok, but degenerate triangle
            // Actually triangle would be degenerate (B=C). Probably no valid triangle.
            cout<<"Mission impossible\n"; return 0;
        }
        cout<<"Mission impossible\n"; return 0;
    }
    double x=(c*c-b*b+a*a)/(2*a);
    double y2=c*c-x*x;
    if(y2<-1e-9){ cout<<"Mission impossible\n"; return 0;}
    if(y2<0)y2=0;
    double y=sqrt(y2);
    if(y<1e-9){
        // degenerate triangle
        cout<<"Mission impossible\n"; return 0;
    }
    printf("%.5f %.5f\n",x,y);
    printf("%.5f %.5f\n",0.0,0.0);
    printf("%.5f %.5f\n",a,0.0);
    return 0;
}
