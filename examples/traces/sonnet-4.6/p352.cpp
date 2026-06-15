#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF=1e18;
int par[100005];ll dist_[100005];int dep[100005];
vector<pair<int,ll>>tree[100005];
int in_[100005],out_[100005],timer_=0;
int anc[100005][17];
int n,m;
void dfs(int u,int p,int d,ll dd){dep[u]=d;dist_[u]=dd;in_[u]=timer_++;anc[u][0]=p;for(int i=1;i<17;i++)anc[u][i]=anc[anc[u][i-1]][i-1];for(auto[v,w]:tree[u])if(v!=p){par[v]=u;dfs(v,u,d+1,dd+w);}out_[u]=timer_-1;}
int lca(int u,int v){if(dep[u]<dep[v])swap(u,v);int diff=dep[u]-dep[v];for(int i=0;i<17;i++)if((diff>>i)&1)u=anc[u][i];if(u==v)return u;for(int i=16;i>=0;i--)if(anc[u][i]!=anc[v][i]){u=anc[u][i];v=anc[v][i];}return anc[u][0];}
ll seg[400005];
void upd(int node,int l,int r,int ql,int qr,ll val){if(ql>qr||r<ql||l>qr)return;if(ql<=l&&r<=qr){seg[node]=min(seg[node],val);return;}int mid=(l+r)/2;upd(2*node,l,mid,ql,qr,val);upd(2*node+1,mid+1,r,ql,qr,val);}
ll query(int node,int l,int r,int pos){ll res=seg[node];if(l==r)return res;int mid=(l+r)/2;if(pos<=mid)return min(res,query(2*node,l,mid,pos));else return min(res,query(2*node+1,mid+1,r,pos));}
int main(){
    scanf("%d%d",&n,&m);
    vector<tuple<int,int,ll,int>>edges(m);
    for(auto&[a,b,l,t]:edges){scanf("%d%d%lld%d",&a,&b,&l,&t);if(t)tree[a].push_back({b,l}),tree[b].push_back({a,l});}
    fill(seg,seg+4*(n+1),INF);
    dfs(1,1,0,0);
    for(auto&[a,b,l,t]:edges){
        if(!t){
            // non-tree edge (a,b,l)
            // For node x in subtree(b) not ancestor of a: val=dist_[a]+l-dist_[b]
            // For node x in subtree(a) not ancestor of b: val=dist_[b]+l-dist_[a]
            // handle both directions
            auto handle=[&](int u,int w){
                ll val=dist_[u]+l-dist_[w];
                // add val to subtree(w) minus ancestors of u within subtree(w)
                if(in_[w]>in_[u]||out_[w]<in_[u]){
                    // u not in subtree(w), update whole subtree(w)
                    upd(1,0,n-1,in_[w],out_[w],val);
                } else {
                    // u is in subtree(w), find child of w on path to u
                    int c=u;for(int i=16;i>=0;i--)if(dep[anc[c][i]]>dep[w])c=anc[c][i];
                    // update subtree(w) minus subtree(c)
                    upd(1,0,n-1,in_[w],in_[c]-1,val);
                    upd(1,0,n-1,out_[c]+1,out_[w],val);
                }
            };
            handle(a,b);handle(b,a);
        }
    }
    for(int i=2;i<=n;i++){
        ll q=query(1,0,n-1,in_[i]);
        ll ans=(q>=INF)?-1:q+dist_[i];
        printf("%lld",ans);if(i<n)printf(" ");
    }
    printf("\n");
}
