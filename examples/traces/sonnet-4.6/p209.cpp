#include <bits/stdc++.h>
using namespace std;
typedef long double ld;
const ld EPS = 1e-9;
struct pt { ld x, y; };
bool eqpt(pt a, pt b){ return fabsl(a.x-b.x)<EPS && fabsl(a.y-b.y)<EPS; }
int addpt(vector<pt>&v, pt p){ for(int i=0;i<(int)v.size();i++) if(eqpt(v[i],p)) return i; v.push_back(p); return v.size()-1; }
ld ang(pt a, pt b){ return atan2l(b.y-a.y, b.x-a.x); }
int main(){
    int n; cin>>n;
    vector<array<ld,4>> lines(n);
    for(int i=0;i<n;i++){
        ld x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2;
        lines[i]={x1,y1,x2,y2};
    }
    // find all intersection points
    vector<pt> pts;
    // for each line, find all points on it
    vector<vector<int>> linepts(n);
    // intersect line i and j
    auto intersect=[&](int i,int j,pt&p)->bool{
        ld x1=lines[i][0],y1=lines[i][1],x2=lines[i][2],y2=lines[i][3];
        ld x3=lines[j][0],y3=lines[j][1],x4=lines[j][2],y4=lines[j][3];
        ld d=(x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
        if(fabsl(d)<EPS) return false;
        ld t=((x1-x3)*(y3-y4)-(y1-y3)*(x3-x4))/d;
        p.x=x1+t*(x2-x1);
        p.y=y1+t*(y2-y1);
        return true;
    };
    for(int i=0;i<n;i++)
        for(int j=i+1;j<n;j++){
            pt p;
            if(intersect(i,j,p)){
                int id=addpt(pts,p);
                linepts[i].push_back(id);
                linepts[j].push_back(id);
            }
        }
    // remove duplicates in linepts
    for(int i=0;i<n;i++){
        sort(linepts[i].begin(),linepts[i].end());
        linepts[i].erase(unique(linepts[i].begin(),linepts[i].end()),linepts[i].end());
        // sort by position along line
        ld dx=lines[i][2]-lines[i][0], dy=lines[i][3]-lines[i][1];
        sort(linepts[i].begin(),linepts[i].end(),[&](int a,int b){
            ld ta=pts[a].x*dx+pts[a].y*dy;
            ld tb=pts[b].x*dx+pts[b].y*dy;
            return ta<tb;
        });
    }
    // build half-edges
    // each edge segment: consecutive points on a line -> two directed half-edges
    struct HE{ int u,v,twin,next; };
    vector<HE> he;
    // adjacency: for each vertex, list of outgoing half-edge indices
    vector<vector<int>> adj(pts.size());
    auto addedge=[&](int u,int v){
        int id=he.size();
        he.push_back({u,v,-1,-1});
        he.push_back({v,u,-1,-1});
        he[id].twin=id+1;
        he[id+1].twin=id;
        adj[u].push_back(id);
        adj[v].push_back(id+1);
    };
    for(int i=0;i<n;i++){
        for(int j=0;j+1<(int)linepts[i].size();j++){
            addedge(linepts[i][j],linepts[i][j+1]);
        }
    }
    // for each vertex, sort outgoing edges by angle
    // then for half-edge e arriving at v (i.e., twin of e leaves v to u)
    // next half-edge is the one leaving v that comes after direction v->u in CW order
    for(int v=0;v<(int)pts.size();v++){
        sort(adj[v].begin(),adj[v].end(),[&](int a,int b){
            ld aa=atan2l(pts[he[a].v].y-pts[v].y, pts[he[a].v].x-pts[v].x);
            ld bb=atan2l(pts[he[b].v].y-pts[v].y, pts[he[b].v].x-pts[v].x);
            return aa<bb;
        });
    }
    // set next pointers
    // for half-edge e = (u->v), next = ?
    // at v, find the outgoing edge that is twin of e (going v->u direction)
    // then the next in CCW order (i.e., prev in adj[v] sorted by angle) is the next half-edge
    // Actually for planar face traversal (left face):
    // next(e) where e=(u->v): at v, find twin(e) in adj[v], then take the PREVIOUS in CCW = next in CW
    for(int e=0;e<(int)he.size();e++){
        int v=he[e].v;
        int tw=he[e].twin; // tw is outgoing from v
        // find tw in adj[v]
        auto& av=adj[v];
        int pos=-1;
        for(int i=0;i<(int)av.size();i++) if(av[i]==tw){pos=i;break;}
        // next half-edge in the face to the left of e:
        // take the edge just before tw in CCW order (i.e., pos-1 mod size), which is CW next
        int npos=(pos-1+(int)av.size())%(int)av.size();
        he[e].next=av[npos];
    }
    // traverse faces
    vector<bool> visited(he.size(),false);
    vector<ld> areas;
    for(int s=0;s<(int)he.size();s++){
        if(visited[s]) continue;
        // traverse face
        vector<int> face;
        int cur=s;
        while(!visited[cur]){
            visited[cur]=true;
            face.push_back(cur);
            cur=he[cur].next;
        }
        // compute area
        ld area=0;
        for(int e:face){
            pt a=pts[he[e].u], b=pts[he[e].v];
            area+=a.x*b.y-b.x*a.y;
        }
        area/=2;
        if(area>1e-8) areas.push_back(area);
    }
    sort(areas.begin(),areas.end());
    cout<<areas.size()<<"\n";
    cout<<fixed<<setprecision(4);
    for(ld a:areas) cout<<a<<"\n";
}
