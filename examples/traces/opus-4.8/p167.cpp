#include <bits/stdc++.h>
using namespace std;

int N,M,K;
int prefix[16][17];
vector<int> dp, par;
int Kp, MM;

inline long long INDEX(int i,int k,int l,int r,int sL,int sR){
    return ((((( (long long)i*Kp + k)*MM + l)*MM + r)*2 + sL)*2 + sR);
}
inline int rowsum(int i,int l,int r){ return prefix[i][r+1]-prefix[i][l]; }

int main(){
    scanf("%d %d %d",&N,&M,&K);
    vector<vector<int>> a(N, vector<int>(M));
    for(int i=0;i<N;i++) for(int j=0;j<M;j++) scanf("%d",&a[i][j]);
    for(int i=0;i<N;i++){
        prefix[i][0]=0;
        for(int j=0;j<M;j++) prefix[i][j+1]=prefix[i][j]+a[i][j];
    }
    if(K==0){
        printf("Oil : 0\n");
        return 0;
    }
    Kp=K+1; MM=M;
    long long total=(long long)N*Kp*MM*MM*4;
    dp.assign(total,-1);
    par.assign(total,-2); // -2 unset, -1 means init/start

    int bestVal=-1; long long bestIdx=-1;

    for(int i=0;i<N;i++){
        // init: this row as starting row
        for(int l=0;l<M;l++){
            for(int r=l;r<M;r++){
                int k=r-l+1;
                if(k>K) break;
                int val=rowsum(i,l,r);
                long long id=INDEX(i,k,l,r,0,0);
                if(val>dp[id]){
                    dp[id]=val;
                    par[id]=-1;
                }
            }
        }
        // update answer with states at row i having k==K
        for(int l=0;l<M;l++) for(int r=l;r<M;r++) for(int sL=0;sL<2;sL++) for(int sR=0;sR<2;sR++){
            long long id=INDEX(i,K,l,r,sL,sR);
            if(dp[id]>bestVal){ bestVal=dp[id]; bestIdx=id; }
        }
        // propagate to i+1
        if(i+1<N){
            for(int k=1;k<=K;k++){
                for(int l=0;l<M;l++) for(int r=l;r<M;r++){
                    if(r-l+1>k) continue;
                    for(int sL=0;sL<2;sL++) for(int sR=0;sR<2;sR++){
                        long long id=INDEX(i,k,l,r,sL,sR);
                        int cur=dp[id];
                        if(cur<0) continue;
                        // choose l',r' for next row
                        // l' range based on sL
                        int lstart = (sL==1)? l : 0;
                        for(int lp=lstart; lp<M; lp++){
                            int nsL = (lp<=l)?0:1;
                            if(sL==1) nsL=1;
                            // r' range based on sR
                            for(int rp=lp; rp<M; rp++){
                                if(sR==1 && rp>r) break; // r' must <= r
                                int nsR;
                                if(sR==1) nsR=1;
                                else nsR=(rp>=r)?0:1;
                                int nk=k+(rp-lp+1);
                                if(nk>K) continue;
                                int nval=cur+rowsum(i+1,lp,rp);
                                long long nid=INDEX(i+1,nk,lp,rp,nsL,nsR);
                                if(nval>dp[nid]){
                                    dp[nid]=nval;
                                    // encode predecessor: (l,r,sL,sR)
                                    par[nid]=(((l*M+r)*2+sL)*2+sR);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // reconstruct
    // decode bestIdx -> i,k,l,r,sL,sR
    // We know it's at row with k==K. Let's decode by recomputing.
    // bestIdx encodes via INDEX formula; decode:
    long long idx=bestIdx;
    int sR=idx%2; idx/=2;
    int sL=idx%2; idx/=2;
    int r=idx%MM; idx/=MM;
    int l=idx%MM; idx/=MM;
    int k=idx%Kp; idx/=Kp;
    int i=(int)idx;

    vector<pair<int,int>> rows; // (l,r) per row, top? we go from bottom row to top
    vector<int> rowIdx;
    int ci=i, ck=k, cl=l, cr=r, csL=sL, csR=sR;
    while(true){
        rows.push_back({cl,cr});
        rowIdx.push_back(ci);
        long long id=INDEX(ci,ck,cl,cr,csL,csR);
        int p=par[id];
        if(p==-1) break;
        int prSR=p%2; p/=2;
        int prSL=p%2; p/=2;
        int prr=p%M; p/=M;
        int prl=p;
        int prk=ck-(cr-cl+1);
        ci=ci-1; ck=prk; cl=prl; cr=prr; csL=prSL; csR=prSR;
    }

    printf("Oil : %d\n",bestVal);
    // rows collected bottom-to-top; reverse for top-to-bottom
    for(int t=(int)rows.size()-1;t>=0;t--){
        int rr=rowIdx[t];
        for(int c=rows[t].first;c<=rows[t].second;c++){
            printf("%d %d\n", rr+1, c+1);
        }
    }
    return 0;
}
