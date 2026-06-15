#include <bits/stdc++.h>
using namespace std;

int n, m;
vector<int> tree;
void upd(int pos, int val){
    for(; pos<=m; pos+=pos&(-pos)) tree[pos]=max(tree[pos],val);
}
int qry(int pos){
    int r=0;
    for(; pos>0; pos-=pos&(-pos)) r=max(r,tree[pos]);
    return r;
}
void reset(){ fill(tree.begin(), tree.end(), 0); }

int main(){
    scanf("%d",&n);
    vector<int> x(n), y(n);
    for(int i=0;i<n;i++) scanf("%d %d",&x[i],&y[i]);
    
    // compress y
    vector<int> ys(y.begin(), y.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());
    m = ys.size();
    vector<int> yr(n);
    for(int i=0;i<n;i++) yr[i] = lower_bound(ys.begin(), ys.end(), y[i]) - ys.begin() + 1; // 1..m
    
    tree.assign(m+1, 0);
    
    // f[i]: longest chain ending at i, strict x and y increasing
    vector<int> f(n), g(n);
    // sort indices by x asc
    vector<int> idx(n);
    for(int i=0;i<n;i++) idx[i]=i;
    sort(idx.begin(), idx.end(), [&](int a,int b){ return x[a]<x[b]; });
    
    reset();
    int i=0;
    while(i<n){
        int j=i;
        while(j<n && x[idx[j]]==x[idx[i]]) j++;
        // query for all in [i,j)
        for(int k=i;k<j;k++){
            int id=idx[k];
            f[id] = 1 + qry(yr[id]-1);
        }
        // update
        for(int k=i;k<j;k++){
            int id=idx[k];
            upd(yr[id], f[id]);
        }
        i=j;
    }
    
    // g[i]: longest chain starting at i. process x descending, query y greater
    // use rank' = m - yr + 1, strict greater original -> rank' strictly less -> query rank'-1
    reset();
    sort(idx.begin(), idx.end(), [&](int a,int b){ return x[a]>x[b]; });
    i=0;
    while(i<n){
        int j=i;
        while(j<n && x[idx[j]]==x[idx[i]]) j++;
        for(int k=i;k<j;k++){
            int id=idx[k];
            int rp = m - yr[id] + 1;
            g[id] = 1 + qry(rp-1);
        }
        for(int k=i;k<j;k++){
            int id=idx[k];
            int rp = m - yr[id] + 1;
            upd(rp, g[id]);
        }
        i=j;
    }
    
    int L=0;
    for(int k=0;k<n;k++) L=max(L,f[k]);
    
    // in A: f+g-1==L
    vector<char> inA(n,0);
    vector<int> cnt(L+2,0);
    for(int k=0;k<n;k++){
        if(f[k]+g[k]-1==L){
            inA[k]=1;
            cnt[f[k]]++;
        }
    }
    vector<char> inB(n,0);
    for(int k=0;k<n;k++){
        if(inA[k] && cnt[f[k]]==1) inB[k]=1;
    }
    
    // output, indices increasing (original order = index order since we go 0..n-1)
    vector<int> A, B;
    for(int k=0;k<n;k++){
        if(inA[k]) A.push_back(k+1);
        if(inB[k]) B.push_back(k+1);
    }
    
    printf("%d", (int)A.size());
    for(int v:A) printf(" %d", v);
    printf("\n");
    printf("%d", (int)B.size());
    for(int v:B) printf(" %d", v);
    printf("\n");
    return 0;
}
