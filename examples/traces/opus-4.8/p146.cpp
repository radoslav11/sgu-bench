#include <bits/stdc++.h>
using namespace std;
int main(){
    string ls; int n;
    cin>>ls>>n;
    long long Lscaled=0;
    {
        size_t dot=ls.find('.');
        string intp, frac;
        if(dot==string::npos){intp=ls;frac="";}
        else{intp=ls.substr(0,dot);frac=ls.substr(dot+1);}
        while(frac.size()<4)frac+='0';
        frac=frac.substr(0,4);
        long long ip=stoll(intp);
        long long fp=stoll(frac);
        Lscaled=ip*10000+fp;
    }
    long long total=0;
    for(int i=0;i<n;i++){
        long long t,v; cin>>t>>v;
        long long d=(t%Lscaled)*(v%Lscaled)%Lscaled;
        long long ds=(d*10000)%Lscaled;
        total=(total+ds)%Lscaled;
    }
    long long other=Lscaled-total;
    long long ans=min(total,other);
    printf("%lld.%04lld\n", ans/10000, ans%10000);
    return 0;
}
