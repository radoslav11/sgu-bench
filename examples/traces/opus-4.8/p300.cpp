#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; 
    if(!(cin>>n))return 0;
    vector<long long> X(n),Y(n);
    for(int i=0;i<n;i++)cin>>X[i]>>Y[i];
    int m=n-1;
    if(m<=0){cout<<0<<"\n";return 0;}
    vector<long long> len(m);
    for(int i=0;i<m;i++)len[i]=abs(X[i+1]-X[i])+abs(Y[i+1]-Y[i]);
    auto segInt=[&](int i,int j,bool adjacent)->bool{
        long long ax1=X[i],ay1=Y[i],ax2=X[i+1],ay2=Y[i+1];
        long long bx1=X[j],by1=Y[j],bx2=X[j+1],by2=Y[j+1];
        long long axmin=min(ax1,ax2),axmax=max(ax1,ax2);
        long long aymin=min(ay1,ay2),aymax=max(ay1,ay2);
        long long bxmin=min(bx1,bx2),bxmax=max(bx1,bx2);
        long long bymin=min(by1,by2),bymax=max(by1,by2);
        long long ixmin=max(axmin,bxmin),ixmax=min(axmax,bxmax);
        long long iymin=max(aymin,bymin),iymax=min(aymax,bymax);
        if(ixmin>ixmax||iymin>iymax)return false;
        if(ixmin<ixmax||iymin<iymax)return true;
        if(adjacent)return false;
        return true;
    };
    long long ans=0;
    int b=0;
    for(int a=0;a<m;a++){
        if(b<a)b=a;
        while(b<m){
            bool ok=true;
            for(int k=a;k<b&&ok;k++) if(segInt(k,b,k==b-1)) ok=false;
            if(!ok)break;
            b++;
        }
        long long L=0;for(int k=a;k<b;k++)L+=len[k];
        ans=max(ans,L);
    }
    cout<<ans<<"\n";
    return 0;
}
