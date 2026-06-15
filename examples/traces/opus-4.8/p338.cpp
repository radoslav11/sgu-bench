#include <bits/stdc++.h>
using namespace std;

double X;

// croupier maximizing P(beat target T): threshold at T
double cwSolve(int T){
    double V[23]; bool rl[23];
    for(int sc=21; sc>=1; sc--){
        double stop = (sc>=T)?1.0:0.0;
        double r=0; for(int d=1;d<=6;d++){int t=sc+d; r += (t<=21? V[t]:0.0);} r/=6.0;
        if(r>=stop){V[sc]=r; rl[sc]=true;} else {V[sc]=stop; rl[sc]=false;}
    }
    double prob[23]; for(int i=0;i<23;i++) prob[i]=0;
    double finalp[23]; for(int i=0;i<23;i++) finalp[i]=0;
    prob[0]=1.0;
    for(int sc=0; sc<=21; sc++){
        double p=prob[sc]; if(p==0) continue;
        bool doRoll = (sc==0)? true : rl[sc];
        if(doRoll){
            for(int d=1;d<=6;d++){int t=sc+d; if(t<=21) prob[t]+=p/6.0;}
        } else finalp[sc]+=p;
    }
    double cw=0; for(int sc=T; sc<=21; sc++) cw+=finalp[sc];
    return cw;
}

// croupier maximizing X*beatBig + beatAndrew, returns P(croupier beats andrew)= P(final in [Sa,21])
double solveCroupier(int Sb, int Sa, bool bigBust){
    double V[23]; bool rl[23];
    double bustval = bigBust? X:0.0;
    for(int sc=21; sc>=0; sc--){
        double beatBigStop = bigBust?1.0:((sc>=Sb)?1.0:0.0);
        double beatAndStop = (sc>=Sa)?1.0:0.0;
        double stop = X*beatBigStop + beatAndStop;
        double r=0; for(int d=1;d<=6;d++){int t=sc+d; r += (t<=21? V[t]:bustval);} r/=6.0;
        if(sc==0){ V[sc]=r; rl[sc]=true; }
        else if(r>=stop){V[sc]=r; rl[sc]=true;} else {V[sc]=stop; rl[sc]=false;}
    }
    double prob[23]; for(int i=0;i<23;i++) prob[i]=0;
    double finalp[23]; for(int i=0;i<23;i++) finalp[i]=0;
    prob[0]=1.0;
    for(int sc=0; sc<=21; sc++){
        double p=prob[sc]; if(p==0) continue;
        bool doRoll = (sc==0)? true : rl[sc];
        if(doRoll){
            for(int d=1;d<=6;d++){int t=sc+d; if(t<=21) prob[t]+=p/6.0;}
        } else finalp[sc]+=p;
    }
    double cb=0; for(int sc=Sa; sc<=21; sc++) cb+=finalp[sc];
    return cb;
}

// generic player DP: stopWin[1..21], returns finalp[1..21] and bust
void playerDP(double stopWin[23], double finalp[23], double &bust){
    double V[23]; bool rl[23];
    for(int sc=21; sc>=1; sc--){
        double stop = stopWin[sc];
        double r=0; for(int d=1;d<=6;d++){int t=sc+d; r += (t<=21? V[t]:0.0);} r/=6.0;
        if(r>=stop){V[sc]=r; rl[sc]=true;} else {V[sc]=stop; rl[sc]=false;}
    }
    double prob[23]; for(int i=0;i<23;i++) prob[i]=0;
    for(int i=0;i<23;i++) finalp[i]=0;
    bust=0;
    prob[0]=1.0;
    for(int sc=0; sc<=21; sc++){
        double p=prob[sc]; if(p==0) continue;
        bool doRoll = (sc==0)? true : rl[sc];
        if(doRoll){
            for(int d=1;d<=6;d++){int t=sc+d; if(t<=21) prob[t]+=p/6.0; else bust+=p/6.0;}
        } else finalp[sc]+=p;
    }
}

int main(){
    cin >> X;
    double cw[23];
    for(int T=1;T<=21;T++) cw[T]=cwSolve(T);

    // Big man strategy
    double stopWinBig[23];
    for(int sb=1; sb<=21; sb++) stopWinBig[sb]= 1.0 - cw[sb];
    double pB[23], pBust;
    playerDP(stopWinBig, pB, pBust);

    double answer=0;

    // big not bust
    for(int Sb=1; Sb<=21; Sb++){
        if(pB[Sb]==0) continue;
        double stopWinA[23];
        for(int sa=1; sa<=21; sa++){
            double cb = solveCroupier(Sb, sa, false);
            stopWinA[sa]= 1.0 - cb;
        }
        double finalA[23], bustA;
        playerDP(stopWinA, finalA, bustA);
        double aw=0;
        for(int sa=1; sa<=21; sa++) aw += finalA[sa]*stopWinA[sa];
        answer += pB[Sb]*aw;
    }

    // big bust
    {
        double stopWinA[23];
        for(int sa=1; sa<=21; sa++) stopWinA[sa]= 1.0 - cw[sa];
        double finalA[23], bustA;
        playerDP(stopWinA, finalA, bustA);
        double aw=0;
        for(int sa=1; sa<=21; sa++) aw += finalA[sa]*stopWinA[sa];
        answer += pBust*aw;
    }

    printf("%.6f\n", answer);
    return 0;
}
