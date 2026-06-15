#include<bits/stdc++.h>
using namespace std;
int main(){
    int n;cin>>n;string s;cin>>s;
    auto ci=[](char c){return c=='L'?0:c=='U'?1:c=='R'?2:3;};
    auto cc=[](int i){return i==0?'L':i==1?'U':i==2?'R':'D';};
    const int INF=1e9;
    int SZ=4*4*3;
    auto idx=[](int l,int r,int la){return l*12+r*3+la;};
    vector<vector<int>> dp(n+1,vector<int>(SZ,INF));
    vector<vector<int>> par(n+1,vector<int>(SZ,-1));
    dp[0][idx(0,2,2)]=0;
    for(int i=0;i<n;i++){
        int req=s[i]=='N'?-1:ci(s[i]);
        for(int l=0;l<4;l++)for(int r=0;r<4;r++){if(l==r)continue;if(l==2&&r==0)continue;
        for(int la=0;la<3;la++){
            int cur=dp[i][idx(l,r,la)];if(cur==INF)continue;
            // enumerate next states
            auto try_=[&](int l2,int r2,int la2,int c){
                if(l2==r2)return;if(l2==2&&r2==0)return;
                // check req
                if(req!=-1){if(la2==0&&l2!=req)return;if(la2==1&&r2!=req)return;if(la2==2&&req!=-1)return;}
                int ni=idx(l2,r2,la2);
                if(dp[i+1][ni]>cur+c){dp[i+1][ni]=cur+c;par[i+1][ni]=idx(l,r,la);}
            };
            // action1: no change no press
            if(req==-1){int ni=idx(l,r,2);if(dp[i+1][ni]>cur){dp[i+1][ni]=cur;par[i+1][ni]=idx(l,r,la);}}
            // action2: press left or right
            if(req==-1||req==l)try_(l,r,0,1);
            if(req==-1||req==r)try_(l,r,1,1);
            // action3: move non-last leg
            for(int p=0;p<4;p++){
                if(la!=1&&p!=r){try_(p,r,0,3);}// move left
                if(la!=0&&p!=l){try_(l,p,1,3);}// move right
            }
            // action4: move last leg
            for(int p=0;p<4;p++){
                if(la==0&&p!=r){try_(p,r,0,9);}
                if(la==1&&p!=l){try_(l,p,1,9);}
                if(la==2){for(int p2=0;p2<4;p2++){if(p2!=r)try_(p2,r,0,9);if(p2!=l)try_(l,p2,1,9);}}
            }
            // action5: jump
            for(int l2=0;l2<4;l2++)for(int r2=0;r2<4;r2++){if(l2==r2)continue;if(l2==2&&r2==0)continue;
                if(req!=-1&&l2!=req&&r2!=req)continue;
                // la2 can be 0 or 1 or 2? jump presses both
                // "press both" - last=2 means both? use 2
                int ni=idx(l2,r2,2);// hmm but which pressed last?
                // actually jump: both pressed, use la2=2 as "both"
                if(req==-1||l2==req||r2==req){
                    if(dp[i+1][ni]>cur+10){dp[i+1][ni]=cur+10;par[i+1][ni]=idx(l,r,la);}
                }
            }
        }}
    }
    // find best final state
    int best=INF,bst=-1;
    for(int x=0;x<SZ;x++)if(dp[n][x]<best){best=dp[n][x];bst=x;}
    cout<<best<<"\n";
    vector<int> path(n+1);path[n]=bst;
    for(int i=n;i>0;i--)path[i-1]=par[i][path[i]];
    for(int i=1;i<=n;i++){int st=path[i];int l=st/12,r=(st%12)/3;cout<<cc(l)<<cc(r)<<"\n";}
}
