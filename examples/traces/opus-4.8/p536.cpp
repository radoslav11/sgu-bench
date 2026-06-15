#include <bits/stdc++.h>
using namespace std;

int n, m;
int main(){
    scanf("%d %d", &n, &m);
    vector<string> board(n);
    for(int i=0;i<n;i++){
        cin>>board[i];
    }
    int S=n*m;
    int kingStart=-1;
    vector<int> ptype, ppos;
    vector<int> pieceAt(S, -1);
    for(int r=0;r<n;r++)for(int c=0;c<m;c++){
        char ch=board[r][c];
        int sq=r*m+c;
        if(ch=='*') kingStart=sq;
        else if(ch=='K'){ pieceAt[sq]=(int)ptype.size(); ptype.push_back(0); ppos.push_back(sq);}
        else if(ch=='B'){ pieceAt[sq]=(int)ptype.size(); ptype.push_back(1); ppos.push_back(sq);}
        else if(ch=='R'){ pieceAt[sq]=(int)ptype.size(); ptype.push_back(2); ppos.push_back(sq);}
    }
    int K=ptype.size();
    if(K==0){ printf("0\n"); return 0; }
    int FULL=(1<<K)-1;
    
    // precompute attack maps
    vector<bitset<256>> ATT(1<<K);
    int knightD[8][2]={{1,2},{1,-2},{-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
    int rookD[4][2]={{1,0},{-1,0},{0,1},{0,-1}};
    int bishopD[4][2]={{1,1},{1,-1},{-1,1},{-1,-1}};
    
    for(int mask=0; mask<(1<<K); mask++){
        bitset<256> occ;
        for(int i=0;i<K;i++) if(!((mask>>i)&1)) occ.set(ppos[i]);
        bitset<256> att;
        for(int i=0;i<K;i++){
            if((mask>>i)&1) continue; // captured
            int sq=ppos[i];
            int r=sq/m, c=sq%m;
            if(ptype[i]==0){
                for(int d=0;d<8;d++){
                    int nr=r+knightD[d][0], nc=c+knightD[d][1];
                    if(nr>=0&&nr<n&&nc>=0&&nc<m) att.set(nr*m+nc);
                }
            } else {
                auto &D = (ptype[i]==2)? rookD : bishopD;
                for(int d=0;d<4;d++){
                    int nr=r+D[d][0], nc=c+D[d][1];
                    while(nr>=0&&nr<n&&nc>=0&&nc<m){
                        int ns=nr*m+nc;
                        att.set(ns);
                        if(occ[ns]) break;
                        nr+=D[d][0]; nc+=D[d][1];
                    }
                }
            }
        }
        ATT[mask]=att;
    }
    
    int NM=1<<K;
    vector<int> dist((long long)S*NM, -1);
    auto idx=[&](int pos,int mask){ return pos*NM+mask; };
    
    deque<int> q; // store encoded pos*NM+mask
    dist[idx(kingStart,0)]=0;
    queue<pair<int,int>> bq;
    bq.push({kingStart,0});
    
    int kd[8][2]={{1,0},{-1,0},{0,1},{0,-1},{1,1},{1,-1},{-1,1},{-1,-1}};
    
    int answer=-1;
    while(!bq.empty()){
        auto [pos,mask]=bq.front(); bq.pop();
        int cd=dist[idx(pos,mask)];
        if(mask==FULL){ answer=cd; break; }
        int r=pos/m, c=pos%m;
        for(int d=0;d<8;d++){
            int nr=r+kd[d][0], nc=c+kd[d][1];
            if(nr<0||nr>=n||nc<0||nc>=m) continue;
            int ns=nr*m+nc;
            int pj=pieceAt[ns];
            bool present = (pj!=-1 && !((mask>>pj)&1));
            int newmask;
            if(present){
                newmask = mask | (1<<pj);
                // after capture, ns must not be attacked by remaining
                if(ATT[newmask].test(ns)) continue;
            } else {
                // empty square (no present piece). but could there be a captured piece spot - that's empty, fine
                newmask = mask;
                if(ATT[mask].test(ns)) continue;
            }
            int id=idx(ns,newmask);
            if(dist[id]==-1){
                dist[id]=cd+1;
                bq.push({ns,newmask});
            }
        }
    }
    
    if(answer==-1){
        // check any state with full mask
        for(int pos=0;pos<S;pos++){
            int d=dist[idx(pos,FULL)];
            if(d!=-1 && (answer==-1||d<answer)) answer=d;
        }
    }
    printf("%d\n", answer);
    return 0;
}
