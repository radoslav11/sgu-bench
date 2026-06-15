#include <bits/stdc++.h>
using namespace std;

char b[9];

int winner(){
    int lines[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(auto&l:lines){
        if(b[l[0]]!='.'&&b[l[0]]==b[l[1]]&&b[l[1]]==b[l[2]]){
            return b[l[0]]=='X'?1:-1;
        }
    }
    return 0;
}

int minimax(char turn){
    int w=winner();
    if(w!=0) return w;
    bool full=true;
    for(int i=0;i<9;i++) if(b[i]=='.') full=false;
    if(full) return 0;
    if(turn=='X'){
        int best=-2;
        for(int i=0;i<9;i++) if(b[i]=='.'){
            b[i]='X';
            best=max(best,minimax('0'));
            b[i]='.';
        }
        return best;
    } else {
        int best=2;
        for(int i=0;i<9;i++) if(b[i]=='.'){
            b[i]='0';
            best=min(best,minimax('X'));
            b[i]='.';
        }
        return best;
    }
}

int main(){
    string line;
    while(getline(cin,line)){
        if(line.substr(0,2)=="Qc") break;
        string rows[3];
        rows[0]=line;
        for(int i=1;i<3;i++) getline(cin,rows[i]);
        getline(cin,line);
        
        for(int r=0;r<3;r++)
            for(int c=0;c<3;c++)
                b[r*3+c]=rows[r][c];
        
        int cntX=0,cnt0=0;
        for(int i=0;i<9;i++){
            if(b[i]=='X') cntX++;
            else if(b[i]=='0') cnt0++;
        }
        
        bool legal=true;
        if(!(cntX==cnt0||cntX==cnt0+1)) legal=false;
        
        int lines[8][3]={{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
        bool xwin=false, owin=false;
        for(auto&l:lines){
            if(b[l[0]]!='.'&&b[l[0]]==b[l[1]]&&b[l[1]]==b[l[2]]){
                if(b[l[0]]=='X') xwin=true;
                else owin=true;
            }
        }
        if(xwin&&owin) legal=false;
        if(xwin && cntX!=cnt0+1) legal=false;
        if(owin && cntX!=cnt0) legal=false;
        
        if(!legal){
            printf("Illegal position.\n");
            continue;
        }
        
        char turn = (cntX==cnt0) ? 'X' : '0';
        int res;
        if(xwin) res=1;
        else if(owin) res=-1;
        else res=minimax(turn);
        
        if(res==1) printf("X wins.\n");
        else if(res==-1) printf("0 wins.\n");
        else printf("Game is a draw.\n");
    }
    return 0;
}
