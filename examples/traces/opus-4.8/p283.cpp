#include <bits/stdc++.h>
using namespace std;

int main(){
    double ax,ay,vax,vay; double ra,ma;
    double bx,by,vbx,vby; double rb,mb;
    cin>>ax>>ay>>vax>>vay>>ra>>ma;
    cin>>bx>>by>>vbx>>vby>>rb>>mb;
    double T; cin>>T;
    
    double R = ra+rb;
    double eps = 1e-12;
    
    for(int iter=0; iter<5 && T>eps; iter++){
        // relative
        double dpx=ax-bx, dpy=ay-by;
        double dvx=vax-vbx, dvy=vay-vby;
        double a = dvx*dvx+dvy*dvy;
        double b = 2*(dpx*dvx+dpy*dvy);
        double c = dpx*dpx+dpy*dpy - R*R;
        double tc = -1;
        if(a>eps){
            double disc = b*b-4*a*c;
            if(disc>=0){
                double sq=sqrt(disc);
                double t1=(-b-sq)/(2*a);
                double t2=(-b+sq)/(2*a);
                // want smallest positive
                if(t1>1e-9 && t1<=T+1e-9){ tc=t1; }
                else if(t2>1e-9 && t2<=T+1e-9 && t1<= 1e-9){ 
                    // already overlapping? skip
                }
            }
        }
        if(tc<0 || tc>T){
            // no collision in remaining time
            ax+=vax*T; ay+=vay*T;
            bx+=vbx*T; by+=vby*T;
            T=0;
            break;
        }
        // advance to collision
        ax+=vax*tc; ay+=vay*tc;
        bx+=vbx*tc; by+=vby*tc;
        T-=tc;
        // collision: normal from B to A
        double nx=ax-bx, ny=ay-by;
        double nlen=sqrt(nx*nx+ny*ny);
        nx/=nlen; ny/=nlen;
        double v1n=vax*nx+vay*ny;
        double v2n=vbx*nx+vby*ny;
        double v1n2=(v1n*(ma-mb)+2*mb*v2n)/(ma+mb);
        double v2n2=(v2n*(mb-ma)+2*ma*v1n)/(ma+mb);
        vax+=(v1n2-v1n)*nx; vay+=(v1n2-v1n)*ny;
        vbx+=(v2n2-v2n)*nx; vby+=(v2n2-v2n)*ny;
    }
    if(T>eps){
        ax+=vax*T; ay+=vay*T;
        bx+=vbx*T; by+=vby*T;
    }
    
    cout<<fixed<<setprecision(3);
    cout<<ax<<" "<<ay<<" "<<vax<<" "<<vay<<"\n";
    cout<<bx<<" "<<by<<" "<<vbx<<" "<<vby<<"\n";
    return 0;
}
