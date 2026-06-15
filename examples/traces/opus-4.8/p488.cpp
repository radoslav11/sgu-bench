#include <bits/stdc++.h>
using namespace std;
int main(){
    int T; scanf("%d",&T);
    while(T--){
        int n; scanf("%d",&n);
        vector<long long> a(n);
        for(auto&x:a) scanf("%lld",&x);
        vector<int> upL(n,0), downR(n,0);
        for(int i=1;i<n;i++) if(a[i]>a[i-1]) upL[i]=upL[i-1]+1;
        for(int i=n-2;i>=0;i--) if(a[i]>a[i+1]) downR[i]=downR[i+1]+1;
        int hill=0, dale=0;
        for(int i=0;i<n;i++){
            int up=upL[i], down=downR[i];
            if(up>0&&down>0) hill=max(hill,min(up,down));
        }
        vector<int> downL(n,0), upR(n,0);
        for(int i=1;i<n;i++) if(a[i]<a[i-1]) downL[i]=downL[i-1]+1;
        for(int i=n-2;i>=0;i--) if(a[i]<a[i+1]) upR[i]=upR[i+1]+1;
        for(int i=0;i<n;i++){
            int d=downL[i], u=upR[i];
            if(d>0&&u>0) dale=max(dale,min(d,u));
        }
        printf("%d %d\n",hill,dale);
    }
}
