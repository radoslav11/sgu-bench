#include <bits/stdc++.h>
using namespace std;

int N;
int idxg[25][25];
int cx[450], cy[450]; // x=col, y=row
int M;
int nxt[450], prv[450], col[450];
bool vis[450];

void rotate(int x,int y,int k,int &ox,int &oy){
    if(k==0){ox=x;oy=y;}
    else if(k==1){ox=-y;oy=x;}
    else if(k==2){ox=-x;oy=-y;}
    else {ox=y;oy=-x;}
}

bool tryMatch(int k,int vx,int vy){
    for(int i=0;i<M;i++){
        int rx,ry; rotate(cx[i],cy[i],k,rx,ry);
        int nx=rx+vx, ny=ry+vy;
        if(nx>=0&&nx<N&&ny>=0&&ny<N&&idxg[ny][nx]!=-1) nxt[i]=idxg[ny][nx];
        else nxt[i]=-1;
        prv[i]=-1;
    }
    for(int i=0;i<M;i++) if(nxt[i]!=-1) prv[nxt[i]]=i;
    for(int i=0;i<M;i++){vis[i]=false; col[i]=-1;}
    // paths
    for(int i=0;i<M;i++){
        if(prv[i]==-1 && !vis[i]){
            int cur=i, idx=0;
            while(cur!=-1 && !vis[cur]){
                vis[cur]=true;
                col[cur]=idx&1;
                cur=nxt[cur];
                idx++;
            }
            if(idx&1) return false;
        }
    }
    // cycles
    for(int i=0;i<M;i++){
        if(!vis[i]){
            int cur=i, idx=0;
            do{
                vis[cur]=true;
                col[cur]=idx&1;
                cur=nxt[cur];
                idx++;
            }while(cur!=i && cur!=-1);
            // cur should equal i for a cycle
            if(cur!=i) return false; // shouldn't happen
            if(idx&1) return false;
        }
    }
    return true;
}

int main(){
    if(!(cin>>N)) return 0;
    vector<string> g(N);
    for(int i=0;i<N;i++){
        cin>>g[i];
    }
    M=0;
    for(int r=0;r<N;r++)for(int c=0;c<N;c++) idxg[r][c]=-1;
    for(int r=0;r<N;r++)for(int c=0;c<N;c++){
        if(g[r][c]=='1'){
            idxg[r][c]=M;
            cx[M]=c; cy[M]=r;
            M++;
        }
    }
    if(M==0){
        cout<<"YES\n";
        for(int r=0;r<N;r++){
            string s(N,'0');
            cout<<s<<"\n";
        }
        return 0;
    }
    if(M&1){
        cout<<"NO\n";
        return 0;
    }
    
    bool found=false;
    int fk=0,fvx=0,fvy=0;
    for(int k=0;k<4 && !found;k++){
        // gather candidate v's
        unordered_set<int> seen;
        seen.reserve(4096);
        for(int i=0;i<M && !found;i++){
            int rx,ry; rotate(cx[i],cy[i],k,rx,ry);
            for(int j=0;j<M;j++){
                int vx=cx[j]-rx;
                int vy=cy[j]-ry;
                int code=(vx+200)*1000+(vy+200);
                if(seen.count(code)) continue;
                seen.insert(code);
                if(tryMatch(k,vx,vy)){
                    found=true; fk=k; fvx=vx; fvy=vy;
                    break;
                }
            }
        }
    }
    
    if(!found){
        cout<<"NO\n";
        return 0;
    }
    // recompute assignment for fk,fvx,fvy
    tryMatch(fk,fvx,fvy);
    cout<<"YES\n";
    vector<string> out(N, string(N,'0'));
    for(int i=0;i<M;i++){
        if(col[i]==0) out[cy[i]][cx[i]]='1';
    }
    for(int r=0;r<N;r++) cout<<out[r]<<"\n";
    return 0;
}
