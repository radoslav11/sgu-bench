#include <bits/stdc++.h>
using namespace std;
int main(){
    double W,H,D;cin>>W>>H>>D;
    int N;cin>>N;
    vector<double>cx(N),cy(N),cz(N),cr(N);
    for(int i=0;i<N;i++)cin>>cx[i]>>cy[i]>>cz[i]>>cr[i];
    auto f=[&](double x,double y,double z){
        double r=min({x,W-x,y,H-y,z,D-z});
        for(int i=0;i<N;i++){double d=sqrt((x-cx[i])*(x-cx[i])+(y-cy[i])*(y-cy[i])+(z-cz[i])*(z-cz[i]));r=min(r,d-cr[i]);}
        return r;
    };
    double bx=W/2,by=H/2,bz=D/2,br=f(bx,by,bz);
    mt19937 rng(12345);
    auto rnd=[&](double lo,double hi){return lo+(hi-lo)*(rng()/(double)4294967295.0);};
    for(int iter=0;iter<500;iter++){
        double x=rnd(0,W),y=rnd(0,H),z=rnd(0,D);
        for(double step=max({W,H,D});step>1e-7;step*=0.9985){
            double best=f(x,y,z),nx=x,ny=y,nz=z;
            double d[6][3]={{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
            for(auto&v:d){double tx=x+v[0]*step,ty=y+v[1]*step,tz=z+v[2]*step,val=f(tx,ty,tz);if(val>best){best=val;nx=tx;ny=ty;nz=tz;}}
            x=nx;y=ny;z=nz;
            if(best>br){br=best;bx=x;by=y;bz=z;}
        }
    }
    cout<<fixed<<setprecision(9)<<bx<<" "<<by<<" "<<bz<<"\n";
}
