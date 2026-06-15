#include <bits/stdc++.h>
using namespace std;
int main(){
    int n; 
    if(scanf("%d",&n)!=1)return 0;
    vector<long long> x(n),y(n);
    for(int i=0;i<n;i++) scanf("%lld %lld",&x[i],&y[i]);
    // check duplicate points
    {
        vector<pair<long long,long long>> p(n);
        for(int i=0;i<n;i++)p[i]={x[i],y[i]};
        sort(p.begin(),p.end());
        for(int i=1;i<n;i++) if(p[i]==p[i-1]){printf("0\n");return 0;}
    }
    vector<int> hpair(n,-1), vpair(n,-1);
    // horizontal: group by y, sort by x
    vector<int> idx(n); for(int i=0;i<n;i++) idx[i]=i;
    // sort by y then x
    sort(idx.begin(),idx.end(),[&](int a,int b){
        if(y[a]!=y[b])return y[a]<y[b];
        return x[a]<x[b];
    });
    long long total=0;
    {
        int i=0;
        while(i<n){
            int j=i;
            while(j<n && y[idx[j]]==y[idx[i]]) j++;
            int cnt=j-i;
            if(cnt%2!=0){printf("0\n");return 0;}
            for(int k=i;k<j;k+=2){
                int a=idx[k],b=idx[k+1];
                hpair[a]=b; hpair[b]=a;
                total += llabs(x[a]-x[b]);
            }
            i=j;
        }
    }
    // vertical: sort by x then y
    sort(idx.begin(),idx.end(),[&](int a,int b){
        if(x[a]!=x[b])return x[a]<x[b];
        return y[a]<y[b];
    });
    {
        int i=0;
        while(i<n){
            int j=i;
            while(j<n && x[idx[j]]==x[idx[i]]) j++;
            int cnt=j-i;
            if(cnt%2!=0){printf("0\n");return 0;}
            for(int k=i;k<j;k+=2){
                int a=idx[k],b=idx[k+1];
                vpair[a]=b; vpair[b]=a;
                total += llabs(y[a]-y[b]);
            }
            i=j;
        }
    }
    // now traverse: each vertex has hpair and vpair. Build single cycle.
    // start at 0, alternate edges. Begin via horizontal edge.
    vector<char> vis(n,0);
    // count cycles using alternation
    int visited=0;
    // traverse one cycle starting at 0
    {
        int cur=0; bool useH=true; // next edge to use is horizontal
        int count=0;
        // We must alternate H,V,H,V around polygon
        while(true){
            if(vis[cur]) break;
            vis[cur]=1; count++; visited++;
            int nxt = useH? hpair[cur]: vpair[cur];
            cur=nxt; useH=!useH;
        }
        // check we returned to 0 and used all? 
    }
    if(visited!=n){printf("0\n");return 0;}
    // Also verify alternation consistency: returning to start with correct parity
    // verify it's a proper closed alternating cycle
    // re-traverse to ensure cycle closes properly
    {
        vector<char> v2(n,0);
        int cur=0; bool useH=true;
        for(int step=0;step<n;step++){
            if(v2[cur]){printf("0\n");return 0;}
            v2[cur]=1;
            cur = useH? hpair[cur]: vpair[cur];
            useH=!useH;
        }
        if(cur!=0){printf("0\n");return 0;}
        // after n steps with alternation, n must be even for parity to return as horizontal start
        if(n%2!=0){printf("0\n");return 0;}
    }
    printf("%lld\n",total);
    return 0;
}
