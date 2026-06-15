#include <bits/stdc++.h>
using namespace std;

int H,W;
vector<string> g;
int cr[9], cc[9];
int ncells=0;
vector<vector<int>> cellId;
vector<pair<int,int>> edges;

int perm[9];
bool used[9];
int targetR[9], targetC[9];

int bestK = 100;
int bestPerm[9];

int par[9];
int findp(int x){ while(par[x]!=x){par[x]=par[par[x]];x=par[x];} return x;}

void eval(){
    for(int i=0;i<9;i++) par[i]=i;
    int vr[9], vc[9];
    for(int i=0;i<9;i++){
        vr[i]=targetR[perm[i]]-cr[i];
        vc[i]=targetC[perm[i]]-cc[i];
    }
    for(auto &e: edges){
        int a=e.first,b=e.second;
        if(vr[a]==vr[b]&&vc[a]==vc[b]){
            int ra=findp(a),rb=findp(b);
            if(ra!=rb) par[ra]=rb;
        }
    }
    int k=0;
    for(int i=0;i<9;i++) if(findp(i)==i) k++;
    if(k<bestK){
        bestK=k;
        for(int i=0;i<9;i++) bestPerm[i]=perm[i];
    }
}

void rec(int idx){
    if(bestK==1) return;
    if(idx==9){ eval(); return;}
    for(int t=0;t<9;t++){
        if(!used[t]){
            used[t]=true;
            perm[idx]=t;
            rec(idx+1);
            used[t]=false;
        }
    }
}

int main(){
    cin>>H>>W;
    g.resize(H);
    for(int i=0;i<H;i++) cin>>g[i];
    cellId.assign(H, vector<int>(W,-1));
    for(int i=0;i<H;i++)for(int j=0;j<W;j++){
        if(g[i][j]=='X'){
            cr[ncells]=i; cc[ncells]=j;
            cellId[i][j]=ncells;
            ncells++;
        }
    }
    // edges
    for(int i=0;i<H;i++)for(int j=0;j<W;j++){
        if(g[i][j]=='X'){
            int a=cellId[i][j];
            if(j+1<W && g[i][j+1]=='X') edges.push_back({a,cellId[i][j+1]});
            if(i+1<H && g[i+1][j]=='X') edges.push_back({a,cellId[i+1][j]});
        }
    }
    for(int k=0;k<9;k++){ targetR[k]=k/3; targetC[k]=k%3; }
    
    rec(0);
    
    // build components for bestPerm
    for(int i=0;i<9;i++) perm[i]=bestPerm[i];
    for(int i=0;i<9;i++) par[i]=i;
    int vr[9], vc[9];
    for(int i=0;i<9;i++){
        vr[i]=targetR[perm[i]]-cr[i];
        vc[i]=targetC[perm[i]]-cc[i];
    }
    for(auto &e: edges){
        int a=e.first,b=e.second;
        if(vr[a]==vr[b]&&vc[a]==vc[b]){
            int ra=findp(a),rb=findp(b);
            if(ra!=rb) par[ra]=rb;
        }
    }
    // assign letters by order of first appearance
    map<int,int> rootLetter;
    int next=0;
    int letterOf[9];
    for(int i=0;i<9;i++){
        int r=findp(i);
        if(rootLetter.find(r)==rootLetter.end()){
            rootLetter[r]=next++;
        }
        letterOf[i]=rootLetter[r];
    }
    
    cout<<bestK<<"\n";
    // logo with letters
    for(int i=0;i<H;i++){
        string line=g[i];
        for(int j=0;j<W;j++){
            if(g[i][j]=='X'){
                int id=cellId[i][j];
                line[j]=(char)('A'+letterOf[id]);
            }
        }
        cout<<line<<"\n";
    }
    cout<<"\n";
    // 3x3
    char grid3[3][3];
    for(int i=0;i<9;i++){
        int t=perm[i];
        grid3[targetR[t]][targetC[t]]=(char)('A'+letterOf[i]);
    }
    for(int r=0;r<3;r++){
        string line="";
        for(int c=0;c<3;c++) line+=grid3[r][c];
        cout<<line<<"\n";
    }
    return 0;
}
