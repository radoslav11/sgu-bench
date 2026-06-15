#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
// buttons: L=0,U=1,R=2,D=3
const char IDX2CHAR[4] = {'L','U','R','D'};

bool legalPos(int lb,int rb){
    if(lb==rb) return false;
    if(lb==2 && rb==0) return false; // left on RIGHT, right on LEFT illegal
    return true;
}

int stIdx(int lb,int rb,int p){ return lb*16+rb*4+p; }

int main(){
    int N;
    scanf("%d",&N);
    string s;
    {
        char buf[2000];
        scanf("%s",buf);
        s = string(buf);
    }
    auto reqOf=[&](char c)->int{
        switch(c){
            case 'L': return 0;
            case 'U': return 1;
            case 'R': return 2;
            case 'D': return 3;
            default: return -1; // N
        }
    };
    
    vector<vector<int>> dp(N+1, vector<int>(64, INF));
    vector<vector<int>> par(N+1, vector<int>(64, -1));
    
    int start = stIdx(0,2,0); // left L, right R, none pressed
    dp[0][start]=0;
    
    for(int i=0;i<N;i++){
        int req = reqOf(s[i]);
        for(int idx=0; idx<64; idx++){
            if(dp[i][idx]==INF) continue;
            int lb = idx/16;
            int rb = (idx/4)%4;
            int p = idx%4;
            if(!legalPos(lb,rb)) continue;
            int base = dp[i][idx];
            
            // helper lambda to relax
            auto relax=[&](int nlb,int nrb,int np,int cost){
                if(!legalPos(nlb,nrb)) return;
                int nidx = stIdx(nlb,nrb,np);
                if(base+cost < dp[i+1][nidx]){
                    dp[i+1][nidx]=base+cost;
                    par[i+1][nidx]=idx;
                }
            };
            
            // Action 1: no press, no move (only if req==-1)
            if(req==-1){
                relax(lb,rb,0,0);
            }
            // Action 2: press one leg, no move
            if(req==-1 || req==lb) relax(lb,rb,1,1);
            if(req==-1 || req==rb) relax(lb,rb,2,1);
            
            // determine which legs didn't press / pressed previous beat
            // p: 0 none,1 left,2 right,3 both
            bool a3left=false, a3right=false; // action3 move leg that didn't press
            bool a4left=false, a4right=false; // action4 move leg that pressed
            if(p==0){ a3left=true; a3right=true; /* a4 none */ }
            else if(p==1){ a3right=true; a4left=true; }
            else if(p==2){ a3left=true; a4right=true; }
            else { /*p==3 both pressed*/ a4left=true; a4right=true; }
            
            // Action 3: move leg that didn't press, cost 3
            if(a3left){
                for(int v=0;v<4;v++){
                    if(v==lb||v==rb) continue;
                    if(req!=-1 && req!=v) continue;
                    relax(v,rb,1,3);
                }
            }
            if(a3right){
                for(int v=0;v<4;v++){
                    if(v==lb||v==rb) continue;
                    if(req!=-1 && req!=v) continue;
                    relax(lb,v,2,3);
                }
            }
            // Action 4: move leg that pressed, cost 9
            if(a4left){
                for(int v=0;v<4;v++){
                    if(v==lb||v==rb) continue;
                    if(req!=-1 && req!=v) continue;
                    relax(v,rb,1,9);
                }
            }
            if(a4right){
                for(int v=0;v<4;v++){
                    if(v==lb||v==rb) continue;
                    if(req!=-1 && req!=v) continue;
                    relax(lb,v,2,9);
                }
            }
            // Action 5: jump both, cost 10
            for(int nlb=0;nlb<4;nlb++){
                for(int nrb=0;nrb<4;nrb++){
                    if(nlb==nrb) continue;
                    if(req!=-1 && req!=nlb && req!=nrb) continue;
                    relax(nlb,nrb,3,10);
                }
            }
        }
    }
    
    int best=INF, bestState=-1;
    for(int idx=0; idx<64; idx++){
        if(dp[N][idx]<best){ best=dp[N][idx]; bestState=idx; }
    }
    
    // reconstruct
    vector<int> states(N+1, -1);
    int cur=bestState;
    for(int i=N;i>=1;i--){
        states[i]=cur;
        cur=par[i][cur];
    }
    
    printf("%d\n", best);
    for(int i=1;i<=N;i++){
        int idx=states[i];
        int lb=idx/16;
        int rb=(idx/4)%4;
        printf("%c%c\n", IDX2CHAR[lb], IDX2CHAR[rb]);
    }
    return 0;
}
