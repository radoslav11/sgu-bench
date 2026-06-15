#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,n1,n2;
    cin>>N>>n1>>n2;
    double x1,y1,x2,y2;
    cin>>x1>>y1>>x2>>y2;
    int k = ((n2-n1)%N+N)%N;
    double a = k*2.0*M_PI/N;
    double ca=cos(a), sa=sin(a);
    double m00=ca, m01=sa, m10=-sa, m11=ca;
    double b00=1-m00, b01=-m01, b10=-m10, b11=1-m11;
    double r0 = x2 - (m00*x1+m01*y1);
    double r1 = y2 - (m10*x1+m11*y1);
    double det = b00*b11-b01*b10;
    double cx = (r0*b11 - b01*r1)/det;
    double cy = (b00*r1 - r0*b10)/det;
    cout<<fixed<<setprecision(6);
    for(int j=1;j<=N;j++){
        int d = ((j-n1)%N+N)%N;
        double ang = -d*2.0*M_PI/N;
        double cc=cos(ang), ss=sin(ang);
        double dx=x1-cx, dy=y1-cy;
        double nx = cx + cc*dx - ss*dy;
        double ny = cy + ss*dx + cc*dy;
        cout<<nx<<" "<<ny<<"\n";
    }
}
