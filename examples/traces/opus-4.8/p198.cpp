#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> par, rnk, parity;

int findp(int x){
    if(par[x]==x) return x;
    int root=findp(par[x]);
    parity[x]^=parity[par[x]];
    par[x]=root;
    return root;
}

int main(){
    if(!(cin>>N)) return 0;
    vector<double> X(N),Y(N),Rr(N);
    for(int i=0;i<N;i++) cin>>X[i]>>Y[i]>>Rr[i];
    double sx,sy,R;
    cin>>sx>>sy>>R;
    // inflated radius for center = R + r_i
    vector<double> rad(N);
    for(int i=0;i<N;i++) rad[i]=R+Rr[i];
    
    par.resize(N); parity.assign(N,0); rnk.assign(N,0);
    for(int i=0;i<N;i++) par[i]=i;
    
    // ray direction, generic
    double dx=0.7234561, dy=0.6905437;
    
    double eps=1e-7;
    bool trapped=false;
    
    for(int i=0;i<N && !trapped;i++){
        for(int j=i+1;j<N && !trapped;j++){
            double ddx=X[i]-X[j], ddy=Y[i]-Y[j];
            double dist=sqrt(ddx*ddx+ddy*ddy);
            double sumR=rad[i]+rad[j];
            if(dist < sumR - eps){
                // overlapping: edge between center i and j
                // compute ray crossing parity
                double Ax=X[i],Ay=Y[i],Bx=X[j],By=Y[j];
                double sgx=Bx-Ax, sgy=By-Ay;
                double det = -dx*sgy + sgx*dy;
                int p=0;
                if(fabs(det)>1e-15){
                    double aox=Ax-sx, aoy=Ay-sy;
                    double t = ( aox*(-sgy) - (-sgx)*aoy ) / det;
                    double u = ( dx*aoy - dy*aox ) / det;
                    if(t>0 && u>0 && u<1) p=1;
                }
                // union i,j with relation parity p
                int ri=findp(i), rj=findp(j);
                if(ri==rj){
                    int rel = parity[i]^parity[j];
                    if((rel^p)==1){
                        trapped=true;
                    }
                }else{
                    // attach
                    if(rnk[ri]<rnk[rj]) swap(ri,rj),swap(i,j);
                    // we need parity such that parity[i]^parity[j]^newrel = p relative
                    // set par[rj]=ri ; parity[rj] = parity[i]^parity[j]^p
                    int relset = parity[i]^parity[j]^p;
                    par[rj]=ri;
                    parity[rj]=relset;
                    if(rnk[ri]==rnk[rj]) rnk[ri]++;
                }
            }
        }
    }
    
    cout << (trapped? "NO":"YES") << endl;
    return 0;
}
