#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; scanf("%d",&n);
    vector<long long> x1(n),y1(n),x2(n),y2(n);
    for(int i=0;i<n;i++) scanf("%lld %lld %lld %lld",&x1[i],&y1[i],&x2[i],&y2[i]);
    long long count=0;
    for(int i=0;i<n;i++){
        long long dx1=x2[i]-x1[i], dy1=y2[i]-y1[i];
        for(int j=i+1;j<n;j++){
            long long dx2=x2[j]-x1[j], dy2=y2[j]-y1[j];
            if(dx1*dx2+dy1*dy2!=0) continue;
            bool share=false;
            if(x1[i]==x1[j]&&y1[i]==y1[j]) share=true;
            else if(x1[i]==x2[j]&&y1[i]==y2[j]) share=true;
            else if(x2[i]==x1[j]&&y2[i]==y1[j]) share=true;
            else if(x2[i]==x2[j]&&y2[i]==y2[j]) share=true;
            if(share) count++;
        }
    }
    printf("%lld\n",count);
    return 0;
}
