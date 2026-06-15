#include <bits/stdc++.h>
using namespace std;
int main(){
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        vector<long long>a(n);
        for(int i=0;i<n;i++) scanf("%lld",&a[i]);
        // For hills: peak at j, strictly increasing i..j, strictly decreasing j..k
        // height = a[j] - min(a[i], a[k])
        vector<int> li(n),rd(n),ld(n),ri(n);
        li[0]=0;
        for(int i=1;i<n;i++) li[i]=(a[i]>a[i-1])?li[i-1]:i;
        rd[n-1]=n-1;
        for(int i=n-2;i>=0;i--) rd[i]=(a[i]>a[i+1])?rd[i+1]:i;
        ld[0]=0;
        for(int i=1;i<n;i++) ld[i]=(a[i]<a[i-1])?ld[i-1]:i;
        ri[n-1]=n-1;
        for(int i=n-2;i>=0;i--) ri[i]=(a[i]<a[i+1])?ri[i+1]:i;
        long long bestHill=0,bestDale=0;
        for(int j=0;j<n;j++){
            if(li[j]<j && rd[j]>j){
                long long h=a[j]-min(a[li[j]],a[rd[j]]);
                if(h>bestHill) bestHill=h;
            }
            if(ld[j]<j && ri[j]>j){
                long long d=max(a[ld[j]],a[ri[j]])-a[j];
                if(d>bestDale) bestDale=d;
            }
        }
        printf("%lld %lld\n",bestHill,bestDale);
    }
}
