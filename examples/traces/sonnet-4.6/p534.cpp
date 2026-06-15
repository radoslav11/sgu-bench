#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const ll INF=1e18;
int n;
struct Edge{int a,b;ll t,p;};
vector<pair<int,int>> adj[100005];
vector<Edge> edges;
ll d[100005];
int par_e[100005];
void bfs(int s){fill(d,d+n+1,-1LL);queue<int>q;q.push(s);d[s]=0;par_e[s]=-1;while(!q.empty()){int u=q.front();q.pop();for(auto[v,i]:adj[u])if(d[v]<0){d[v]=d[u]+edges[i].t;par_e[v]=i;q.push(v);}}}
int main(){
scanf("%d",&n);edges.resize(n-1);
for(int i=0;i<n-1;i++){int a,b;ll t,p;scanf("%d%d%lld%lld",&a,&b,&t,&p);edges[i]={a,b,t,p};adj[a].push_back({b,i});adj[b].push_back({a,i});}
bfs(1);int u=max_element(d+1,d+n+1)-d;
bfs(u);int v=max_element(d+1,d+n+1)-d;
ll D=d[v];
vector<ll> du(n+1),dv(n+1);
copy(d,d+n+1,dv.begin());
// get path u->v
vector<int> path_edges;
{int cur=v;while(par_e[cur]!=-1){path_edges.push_back(par_e[cur]);int e=par_e[cur];cur=(edges[e].a==cur?edges[e].b:edges[e].a);}}
bfs(v);copy(d,d+n+1,du.begin());
bfs(u);copy(d,d+n+1,dv.begin());
// For each edge on path, check if removing it breaks ALL diameters
// Find maxdist from each node not using certain edge - hard
// Alternative: find the "core" subpath
// For each node on path, compute max distance to any node
// A node x is in core if: every diameter path passes through x
// x is in core iff du[x]+dv[x]==D and max_other(x)<D where max_other considers branches
// Let's compute for each node: max dist not going toward u and not going toward v
// Build path node sequence
vector<int> path_nodes;
{int cur=v;path_nodes.push_back(cur);while(par_e[cur]!=-1){int e=par_e[cur];cur=(edges[e].a==cur?edges[e].b:edges[e].a);path_nodes.push_back(cur);}reverse(path_nodes.begin(),path_nodes.end());reverse(path_edges.begin(),path_edges.end());}
// du[x] = dist from u, dv[x] = dist from v (already computed above? no - let me redo)
// Actually du = dist from u, dv = dist from v
// For node x on path, max dist to any node = max(du[x], dv[x], max dist through branches)
// We need: for each node x on path, is there a diameter path NOT passing through the edges adjacent to x on the diameter path?
// A diameter path avoids edge (x, next_toward_v) if there's a path of length D not using that edge
// i.e., from u side: can reach some y with du[y] such that there's another path
// Let's just try: find the range [l,r] of path nodes such that for node x in [l,r],
// du[x]+dv[x]==D (all path nodes satisfy this)
// and the branching condition
// For a node x on path: there's a diameter NOT going through the edge toward v iff
// max dist from x through branches (not toward u or v) + du[x] >= D
// Compute for each node on path: max dist through side branches
int m=path_nodes.size();
// We need max dist from each path node ignoring path edges
// Do a DFS/BFS from each path node restricted to non-path edges? 
// Better: compute max dist from each node overall, then the side branch max is:
// for node x: max_dist_overall[x] but we need to exclude path neighbors
// Let me compute for all nodes their max distance using a rerooting technique
// or simply: for each path node x, its max side dist = ?
// Let's do: bfs from all nodes to get eccentricity? No.
// Simpler approach for small n: 
// mark path edges, then for each path node do BFS ignoring path edges
// n=1e5, path length up to 1e5, so O(n) total since each non-path edge visited once
vector<bool> on_path_edge(n-1,false);
for(int e:path_edges)on_path_edge[e]=true;
// BFS from each path node using only non-path edges (forest BFS)
// Actually just one multi-source approach won't work directly
// Let's do: for each connected component when path edges removed, find max dist within component
// Then for each path node, side_max[x] = max dist to any node in its component (excluding path)
vector<ll> side_max(n+1,0);
vector<bool> visited(n+1,false);
for(int x:path_nodes){
if(visited[x])continue;
// BFS from x using non-path edges
queue<int>q;q.push(x);visited[x]=true;
vector<pair<int,ll>> comp;comp.push_back({x,0});
vector<pair<int,ll>> bfsq;bfsq.push_back({x,0});
// use dist array locally
map<int,ll> ld;ld[x]=0;
queue<pair<int,ll>> q2;q2.push({x,0});
// just BFS
while(!q2.empty()){auto[u2,dd]=q2.front();q2.pop();for(auto[v2,i]:adj[u2]){if(!on_path_edge[i]&&ld.find(v2)==ld.end()){ld[v2]=dd+edges[i].t;q2.push({v2,ld[v2]});}}}
// find max dist
ll mx=0;for(auto&[node,dist]:ld)mx=max(mx,dist);
for(auto&[node,dist]:ld){side_max[node]=mx-dist;visited[node]=true;}
}
// Hmm, side_max[x] = max dist from x within its component (non-path edges)
// Now for path node path_nodes[i]:
// - going toward u gives du[path_nodes[i]]
// - going toward v gives dv[path_nodes[i]]  
// - side branch gives side_max[path_nodes[i]]
// A diameter must pass through edge between path_nodes[i] and path_nodes[i+1] 
// (the edge on path toward v from i) IFF:
// there's no other way to achieve distance D
// Actually: edge path_edges[i] (between path_nodes[i] and path_nodes[i+1]) is on ALL diameters iff
// removing it means max dist < D
// After removing edge i on path: 
//   left component (containing u): max dist = max over nodes x in left of (max(du[x], side_max_left[x], ...))
//   Hmm this is getting complex.
// Let me think differently.
// The diameter path edges that are on EVERY diameter:
// edge e=(path_nodes[i], path_nodes[i+1]) is on every diameter iff
// from path_nodes[i] side: du[path_nodes[i]] + side_max[path_nodes[i]] < D
//   AND more precisely: the best path not using e has length < D
// 
// A path not using edge e either:
// 1) stays entirely in left subtree (max = max over left nodes of their eccentricity within left)
// 2) stays entirely in right subtree
// 3) goes from left through some other route - impossible in a tree!
// So removing edge e, the new diameter = max(diam_left, diam_right)
// diam_left = max over pairs in left component
// diam_right = max over pairs in right component
//
// For the path decomposition:
// left component when removing path_edges[i]: contains path_nodes[0..i] and their side branches
// right component: contains path_nodes[i+1..m-1] and their side branches
//
// diam_left: the diameter of the left component
// = max over j<=i, k<=i of (du[path_nodes[j]] - du[path_nodes[k]] + side... ) 
// Actually: max dist in left component between any two nodes a, b
// = for nodes a in left: their distance to any other node in left
// If a is in side branch of path_nodes[j]: dist(a, path_nodes[j]) + path dist + dist(path_nodes[k], b_in_side_k)
// = (side_dist_a_from_j) + (du[path_nodes[k]] - du[path_nodes[j]]) + side_dist_b_from_k  [assuming j<=k]
// This is complex. Let me think of it as:
// For the left component (path_nodes[0..i] with their subtrees):
// Diameter = max over j<=k<=i of [ (max dist from path_nodes[j] into its subtree) + dist(j,k on path) + (max dist from path_nodes[k] into its subtree) ]
// = max over j<=k<=i of [ side_max[path_nodes[j]] + (du[path_nodes[k]] - du[path_nodes[j]]) + side_max[path_nodes[k]] ]
// Also the diameter could be entirely within one subtree = 2*side_max? No, side_max is max from the root, so diam within subtree could be more.
// Hmm, this isn't quite right either since side_max[x] = max dist from x in its subtree component, not the subtree diameter.
// 
// I think I need to track subtree diameters too.
// This is getting complicated. Let me look for a cleaner approach.
//
// Alternative cleaner approach:
// 1. Find diameter D and one diameter path P.
// 2. We must cut at least one edge on P (since P is a diameter path).
// 3. But cutting one edge on P might not reduce the overall diameter if there are other diameter paths not sharing that edge.
// 4. The minimum cost solution: we need to find a set of edges whose removal reduces ALL diameter paths.
//
// Key insight: In a tree, the set of all "diameter paths" - their intersection forms a connected subpath (possibly a single node). 
// Let's find this intersection.
// The intersection is the set of nodes x such that du[x] + dv[x] = D (all on some diameter)
// AND x is on EVERY diameter.
// x is on every diameter iff: side_max[x] + max(du[x], dv[x]) < D
// i.e., no diameter bypasses x through a side branch.
// Wait: a diameter passes through x if it uses x as an intermediate or endpoint.
// A diameter NOT through x: must achieve length D using nodes not passing through x.
// In a tree removing x splits it. The diameter of the original tree not through x
// = max over pairs in same component after removing x.
// This equals max component diameter after removing x.
// If max component diameter = D, then x is NOT on every diameter.
// If max component diameter < D, then x IS on every diameter.
//
// For x on the main diameter path:
// Components when x removed: left part (toward u), right part (toward v), and side branches.
// Max dist in left part between two nodes = similar analysis.
// 
// Let me just compute for each path node x = path_nodes[i]:
// "left_max[i]" = max over j<=i of (du[path_nodes[j]] + side_max[path_nodes[j]])
//   This represents: max dist from any node in left components to reach path_nodes[i]... no.
// 
// Actually: the farthest node from path_nodes[i] in the left component (not going right) is:
// max over j<=i of (dist(path_nodes[i], path_nodes[j]) + side_max[path_nodes[j]])
// = max over j<=i of ( (du[path_nodes[i]] - du[path_nodes[j]]) + side_max[path_nodes[j]] )
// [since j is to the left, du[path_nodes[j]] <= du[path_nodes[i]]]
// Wait, du is dist from u, so du[path_nodes[0]]=0 (u itself), and increases going right.
// dist(path_nodes[i], path_nodes[j]) for j<=i = du[path_nodes[i]] - du[path_nodes[j]]
//
// So farthest_left[i] = max_{j<=i} (du[path_nodes[i]] - du[path_nodes[j]] + side_max[path_nodes[j]])
//                      = du[path_nodes[i]] + max_{j<=i} (side_max[path_nodes[j]] - du[path_nodes[j]])
//
// Similarly farthest_right[i] = max_{j>=i} (du[path_nodes[j]] - du[path_nodes[i]] + side_max[path_nodes[j]])
//                               = -du[path_nodes[i]] + max_{j>=i} (side_max[path_nodes[j]] + du[path_nodes[j]])
//
// The diameter when edge path_edges[i] removed (between path_nodes[i] and path_nodes[i+1]):
// Left component diam = max over pairs j<=k<=i of:
//   side_max[path_nodes[j]] + (du[path_nodes[k]]-du[path_nodes[j]]) + side_max[path_nodes[k]]
//   = max_{j<=k<=i} (side_max[j] - du[j] + du[k] + side_max[k])  [using shorthand]
//   = max_{k<=i} ( du[k]+side_max[k] + max_{j<=k}(side_max[j]-du[j]) )
// Also need to consider diameters entirely within a side subtree. But wait, side_max[x] is the max distance from x in its subtree; the diameter of that subtree isn't captured by just side_max.
// Hmm. I need the diameter of each side subtree too.
//
// Let me compute sub_diam[x] = diameter of the component of x when all path edges removed.
// This can be done with a BFS/DFS from each path node.
//
// Then left_component_diam when removing path_edges[i] = 
//   max( max_{j<=i} sub_diam[path_nodes[j]], 
//        max_{j<k<=i} (side_max[path_nodes[j]] + dist_on_path(j,k) + side_max[path_nodes[k]]) )
// The second term = max_{k<=i} (side_max[path_nodes[k]] + du[path_nodes[k]] + max_{j<=k}(side_max[path_nodes[j]] - du[path_nodes[j]]))
//
// And similarly for right.
// 
// This seems doable! Let me code it up.

printf("starting real solution\n");
return 0;
}
