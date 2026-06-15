#include <bits/stdc++.h>
using namespace std;

double W,H,D;
int N;
double X[15],Y[15],Z[15],R[15];

double eval(double x,double y,double z){
    double m=min(min(x,W-x),min(min(y,H-y),min(z,D-z)));
    for(int i=0;i<N;i++){
        double dx=x-X[i],dy=y-Y[i],dz=z-Z[i];
        double d=sqrt(dx*dx+dy*dy+dz*dz)-R[i];
        if(d<m)m=d;
    }
    return m;
}

int main(){
    cin>>W>>H>>D;
    cin>>N;
    for(int i=0;i<N;i++)cin>>X[i]>>Y[i]>>Z[i]>>R[i];
    
    double bx=W/2,by=H/2,bz=D/2,bv=eval(bx,by,bz);
    
    mt19937 rng(12345);
    uniform_real_distribution<double> ux(0,W),uy(0,H),uz(0,D);
    
    int restarts=200;
    for(int rs=0;rs<restarts;rs++){
        double x,y,z;
        if(rs==0){x=W/2;y=H/2;z=D/2;}
        else {x=ux(rng);y=uy(rng);z=uz(rng);}
        double v=eval(x,y,z);
        double step=max(max(W,H),D)/2;
        while(step>1e-9){
            bool improved=false;
            double dirs[6][3]={{step,0,0},{-step,0,0},{0,step,0},{0,-step,0},{0,0,step},{0,0,-step}};
            for(int d=0;d<6;d++){
                double nx=x+dirs[d][0],ny=y+dirs[d][1],nz=z+dirs[d][2];
                double nv=eval(nx,ny,nz);
                if(nv>v){v=nv;x=nx;y=ny;z=nz;improved=true;}
            }
            if(!improved)step/=2;
        }
        if(v>bv){bv=v;bx=x;by=y;bz=z;}
    }
    
    {
        double x=bx,y=by,z=bz,v=bv;
        double step=max(max(W,H),D)/4;
        while(step>1e-11){
            bool improved=false;
            double dirs[6][3]={{step,0,0},{-step,0,0},{0,step,0},{0,-step,0},{0,0,step},{0,0,-step}};
            for(int d=0;d<6;d++){
                double nx=x+dirs[d][0],ny=y+dirs[d][1],nz=z+dirs[d][2];
                double nv=eval(nx,ny,nz);
                if(nv>v){v=nv;x=nx;y=ny;z=nz;improved=true;}
            }
            if(!improved)step/=2;
        }
        if(v>bv){bv=v;bx=x;by=y;bz=z;}
    }
    
    printf("%.10f %.10f %.10f\n",bx,by,bz);
    return 0;
}
