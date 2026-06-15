#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    if(scanf("%d",&n)!=1) return 0;
    vector<double> x1(n),y1(n),x2(n),y2(n);
    vector<char> col(n);
    for(int i=0;i<n;i++){
        double a,b,c,d; char ch[5];
        scanf("%lf %lf %lf %lf %s",&a,&b,&c,&d,ch);
        // ensure x1<=x2
        if(a>c){swap(a,c);swap(b,d);}
        x1[i]=a;y1[i]=b;x2[i]=c;y2[i]=d;
        col[i]=ch[0];
    }
    vector<double> xs;
    for(int i=0;i<n;i++){
        if(x1[i]==x2[i]) continue; // vertical, zero width
        xs.push_back(x1[i]);
        xs.push_back(x2[i]);
    }
    // intersections
    for(int i=0;i<n;i++){
        if(x1[i]==x2[i]) continue;
        for(int j=i+1;j<n;j++){
            if(x1[j]==x2[j]) continue;
            double px=x1[i],py=y1[i];
            double rx=x2[i]-x1[i],ry=y2[i]-y1[i];
            double qx=x1[j],qy=y1[j];
            double sx=x2[j]-x1[j],sy=y2[j]-y1[j];
            double denom=rx*sy-ry*sx;
            if(fabs(denom)<1e-12) continue;
            double t=((qx-px)*sy-(qy-py)*sx)/denom;
            double u=((qx-px)*ry-(qy-py)*rx)/denom;
            if(t>=-1e-9&&t<=1+1e-9&&u>=-1e-9&&u<=1+1e-9){
                double ix=px+t*rx;
                xs.push_back(ix);
            }
        }
    }
    sort(xs.begin(),xs.end());
    // dedupe
    vector<double> ux;
    for(double v:xs){
        if(ux.empty()||v-ux.back()>1e-7) ux.push_back(v);
    }
    double SR=0,SG=0,SB=0;
    for(size_t k=0;k+1<ux.size();k++){
        double xl=ux[k], xr=ux[k+1];
        double len=xr-xl;
        if(len<=1e-12) continue;
        double xm=(xl+xr)/2.0;
        double bestY=1e18; int best=-1;
        for(int i=0;i<n;i++){
            if(x1[i]==x2[i]) continue;
            if(xm<=x1[i]+1e-12||xm>=x2[i]-1e-12) continue; // must be interior
            double y=y1[i]+(y2[i]-y1[i])*(xm-x1[i])/(x2[i]-x1[i]);
            if(y<bestY-1e-12){bestY=y;best=i;}
        }
        if(best<0) continue;
        if(col[best]=='R') SR+=len;
        else if(col[best]=='G') SG+=len;
        else SB+=len;
    }
    auto pr=[](double v)->string{
        // round to 2 decimals, trim trailing zeros
        char buf[64];
        sprintf(buf,"%.2f",v+1e-9);
        string s(buf);
        // trim
        if(s.find('.')!=string::npos){
            while(s.back()=='0') s.pop_back();
            if(s.back()=='.') s.pop_back();
        }
        return s;
    };
    printf("R %s\n",pr(SR).c_str());
    printf("G %s\n",pr(SG).c_str());
    printf("B %s\n",pr(SB).c_str());
    return 0;
}
