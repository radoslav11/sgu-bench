#include <bits/stdc++.h>
using namespace std;

int R, C, N, M; // M=N+2
int EXITS, FAILS;
vector<char> letters;
vector<int> nextCell;
vector<int> A; // current state per start cell 0..N-1

int moveCell(int cell, char arrow){
    int r=cell/C, col=cell%C;
    switch(arrow){
        case '<': col--; break;
        case '>': col++; break;
        case '^': r--; break;
        case 'v': r++; break;
        case 'V': r++; break;
    }
    if(r<0||r>=R||col<0||col>=C) return EXITS;
    return r*C+col;
}

void processLiteral(const string &L){
    for(char ch: L){
        for(int c=0;c<N;c++){
            if(A[c]==FAILS) continue;
            int s=A[c];
            if(s==EXITS){ A[c]=FAILS; continue; }
            if(letters[s]!=ch){ A[c]=FAILS; continue; }
            A[c]=nextCell[s];
        }
    }
}

void processBlock(const string &F, long long K){
    // build g over all states
    vector<int> g(M);
    g[EXITS]=FAILS; g[FAILS]=FAILS;
    for(int s=0;s<N;s++){
        int cur=s;
        for(char ch: F){
            if(cur==EXITS || cur==FAILS){ cur=FAILS; break; }
            if(letters[cur]!=ch){ cur=FAILS; break; }
            cur=nextCell[cur];
        }
        g[s]=cur;
    }
    // h = g^K
    vector<int> h(M), base(M), tmp(M);
    for(int i=0;i<M;i++) h[i]=i;
    base=g;
    long long k=K;
    while(k>0){
        if(k&1){
            for(int i=0;i<M;i++) h[i]=base[h[i]];
        }
        k>>=1;
        if(k>0){
            for(int i=0;i<M;i++) tmp[i]=base[base[i]];
            base=tmp;
        }
    }
    // apply
    for(int c=0;c<N;c++){
        if(A[c]==FAILS) continue;
        A[c]=h[A[c]];
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    if(!(cin>>R>>C)) return 0;
    vector<string> arr(R), lett(R);
    for(int i=0;i<R;i++) cin>>arr[i];
    for(int i=0;i<R;i++) cin>>lett[i];
    N=R*C; M=N+2; EXITS=N; FAILS=N+1;
    letters.assign(N,'a');
    nextCell.assign(N,0);
    for(int r=0;r<R;r++){
        for(int c=0;c<C;c++){
            int idx=r*C+c;
            letters[idx]=lett[r][c];
            nextCell[idx]=moveCell(idx, arr[r][c]);
        }
    }
    int Q; cin>>Q;
    while(Q--){
        string w; cin>>w;
        A.assign(N,0);
        for(int c=0;c<N;c++) A[c]=c;
        int i=0, n=(int)w.size();
        while(i<n){
            if(w[i]=='('){
                i++;
                string F;
                while(i<n && w[i]!=')'){ F+=w[i]; i++; }
                i++; // skip ')'
                long long K=0;
                while(i<n && isdigit((unsigned char)w[i])){ K=K*10+(w[i]-'0'); i++; }
                processBlock(F,K);
            } else {
                string L;
                while(i<n && w[i]!='('){ L+=w[i]; i++; }
                processLiteral(L);
            }
        }
        int found=-1;
        for(int c=0;c<N;c++){
            if(A[c]!=FAILS){ found=c; break; }
        }
        if(found<0) cout<<"NO\n";
        else{
            int r=found/C, c=found%C;
            cout<<"YES ("<<(r+1)<<","<<(c+1)<<")\n";
        }
    }
    return 0;
}
