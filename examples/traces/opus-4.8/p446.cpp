#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; if(!(cin>>n)) return 0;
    vector<double> x1(n),y1(n),x2(n),y2(n);
    for(int i=0;i<n;i++) cin>>x1[i]>>y1[i];
    for(int i=0;i<n;i++) cin>>x2[i]>>y2[i];
    double cx1=0,cy1=0,cx2=0,cy2=0;
    for(int i=0;i<n;i++){cx1+=x1[i];cy1+=y1[i];cx2+=x2[i];cy2+=y2[i];}
    cx1/=n;cy1/=n;cx2/=n;cy2/=n;
    for(int i=0;i<n;i++){x1[i]-=cx1;y1[i]-=cy1;x2[i]-=cx2;y2[i]-=cy2;}
    int idx=0; double best=-1;
    for(int i=0;i<n;i++){double d=x1[i]*x1[i]+y1[i]*y1[i]; if(d>best){best=d;idx=i;}}
    double dist1=sqrt(best);
    double a1=atan2(y1[idx],x1[idx]);
    const double PI=acos(-1.0);
    double resDiff=0, resErr=1e18;
    for(int j=0;j<n;j++){
        double d=sqrt(x2[j]*x2[j]+y2[j]*y2[j]);
        if(fabs(d-dist1)>1e-3) continue;
        double a2=atan2(y2[j],x2[j]);
        double theta=a2-a1;
        double c=cos(theta), s=sin(theta), err=0;
        for(int i=0;i<n;i++){
            double rx=x1[i]*c - y1[i]*s, ry=x1[i]*s + y1[i]*c;
            double bm=1e18;
            for(int k=0;k<n;k++){double dx=rx-x2[k],dy=ry-y2[k]; bm=min(bm,dx*dx+dy*dy);}
            err+=bm;
        }
        if(err<resErr){resErr=err; double diff=theta;
            while(diff<0)diff+=2*PI; while(diff>=2*PI)diff-=2*PI; if(diff>PI)diff=2*PI-diff; resDiff=diff;}
    }
    printf("%.10f\n",resDiff);
    return 0;
}
