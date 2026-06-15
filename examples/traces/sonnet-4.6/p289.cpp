#include <bits/stdc++.h>
using namespace std;
char board[9];
bool wins(char c) {
    int w[8][3] = {{0,1,2},{3,4,5},{6,7,8},{0,3,6},{1,4,7},{2,5,8},{0,4,8},{2,4,6}};
    for(auto& line : w) if(board[line[0]]==c && board[line[1]]==c && board[line[2]]==c) return true;
    return false;
}
int minimax(int turn) {
    if(wins('X')) return 1;
    if(wins('0')) return -1;
    bool empty = false;
    for(int i=0;i<9;i++) if(board[i]=='.') { empty=true; break; }
    if(!empty) return 0;
    char c = (turn==1) ? 'X' : '0';
    int best = -turn * 2;
    for(int i=0;i<9;i++) {
        if(board[i]=='.') {
            board[i] = c;
            int val = minimax(-turn);
            board[i] = '.';
            if(turn==1) best = max(best, val);
            else best = min(best, val);
        }
    }
    return best;
}
int main(){
    string line, rows[3];
    while(getline(cin, rows[0])) {
        if(rows[0]=="Qc") break;
        getline(cin, rows[1]); getline(cin, rows[2]); getline(cin, line);
        for(int i=0;i<3;i++) for(int j=0;j<3;j++) board[i*3+j]=rows[i][j];
        int cx=0,co=0;
        for(int i=0;i<9;i++) { if(board[i]=='X') cx++; if(board[i]=='0') co++; }
        bool xw=wins('X'), ow=wins('0'), legal=true;
        if(cx!=co && cx!=co+1) legal=false;
        if(xw&&ow) legal=false;
        if(xw&&cx!=co+1) legal=false;
        if(ow&&cx!=co) legal=false;
        if(!legal) { cout<<"Illegal position.\n"; continue; }
        int turn=(cx==co)?1:-1;
        if(xw) { cout<<"X wins.\n"; continue; }
        if(ow) { cout<<"0 wins.\n"; continue; }
        int r=minimax(turn);
        if(r==1) cout<<"X wins.\n";
        else if(r==-1) cout<<"0 wins.\n";
        else cout<<"Game is a draw.\n";
    }
}
