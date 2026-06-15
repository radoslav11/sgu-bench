#include <bits/stdc++.h>
using namespace std;

int par[400005];
int find(int x){ while(par[x]!=x){ par[x]=par[par[x]]; x=par[x];} return x;}
void uni(int a,int b){ a=find(a); b=find(b); if(a!=b) par[a]=b;}

int main(){
    int k;
    scanf("%d",&k);
    vector<int> xs(k), ys(k);
    vector<int> rowvals, colvals;
    for(int i=0;i<k;i++){
        scanf("%d %d",&xs[i],&ys[i]);
        rowvals.push_back(xs[i]);
        colvals.push_back(ys[i]);
    }
    sort(rowvals.begin(), rowvals.end());
    rowvals.erase(unique(rowvals.begin(),rowvals.end()),rowvals.end());
    sort(colvals.begin(), colvals.end());
    colvals.erase(unique(colvals.begin(),colvals.end()),colvals.end());
    int R=rowvals.size(), C=colvals.size();
    int total=R+C;
    for(int i=0;i<total;i++) par[i]=i;
    for(int i=0;i<k;i++){
        int r=lower_bound(rowvals.begin(),rowvals.end(),xs[i])-rowvals.begin();
        int c=lower_bound(colvals.begin(),colvals.end(),ys[i])-colvals.begin();
        uni(r, R+c);
    }
    vector<long long> rcnt(total,0), ccnt(total,0);
    for(int i=0;i<R;i++) rcnt[find(i)]++;
    for(int i=0;i<C;i++) ccnt[find(R+i)]++;
    long long ans=0;
    for(int i=0;i<total;i++){
        ans += rcnt[i]*ccnt[i];
    }
    printf("%lld\n", ans);
    return 0;
}
