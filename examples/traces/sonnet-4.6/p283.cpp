#include <bits/stdc++.h>
using namespace std;
int main(){
    double ax,ay,vax,vay,ra,ma;
    double bx,by,vbx,vby,rb,mb;
    int t;
    cin>>ax>>ay>>vax>>vay>>ra>>ma;
    cin>>bx>>by>>vbx>>vby>>rb>>mb;
    cin>>t;
    double time_left=t;
    double rsum=ra+rb;
    while(time_left>1e-12){
        double dx=ax-bx,dy=ay-by;
        double dvx=vax-vbx,dvy=vay-vby;
        double A=dvx*dvx+dvy*dvy;
        double B=2*(dx*dvx+dy*dvy);
        double C=dx*dx+dy*dy-rsum*rsum;
        double tau=-1;
        if(A>1e-15){
            double disc=B*B-4*A*C;
            if(disc>=0){
                double sq=sqrt(disc);
                double t1=(-B-sq)/(2*A);
                double t2=(-B+sq)/(2*A);
                if(t1>1e-9&&t1<=time_left+1e-9)tau=t1;
                else if(t2>1e-9&&t2<=time_left+1e-9)tau=t2;
            }
        }
        if(tau<0||tau>time_left+1e-9){
            ax+=vax*time_left;ay+=vay*time_left;
            bx+=vbx*time_left;by+=vby*time_left;
            break;
        }
        ax+=vax*tau;ay+=vay*tau;
        bx+=vbx*tau;by+=vby*tau;
        time_left-=tau;
        double nx=bx-ax,ny=by-ay;
        double len=sqrt(nx*nx+ny*ny);
        nx/=len;ny/=len;
        double uA=vax*nx+vay*ny;
        double uB=vbx*nx+vby*ny;
        double uA2=((ma-mb)*uA+2*mb*uB)/(ma+mb);
        double uB2=((mb-ma)*uB+2*ma*uA)/(ma+mb);
        vax+=(uA2-uA)*nx;vay+=(uA2-uA)*ny;
        vbx+=(uB2-uB)*nx;vby+=(uB2-uB)*ny;
    }
    cout<<fixed<<setprecision(3);
    cout<<ax<<" "<<ay<<" "<<vax<<" "<<vay<<"\n";
    cout<<bx<<" "<<by<<" "<<vbx<<" "<<vby<<"\n";
    return 0;
}
