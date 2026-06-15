#include<bits/stdc++.h>
using namespace std;
int main(){
    int n, m;
    scanf("%d %d", &n, &m);
    vector<long long> deg(n+1, 0);
    for(int i = 0; i < m; i++){
        int u, v;
        scanf("%d %d", &u, &v);
        deg[u]++;
        deg[v]++;
    }
    long long ans = 2LL * m;
    for(int i = 1; i <= n; i++){
        ans += deg[i] * (deg[i] - 1);
    }
    printf("%lld\n", ans);
    return 0;
}
