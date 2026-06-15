#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<double> x(n),y(n);
    for(int i=0;i<n;i++)cin>>x[i]>>y[i];
    string s; cin>>s;
    vector<double> k(n);
    {int i=0; string cur; for(char c: s){ if(c==':'){k[i++]=stod(cur);cur="";} else cur+=c;} k[i]=stod(cur);}
    double sumK=0; for(double v:k)sumK+=v;
    double S=0; for(int i=0;i<n;i++){int j=(i+1)%n; S+=x[i]*y[j]-x[j]*y[i];} S/=2;
    auto eq=[&](int i,double&a,double&b,double&c){
        int j=(i+1)%n; double Ti=S*k[i]/sumK;
        a=-(y[j]-y[i]); b=(x[j]-x[i]);
        c=2*Ti+(x[j]-x[i])*y[i]-(y[j]-y[i])*x[i];
    };
    double px=0,py=0; bool found=false;
    for(int i=0;i<n&&!found;i++)for(int j=i+1;j<n&&!found;j++){
        double a1,b1,c1,a2,b2,c2; eq(i,a1,b1,c1);eq(j,a2,b2,c2);
        double det=a1*b2-a2*b1;
        if(fabs(det)<1e-9)continue;
        px=(c1*b2-c2*b1)/det; py=(a1*c2-a2*c1)/det; found=true;
    }
    if(!found){cout<<-1<<endl;return 0;}
    for(int i=0;i<n;i++){double a,b,c;eq(i,a,b,c); if(fabs(a*px+b*py-c)>1e-6){cout<<-1<<endl;return 0;}}
    cout<<fixed<<setprecision(10);
    for(int i=0;i<n;i++){int j=(i+1)%n;
        cout<<"3 "<<x[i]<<" "<<y[i]<<" "<<x[j]<<" "<<y[j]<<" "<<px<<" "<<py<<"\n";
    }
    return 0;
}
