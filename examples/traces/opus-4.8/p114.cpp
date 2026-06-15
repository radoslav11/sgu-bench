#include <bits/stdc++.h>
using namespace std;
int main(){
    int n;
    if(!(cin>>n))return 0;
    vector<pair<double,double>> c(n);
    double total=0;
    for(int i=0;i<n;i++){cin>>c[i].first>>c[i].second;total+=c[i].second;}
    sort(c.begin(),c.end());
    double half=total/2.0;
    double cum=0;
    double ans=c[0].first;
    for(int i=0;i<n;i++){
        cum+=c[i].second;
        if(cum>=half){ans=c[i].first;break;}
    }
    printf("%.5f\n",ans);
    return 0;
}
