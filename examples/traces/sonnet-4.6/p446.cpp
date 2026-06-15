#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<double>x1(n),y1(n),x2(n),y2(n);
    for(int i=0;i<n;i++)cin>>x1[i]>>y1[i];
    for(int i=0;i<n;i++)cin>>x2[i]>>y2[i];
    double cx1=0,cy1=0,cx2=0,cy2=0;
    for(int i=0;i<n;i++){cx1+=x1[i];cy1+=y1[i];cx2+=x2[i];cy2+=y2[i];}
    cx1/=n;cy1/=n;cx2/=n;cy2/=n;
    for(int i=0;i<n;i++){x1[i]-=cx1;y1[i]-=cy1;x2[i]-=cx2;y2[i]-=cy2;}
    auto verify=[&](double theta)->bool{
        const double eps=1e-4;
        vector<bool>used(n,false);
        for(int i=0;i<n;i++){
            double rx=x1[i]*cos(theta)-y1[i]*sin(theta);
            double ry=x1[i]*sin(theta)+y1[i]*cos(theta);
            bool found=false;
            for(int j=0;j<n;j++){
                if(!used[j]&&fabs(x2[j]-rx)<eps&&fabs(y2[j]-ry)<eps){used[j]=true;found=true;break;}
            }
            if(!found)return false;
        }
        return true;
    };
    double r1=sqrt(x1[0]*x1[0]+y1[0]*y1[0]);
    double best=1e18;
    for(int j=0;j<n;j++){
        double r2=sqrt(x2[j]*x2[j]+y2[j]*y2[j]);
        if(fabs(r1-r2)>1e-4)continue;
        double theta=atan2(x1[0]*y2[j]-y1[0]*x2[j],x1[0]*x2[j]+y1[0]*y2[j]);
        if(verify(theta)){
            double ang=fabs(theta);
            if(ang>M_PI)ang=2*M_PI-ang;
            best=min(best,ang);
        }
    }
    if(best==1e18)best=0;
    printf("%.9f\n",best);
}
