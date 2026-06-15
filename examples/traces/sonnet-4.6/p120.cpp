#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,N1,N2;
    cin>>N>>N1>>N2;
    double x1,y1,x2,y2;
    cin>>x1>>y1>>x2>>y2;
    double dx=x2-x1,dy=y2-y1;
    double L=sqrt(dx*dx+dy*dy);
    int steps=((N2-N1)%N+N)%N;
    double delta=steps*2.0*M_PI/N;
    double R=L/(2.0*sin(delta/2.0));
    double d=R*cos(delta/2.0);
    double mx=(x1+x2)/2,my=(y1+y2)/2;
    double px=-dy/L,py=dx/L;
    double cx=mx+d*px,cy=my+d*py;
    double angle1=atan2(y1-cy,x1-cx);
    double step_angle=-2.0*M_PI/N;
    for(int i=1;i<=N;i++){
        double a=angle1+(i-N1)*step_angle;
        printf("%.6f %.6f\n",cx+R*cos(a),cy+R*sin(a));
    }
}
