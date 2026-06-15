#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; cin>>n;
    vector<double>cx(n),cy(n),cr(n);
    for(int i=0;i<n;i++) cin>>cx[i]>>cy[i]>>cr[i];
    double burned=0,inverse=0;
    double dx=0.001;
    double xmin=1e9,xmax=-1e9;
    for(int i=0;i<n;i++){xmin=min(xmin,cx[i]-cr[i]);xmax=max(xmax,cx[i]+cr[i]);}
    for(double x=xmin+dx/2;x<xmax;x+=dx){
        vector<pair<double,int>>evs;
        for(int i=0;i<n;i++){double d=cr[i]*cr[i]-(x-cx[i])*(x-cx[i]);if(d<0)continue;double h=sqrt(d);evs.push_back({cy[i]-h,1});evs.push_back({cy[i]+h,-1});}
        sort(evs.begin(),evs.end());
        int cnt=0;double prev=0;
        for(auto&e:evs){if(cnt>0){if(cnt%2==1)burned+=(e.first-prev)*dx;else inverse+=(e.first-prev)*dx;}prev=e.first;cnt+=e.second;}
    }
    printf("%.5f %.5f\n",burned,inverse);
}
