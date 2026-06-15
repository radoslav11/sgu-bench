#include <bits/stdc++.h>
using namespace std;

int H, W, N;
vector<string> g;
int dr[4]={-1,1,0,0}, dc[4]={0,0,-1,1};
int opp[4]={1,0,3,2};
char pchar[4]={'U','D','L','R'};
char wchar[4]={'u','d','l','r'};

inline bool wall(int idx){ return g[idx/W][idx%W]=='#'; }

int stampCnt=0;
vector<int> bstamp, bdist;
void bfsDist(int box,int src){
    stampCnt++;
    queue<int>q;
    bdist[src]=0; bstamp[src]=stampCnt; q.push(src);
    while(!q.empty()){
        int u=q.front();q.pop();
        int r=u/W,c=u%W;
        for(int k=0;k<4;k++){
            int nr=r+dr[k], nc=c+dc[k];
            if(nr<0||nr>=H||nc<0||nc>=W) continue;
            int ni=nr*W+nc;
            if(ni==box) continue;
            if(g[nr][nc]=='#') continue;
            if(bstamp[ni]==stampCnt) continue;
            bstamp[ni]=stampCnt; bdist[ni]=bdist[u]+1; q.push(ni);
        }
    }
}

int stampCnt2=0;
vector<int> pstamp, pcell, pmove;
string bfsPath(int box,int src,int dst){
    if(src==dst) return "";
    stampCnt2++;
    queue<int>q;
    pstamp[src]=stampCnt2; pcell[src]=-1; q.push(src);
    bool found=false;
    while(!q.empty()){
        int u=q.front();q.pop();
        if(u==dst){found=true;break;}
        int r=u/W,c=u%W;
        for(int k=0;k<4;k++){
            int nr=r+dr[k], nc=c+dc[k];
            if(nr<0||nr>=H||nc<0||nc>=W) continue;
            int ni=nr*W+nc;
            if(ni==box) continue;
            if(g[nr][nc]=='#') continue;
            if(pstamp[ni]==stampCnt2) continue;
            pstamp[ni]=stampCnt2; pcell[ni]=u; pmove[ni]=k; q.push(ni);
        }
    }
    string s;
    int u=dst;
    while(u!=src){
        s+=wchar[pmove[u]];
        u=pcell[u];
    }
    reverse(s.begin(),s.end());
    return s;
}

inline int neigh(int cell,int side){
    int r=cell/W, c=cell%W;
    int nr=r+dr[side], nc=c+dc[side];
    return nr*W+nc;
}
inline bool inb(int r,int c){ return r>=0&&r<H&&c>=0&&c<W; }
inline bool freecell(int cell){
    int r=cell/W,c=cell%W;
    if(!inb(r,c)) return false;
    return g[r][c]!='#';
}

int main(){
    string line;
    while(getline(cin,line)) g.push_back(line);
    if(g.empty()){ printf("Impossible.\n"); return 0; }
    W=0;
    for(auto&s:g) W=max(W,(int)s.size());
    if(W==0){ printf("Impossible.\n"); return 0; }
    for(auto&s:g){ while((int)s.size()<W) s.push_back(' '); }
    H=g.size();
    N=H*W;
    
    int B0=-1,S0=-1,D=-1;
    for(int r=0;r<H;r++)for(int c=0;c<W;c++){
        char ch=g[r][c];
        if(ch=='@') S0=r*W+c;
        else if(ch=='$') B0=r*W+c;
        else if(ch=='.') D=r*W+c;
    }
    
    if(B0<0||S0<0||D<0){ printf("Impossible.\n"); return 0; }
    
    if(B0==D){
        printf("\n");
        return 0;
    }
    
    bstamp.assign(N,0); bdist.assign(N,0);
    pstamp.assign(N,0); pcell.assign(N,0); pmove.assign(N,0);
    
    int NS=N*4;
    const int INF=INT_MAX/2;
    vector<int> distP(NS,INF), distM(NS,INF), par(NS,-1);
    
    priority_queue< tuple<int,int,int>, vector<tuple<int,int,int>>, greater<tuple<int,int,int>> > pq;
    
    // start states
    bfsDist(B0, S0);
    for(int side=0;side<4;side++){
        int n=neigh(B0,side);
        int r=n/W,c=n%W;
        if(!inb(r,c)) continue;
        if(g[r][c]=='#') continue;
        if(bstamp[n]!=stampCnt) continue; // unreachable
        int d=bdist[n];
        int st=B0*4+side;
        if(make_pair(0,d) < make_pair(distP[st],distM[st])){
            distP[st]=0; distM[st]=d; par[st]=-2;
            pq.push(make_tuple(0,d,st));
        }
    }
    
    int goal=-1;
    while(!pq.empty()){
        auto t=pq.top(); pq.pop();
        int pu=get<0>(t), mu=get<1>(t), st=get<2>(t);
        if(pu!=distP[st]||mu!=distM[st]) continue;
        int box=st/4, side=st%4;
        if(box==D){ goal=st; break; }
        
        int sok=neigh(box,side);
        bfsDist(box, sok);
        // walk transitions
        for(int s2=0;s2<4;s2++){
            if(s2==side) continue;
            int n=neigh(box,s2);
            int r=n/W,c=n%W;
            if(!inb(r,c)) continue;
            if(g[r][c]=='#') continue;
            if(bstamp[n]!=stampCnt) continue;
            int d=bdist[n];
            int nst=box*4+s2;
            int np=pu, nm=mu+d;
            if(make_pair(np,nm) < make_pair(distP[nst],distM[nst])){
                distP[nst]=np; distM[nst]=nm; par[nst]=st;
                pq.push(make_tuple(np,nm,nst));
            }
        }
        // push transition: box moves in direction opp[side]
        int k=opp[side];
        int newB=neigh(box,k);
        int nr=newB/W,nc=newB%W;
        if(inb(nr,nc) && g[nr][nc]!='#'){
            int nst=newB*4+side;
            int np=pu+1, nm=mu+1;
            if(make_pair(np,nm) < make_pair(distP[nst],distM[nst])){
                distP[nst]=np; distM[nst]=nm; par[nst]=st;
                pq.push(make_tuple(np,nm,nst));
            }
        }
    }
    
    if(goal<0){ printf("Impossible.\n"); return 0; }
    
    // reconstruct chain
    vector<int> chain;
    int cur=goal;
    while(true){
        chain.push_back(cur);
        if(par[cur]==-2) break;
        cur=par[cur];
    }
    reverse(chain.begin(),chain.end());
    
    string res;
    // initial walk
    int startSt=chain[0];
    int box0=startSt/4, side0=startSt%4;
    int firstSok=neigh(box0,side0);
    res += bfsPath(B0, S0, firstSok);
    
    for(size_t i=1;i<chain.size();i++){
        int prev=chain[i-1], curst=chain[i];
        int bp=prev/4, bc=curst/4;
        if(bp==bc){
            int sp=prev%4, sc=curst%4;
            int sa=neigh(bp,sp), sb=neigh(bp,sc);
            res += bfsPath(bp, sa, sb);
        } else {
            // push: find direction box moved
            for(int k=0;k<4;k++){
                if(neigh(bp,k)==bc){
                    res += pchar[k];
                    break;
                }
            }
        }
    }
    
    printf("%s\n", res.c_str());
    return 0;
}
