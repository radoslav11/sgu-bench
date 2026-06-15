#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,N1,N2;
    double x1,y1,x2,y2;
    cin>>N>>N1>>N2>>x1>>y1>>x2>>y2;
    int k=((N2-N1)%N+N)%N;
    double PI=acos(-1.0);
    double alpha=k*2*PI/N;
    double mx=(x1+x2)/2, my=(y1+y2)/2;
    double dx=x2-x1, dy=y2-y1;
    double half=alpha/2;
    double L=sqrt(dx*dx+dy*dy);
    double R = L/(2*sin(half));
    double dlen = R*cos(half);
    double ux=dx/L, uy=dy/L;
    double cx=mx + uy*dlen;
    double cy=my - ux*dlen;
    double a1=atan2(y1-cy, x1-cx);
    for(int i=1;i<=N;i++){
        double ang=a1 - (i-N1)*2*PI/N;
        printf("%.6f %.6f\n", cx+R*cos(ang)+0.0, cy+R*sin(ang)+0.0);
    }
    return 0;
}
