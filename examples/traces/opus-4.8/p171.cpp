#include <bits/stdc++.h>
using namespace std;

int main(){
    int K;
    scanf("%d",&K);
    vector<int> N(K), Q(K);
    for(int i=0;i<K;i++) scanf("%d",&N[i]);
    for(int i=0;i<K;i++) scanf("%d",&Q[i]);
    
    // count total students
    long long total=0;
    for(int i=0;i<K;i++) total+=N[i];
    int n=(int)total;
    
    vector<int> P(n), w(n);
    for(int i=0;i<n;i++) scanf("%d",&P[i]);
    for(int i=0;i<n;i++) scanf("%d",&w[i]);
    
    // zone order sorted by Q descending
    vector<int> zorder(K);
    for(int i=0;i<K;i++) zorder[i]=i;
    sort(zorder.begin(), zorder.end(), [&](int a,int b){
        return Q[a] > Q[b];
    });
    
    vector<int> cap = N; // remaining capacity
    vector<int> assign(n, -1);
    
    // students by weight descending
    vector<int> sorder(n);
    for(int i=0;i<n;i++) sorder[i]=i;
    sort(sorder.begin(), sorder.end(), [&](int a,int b){
        return w[a] > w[b];
    });
    
    for(int si=0; si<n; si++){
        int s = sorder[si];
        int p = P[s];
        // find largest Q < p among zones with capacity
        for(int zi=0; zi<K; zi++){
            int z = zorder[zi];
            if(cap[z]>0 && Q[z] < p){
                assign[s]=z;
                cap[z]--;
                break;
            }
        }
    }
    
    // fill remaining students into remaining capacity
    // collect zones with capacity
    int zptr=0;
    for(int s=0; s<n; s++){
        if(assign[s]==-1){
            // find any zone with capacity
            while(zptr<K && cap[zptr]<=0) zptr++;
            assign[s]=zptr;
            cap[zptr]--;
        }
    }
    
    for(int i=0;i<n;i++){
        printf("%d", assign[i]+1);
        if(i+1<n) printf(" ");
    }
    printf("\n");
    return 0;
}
