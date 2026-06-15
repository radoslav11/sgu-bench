#include <bits/stdc++.h>
using namespace std;
int N;
vector<double> A, B, C;
double minDist(double x, double y){
    double mn=1e18;
    for(int i=0;i<N;i++){double d=A[i]*x+B[i]*y+C[i];if(d<mn)mn=d;}
    return mn;
}
double ternaryY(double x,double ylo,double yhi){
    for(int i=0;i<200;i++){double m1=ylo+(yhi-ylo)/3,m2=yhi-(yhi-ylo)/3;if(minDist(x,m1)<minDist(x,m2))ylo=m1;else yhi=m2;}
    return minDist(x,(ylo+yhi)/2);
}
int main(){
    ios::sync_with_stdio(false);cin.tie(0);
    cin>>N;
    vector<double>px(N),py(N);
    for(int i=0;i<N;i++)cin>>px[i]>>py[i];
    for(int i=0;i<N;i++){double x1=px[i],y1=py[i],x2=px[(i+1)%N],y2=py[(i+1)%N],dx=x2-x1,dy=y2-y1,len=sqrt(dx*dx+dy*dy),a=-dy/len,b=dx/len,c=-(a*x1+b*y1);A.push_back(a);B.push_back(b);C.push_back(c);}
    double xlo=*min_element(px.begin(),px.end()),xhi=*max_element(px.begin(),px.end()),ylo=*min_element(py.begin(),py.end()),yhi=*max_element(py.begin(),py.end());
    double xl=xlo,xr=xhi;
    for(int i=0;i<200;i++){double m1=xl+(xr-xl)/3,m2=xr-(xr-xl)/3;if(ternaryY(m1,ylo,yhi)<ternaryY(m2,ylo,yhi))xl=m1;else xr=m2;}
    cout<<fixed<<setprecision(6)<<ternaryY((xl+xr)/2,ylo,yhi)<<endl;
}
