#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<double>x(n),y(n);
    for(int i=0;i<n;i++) cin>>x[i]>>y[i];
    long long ans=0;
    for(int s=0;s<n;s++){
        for(int b=0;b<n;b++) if(b!=s){
            for(int c=b+1;c<n;c++) if(c!=s){
                double bx=x[b]-x[s],by=y[b]-y[s];
                double cx=x[c]-x[s],cy=y[c]-y[s];
                double dot=bx*cx+by*cy;
                if(dot<=0) continue;
                double lb=sqrt(bx*bx+by*by),lc=sqrt(cx*cx+cy*cy);
                double cosA=dot/(lb*lc);
                if(cosA>=1.0) cosA=0.9999999;
                double A=acos(cosA)*180.0/M_PI;
                int k=(int)(90.0/A);
                int d=((int)(floor(1000.0*cosA)))%10;
                if(k>=1 && k==d) ans++;
            }
        }
    }
    cout<<ans<<endl;
}
