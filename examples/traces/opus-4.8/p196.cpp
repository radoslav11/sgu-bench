#include <bits/stdc++.h>
using namespace std;
int main(){
    int n,m;
    scanf("%d %d",&n,&m);
    vector<long long> deg(n+1,0);
    for(int i=0;i<m;i++){
        int a,b;scanf("%d %d",&a,&b);
        deg[a]++;deg[b]++;
    }
    long long s=0;
    for(int i=1;i<=n;i++)s+=deg[i]*deg[i];
    printf("%lld\n",s);
}
