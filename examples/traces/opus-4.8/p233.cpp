#include <bits/stdc++.h>
using namespace std;

int main(){
    int N;
    if(scanf("%d",&N)!=1) return 0;
    while(N--){
        double X0,Y0,R,Ax,Ay,Bx,By;
        scanf("%lf %lf %lf %lf %lf %lf %lf",&X0,&Y0,&R,&Ax,&Ay,&Bx,&By);
        double dx=Bx-Ax, dy=By-Ay;
        double len=sqrt(dx*dx+dy*dy);
        if(len<1e-12){
            // degenerate, output any point
            printf("%.6f %.6f\n", X0+R, Y0);
            continue;
        }
        double nx=-dy/len, ny=dx/len;
        double d=len/2.0;
        double Mx=(Ax+Bx)/2.0, My=(Ay+By)/2.0;
        double ux=Mx-X0, uy=My-Y0;
        double m=ux*nx+uy*ny;
        double usq=ux*ux+uy*uy;
        double K=R*R+d*d-usq;
        double A2=4.0*R*R-4.0*m*m;
        double B2=4.0*K*m;
        double C2=4.0*R*R*d*d-K*K;
        
        double bestT=0; double bestR=1e18; bool found=false;
        
        if(fabs(A2)<1e-15){
            // linear: B2*t + C2 =0
            if(fabs(B2)>1e-15){
                double t=-C2/B2;
                if(K-2*m*t >= -1e-6){
                    double rs=sqrt(d*d+t*t);
                    if(rs<bestR){bestR=rs;bestT=t;found=true;}
                }
            }
        } else {
            double disc=B2*B2-4.0*A2*C2;
            if(disc<0) disc=0;
            double sq=sqrt(disc);
            double t1=(-B2+sq)/(2.0*A2);
            double t2=(-B2-sq)/(2.0*A2);
            for(double t : {t1,t2}){
                if(K-2*m*t >= -1e-6){
                    double rs=sqrt(d*d+t*t);
                    if(rs<bestR){bestR=rs;bestT=t;found=true;}
                }
            }
        }
        
        if(!found){
            // fallback
            printf("%.6f %.6f\n", X0+R, Y0);
            continue;
        }
        
        double t=bestT;
        double Px=Mx+t*nx, Py=My+t*ny;
        double OPx=Px-X0, OPy=Py-Y0;
        double dist=sqrt(OPx*OPx+OPy*OPy);
        double Cx,Cy;
        if(dist<1e-12){
            Cx=X0+R*nx; Cy=Y0+R*ny;
        } else {
            Cx=X0+R*OPx/dist;
            Cy=Y0+R*OPy/dist;
        }
        printf("%.6f %.6f\n", Cx, Cy);
    }
    return 0;
}
