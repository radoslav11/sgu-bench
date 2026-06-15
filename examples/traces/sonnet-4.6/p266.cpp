#include<bits/stdc++.h>
using namespace std;
int main(){
    int R;
    while(cin>>R){
        double x1,y1,z1,x2,y2,z2;
        cin>>x1>>y1>>z1>>x2>>y2>>z2;
        double d1=sqrt(x1*x1+y1*y1+z1*z1);
        double d2=sqrt(x2*x2+y2*y2+z2*z2);
        double nx1=x1/d1,ny1=y1/d1,nz1=z1/d1;
        double nx2=x2/d2,ny2=y2/d2,nz2=z2/d2;
        double cosA1=(double)R/d1,cosA2=(double)R/d2;
        int NT=2000,NP=4000;
        double dtheta=M_PI/NT,dphi=2*M_PI/NP;
        double area=0;
        for(int i=0;i<NT;i++){
            double theta=(i+0.5)*dtheta;
            double st=sin(theta),ct=cos(theta);
            double w=R*R*st*dtheta*dphi;
            for(int j=0;j<NP;j++){
                double phi=(j+0.5)*dphi;
                double x=st*cos(phi),y=st*sin(phi),z=ct;
                if(x*nx1+y*ny1+z*nz1>=cosA1||x*nx2+y*ny2+z*nz2>=cosA2)area+=w;
            }
        }
        printf("%.3f\n",area);
    }
}
