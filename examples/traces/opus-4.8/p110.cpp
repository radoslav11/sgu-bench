#include <bits/stdc++.h>
using namespace std;

int n;
double cx[55], cy[55], cz[55], cr[55];

int main(){
    cin >> n;
    for(int i=0;i<n;i++) cin>>cx[i]>>cy[i]>>cz[i]>>cr[i];
    double px,py,pz,qx,qy,qz;
    cin>>px>>py>>pz>>qx>>qy>>qz;
    double dx=qx-px, dy=qy-py, dz=qz-pz;
    double dl=sqrt(dx*dx+dy*dy+dz*dz);
    dx/=dl; dy/=dl; dz/=dl;
    
    vector<int> seq;
    
    while((int)seq.size() <= 10){
        int best=-1; double bestT=1e18;
        for(int i=0;i<n;i++){
            double ox=px-cx[i], oy=py-cy[i], oz=pz-cz[i];
            double b = ox*dx+oy*dy+oz*dz;
            double c = ox*ox+oy*oy+oz*oz - cr[i]*cr[i];
            double disc = b*b - c;
            double tol = 1e-6;
            if(disc < -tol) continue;
            double sq = disc < 0 ? 0 : sqrt(disc);
            double t1 = -b - sq;
            double t2 = -b + sq;
            double t;
            if(t1 > 1e-7) t = t1;
            else if(t2 > 1e-7) t = t2;
            else continue;
            if(t < bestT){ bestT = t; best=i; }
        }
        if(best<0) break;
        seq.push_back(best+1);
        double hx = px + bestT*dx;
        double hy = py + bestT*dy;
        double hz = pz + bestT*dz;
        double nx = (hx-cx[best]);
        double ny = (hy-cy[best]);
        double nz = (hz-cz[best]);
        double nl = sqrt(nx*nx+ny*ny+nz*nz);
        nx/=nl; ny/=nl; nz/=nl;
        double dn = dx*nx+dy*ny+dz*nz;
        double rx = dx - 2*dn*nx;
        double ry = dy - 2*dn*ny;
        double rz = dz - 2*dn*nz;
        double rl = sqrt(rx*rx+ry*ry+rz*rz);
        rx/=rl; ry/=rl; rz/=rl;
        px=hx; py=hy; pz=hz;
        dx=rx; dy=ry; dz=rz;
        px += dx*1e-7; py += dy*1e-7; pz += dz*1e-7;
        if((int)seq.size() > 10) break;
    }
    
    bool etc=false;
    if((int)seq.size() > 10){ etc=true; seq.resize(10); }
    
    for(int i=0;i<(int)seq.size();i++){
        if(i) cout<<' ';
        cout<<seq[i];
    }
    if(etc) cout<<" etc.";
    cout<<"\n";
    return 0;
}
