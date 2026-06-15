#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,l1,l2,p;cin>>n>>l1>>l2>>p;
    double target=p/100.0;
    auto logC=[&](int nn,int kk)->double{if(kk<0||kk>nn)return -1e18;if(kk==0||kk==nn)return 0.0;double r=0;for(int i=0;i<kk;i++)r+=log((double)(nn-i))-log((double)(i+1));return r;};
    vector<double>logw(n+1,-1e18);
    for(int k=0;k<=n;k++){double a=logC(k,l1),b=logC(n-k,l2);if(a>-1e17&&b>-1e17)logw[k]=a+b;}
    double mx=*max_element(logw.begin(),logw.end());
    vector<double>w(n+1,0);double tot=0;
    for(int k=0;k<=n;k++){if(logw[k]>-1e17)w[k]=exp(logw[k]-mx);tot+=w[k];}
    vector<double>pre(n+2,0);for(int k=0;k<=n;k++)pre[k+1]=pre[k]+w[k]/tot;
    int ba=0,bb=n;
    for(int len=0;len<=n;len++){bool f=false;for(int a=0;a+len<=n;a++){int b=a+len;if(pre[b+1]-pre[a]>=target-1e-9){ba=a;bb=b;f=true;break;}}if(f)break;}
    cout<<ba<<" "<<bb<<endl;
}
