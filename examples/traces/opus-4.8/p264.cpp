#include <bits/stdc++.h>
using namespace std;

int N;
vector<string> manName, womanName;
unordered_map<string,int> manId, womanId;
vector<vector<int>> manPref; // man's list of woman ids in order
vector<vector<int>> womanRank; // womanRank[w][m] = rank
vector<int> manNext, manPartner, womanPartner;

int main(){
    scanf("%d",&N);
    manName.resize(N); womanName.resize(N);
    manPref.assign(N, vector<int>());
    // first read all men lines, but we need woman ids assigned.
    // Read men lines storing names, then woman lines, then build ids.
    // Easier: read raw tokens.
    
    vector<vector<string>> menLines(N), womenLines(N);
    char buf[32];
    auto readToken=[&](string &s)->bool{
        int c;
        // skip whitespace
        while((c=getchar_unlocked())!=EOF && (c==' '||c=='\n'||c=='\r'||c=='\t'));
        if(c==EOF) return false;
        int idx=0;
        while(c!=EOF && c!=' ' && c!='\n' && c!='\r' && c!='\t'){
            buf[idx++]=(char)c;
            c=getchar_unlocked();
        }
        buf[idx]=0;
        s.assign(buf,idx);
        return true;
    };
    
    for(int i=0;i<N;i++){
        menLines[i].resize(N+1);
        for(int j=0;j<=N;j++) readToken(menLines[i][j]);
    }
    for(int i=0;i<N;i++){
        womenLines[i].resize(N+1);
        for(int j=0;j<=N;j++) readToken(womenLines[i][j]);
    }
    
    // assign man ids from men lines first column
    for(int i=0;i<N;i++){
        manName[i]=menLines[i][0];
        manId[manName[i]]=i;
    }
    // assign woman ids from women lines first column
    for(int i=0;i<N;i++){
        womanName[i]=womenLines[i][0];
        womanId[womanName[i]]=i;
    }
    
    // build man pref
    for(int i=0;i<N;i++){
        manPref[i].resize(N);
        for(int j=0;j<N;j++){
            manPref[i][j]=womanId[menLines[i][j+1]];
        }
    }
    // build woman rank
    womanRank.assign(N, vector<int>(N));
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            int m=manId[womenLines[i][j+1]];
            womanRank[i][m]=j;
        }
    }
    
    manNext.assign(N,0);
    manPartner.assign(N,-1);
    womanPartner.assign(N,-1);
    
    // free men queue
    queue<int> freeMen;
    for(int i=0;i<N;i++) freeMen.push(i);
    
    while(!freeMen.empty()){
        int m=freeMen.front(); freeMen.pop();
        if(manPartner[m]!=-1) continue;
        // propose to next woman
        int w=manPref[m][manNext[m]++];
        if(womanPartner[w]==-1){
            womanPartner[w]=m;
            manPartner[m]=w;
        } else {
            int cur=womanPartner[w];
            if(womanRank[w][m] < womanRank[w][cur]){
                // w prefers m
                womanPartner[w]=m;
                manPartner[m]=w;
                manPartner[cur]=-1;
                freeMen.push(cur);
            } else {
                // rejected, stays free
                freeMen.push(m);
            }
        }
    }
    
    // output
    string out;
    out.reserve(N*24);
    out+="YES\n";
    for(int m=0;m<N;m++){
        out+=manName[m];
        out+=' ';
        out+=womanName[manPartner[m]];
        out+='\n';
    }
    fputs(out.c_str(), stdout);
    return 0;
}
