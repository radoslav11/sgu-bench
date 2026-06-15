#include <bits/stdc++.h>
using namespace std;

struct P{double x,y;};
vector<P> poly, sq;

// intersect infinite line {point : point·n = p} with convex polygon, get t-range along e
// n·e=0, e unit, n unit
bool lineInterval(const vector<P>& pg,double nx,double ny,double ex,double ey,double p,double&tlo,double&thi){
    double tmin=1e18,tmax=-1e18; bool found=false;
    int m=pg.size();
    for(int i=0;i<m;i++){
        const P&a=pg[i]; const P&b=pg[(i+1)%m];
        double fa=a.x*nx+a.y*ny-p;
        double fb=b.x*nx+b.y*ny-p;
        if((fa<0&&fb>0)||(fa>0&&fb<0)){
            double s=fa/(fa-fb);
            double ix=a.x+(b.x-a.x)*s, iy=a.y+(b.y-a.y)*s;
            double t=ix*ex+iy*ey;
            tmin=min(tmin,t);tmax=max(tmax,t);found=true;
        }
    }
    for(int i=0;i<m;i++){
        double fv=pg[i].x*nx+pg[i].y*ny-p;
        if(fabs(fv)<1e-7){
            double t=pg[i].x*ex+pg[i].y*ey;
            tmin=min(tmin,t);tmax=max(tmax,t);found=true;
        }
    }
    tlo=tmin;thi=tmax;return found;
}

double feval(double nx,double ny,double ex,double ey,double p){
    double tdl,tdh,tkl,tkh;
    if(!lineInterval(sq,nx,ny,ex,ey,p,tdl,tdh)) return 0.0;
    if(!lineInterval(poly,nx,ny,ex,ey,p,tkl,tkh)) return 0.0;
    double Dlen=tdh-tdl;
    double L=tkl-tdl;
    double R=tdh-tkh;
    if(L<0)L=0; if(R<0)R=0;
    return Dlen*Dlen*Dlen - L*L*L - R*R*R;
}

double g(double phi){
    double nx=cos(phi), ny=sin(phi);
    double ex=-sin(phi), ey=cos(phi);
    // p range = projection range of polygon
    double pmin=1e18,pmax=-1e18;
    vector<double> bps;
    for(auto&v:poly){double pr=v.x*nx+v.y*ny; pmin=min(pmin,pr);pmax=max(pmax,pr);}
    bps.push_back(pmin); bps.push_back(pmax);
    for(auto&v:poly){double pr=v.x*nx+v.y*ny; if(pr>pmin+1e-12&&pr<pmax-1e-12) bps.push_back(pr);}
    for(auto&v:sq){double pr=v.x*nx+v.y*ny; if(pr>pmin+1e-12&&pr<pmax-1e-12) bps.push_back(pr);}
    sort(bps.begin(),bps.end());
    // unique
    vector<double> b2;
    for(double x:bps){ if(b2.empty()||x>b2.back()+1e-12) b2.push_back(x); }
    double total=0;
    for(size_t i=0;i+1<b2.size();i++){
        double p1=b2[i], p2=b2[i+1];
        double mid=(p1+p2)/2;
        double f1=feval(nx,ny,ex,ey,p1);
        double fm=feval(nx,ny,ex,ey,mid);
        double f2=feval(nx,ny,ex,ey,p2);
        total += (p2-p1)/6.0*(f1+4*fm+f2);
    }
    return total;
}

double simp(double a,double b,double fa,double fm,double fb){
    return (b-a)/6.0*(fa+4*fm+fb);
}
double adapt(double a,double b,double fa,double fm,double fb,double whole,double eps,int depth){
    double m=(a+b)/2;
    double lm=(a+m)/2, rm=(m+b)/2;
    double flm=g(lm), frm=g(rm);
    double left=simp(a,m,fa,flm,fm);
    double right=simp(m,b,fm,frm,fb);
    if(depth<=0 || fabs(left+right-whole)<=15*eps)
        return left+right+(left+right-whole)/15.0;
    return adapt(a,m,fa,flm,fm,left,eps/2,depth-1)+adapt(m,b,fm,frm,fb,right,eps/2,depth-1);
}

int main(){
    int n; 
    if(!(cin>>n)) return 0;
    poly.resize(n);
    for(int i=0;i<n;i++) cin>>poly[i].x>>poly[i].y;
    sq = { {0,0},{100,0},{100,100},{0,100} };
    double a=0.0,b=M_PI;
    double m=(a+b)/2;
    double fa=g(a),fm=g(m),fb=g(b);
    double whole=simp(a,b,fa,fm,fb);
    double integral=adapt(a,b,fa,fm,fb,whole,1e-3,60);
    double W=integral/3.0;
    double A=10000.0;
    double prob=W/(A*A);
    printf("%.10f\n",prob);
    return 0;
}
