#include <bits/stdc++.h>
using namespace std;

int N;
bool Q[20][20];

// Rotate (r,c) by rot (0=0,1=90cw,2=180,3=270cw) in NxN grid
pair<int,int> rotate_cell(int r, int c, int rot) {
    for(int i=0;i<rot;i++) {
        int nr = c, nc = N-1-r;
        r=nr; c=nc;
    }
    return {r,c};
}

int main(){
    int dummy; // skip "Test #X" line? 
    // Actually input has "Test #1" as first line? Let me re-read.
    // The sample shows "Test #1" but that might just be labeling in the problem display.
    // Let's just read N then the grid.
    string line;
    // Try to read N, skip non-numeric lines
    while(getline(cin,line)){
        if(line.empty()) continue;
        if(isdigit(line[0])){N=stoi(line);break;}
    }
    for(int i=0;i<N;i++){
        while(getline(cin,line)){if(!line.empty()&&(line[0]=='0'||line[0]=='1'))break;}
        for(int j=0;j<N;j++) Q[i][j]=(line[j]=='1');
    }
    int total=0;
    for(int i=0;i<N;i++) for(int j=0;j<N;j++) if(Q[i][j]) total++;
    if(total%2!=0){cout<<"NO\n";return 0;}
    for(int rot=0;rot<4;rot++){
        for(int dr=-(N-1);dr<N;dr++){
            for(int dc=-(N-1);dc<N;dc++){
                // Check if f(r,c)=rotate(r,c,rot)+(dr,dc) maps Q->Q
                bool maps=true;
                for(int i=0;i<N&&maps;i++) for(int j=0;j<N&&maps;j++) if(Q[i][j]){
                    auto [nr,nc]=rotate_cell(i,j,rot);
                    nr+=dr; nc+=dc;
                    if(nr<0||nr>=N||nc<0||nc>=N||!Q[nr][nc]) maps=false;
                }
                if(!maps) continue;
                // Check surjective (bijective since same count)
                // Try to 2-color: A=0, B=1, f(A)=B
                int color[20][20];
                memset(color,-1,sizeof(color));
                bool ok=true;
                for(int i=0;i<N&&ok;i++) for(int j=0;j<N&&ok;j++) if(Q[i][j]&&color[i][j]==-1){
                    // BFS/chain
                    int r=i,c=j;
                    color[r][c]=0;
                    int cr=r,cc=c;
                    while(true){
                        auto [nr,nc]=rotate_cell(cr,cc,rot);
                        nr+=dr;nc+=dc;
                        if(nr==r&&nc==c) break;
                        if(!Q[nr][nc]){ok=false;break;}
                        if(color[nr][nc]!=-1){if(color[nr][nc]!=1-color[cr][cc])ok=false;break;}
                        color[nr][nc]=1-color[cr][cc];
                        cr=nr;cc=nc;
                    }
                }
                if(!ok) continue;
                cout<<"YES\n";
                for(int i=0;i<N;i++){for(int j=0;j<N;j++) cout<<(Q[i][j]&&color[i][j]==0?'1':'0');cout<<'\n';}
                return 0;
            }
        }
    }
    cout<<"NO\n";
}
