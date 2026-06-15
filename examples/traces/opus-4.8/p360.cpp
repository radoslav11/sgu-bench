#include <bits/stdc++.h>
using namespace std;

string prog;
int N,M;
vector<string> grid;
int rr,cc,dir;
int cmdCount;
vector<pair<int,int>> visited;
set<pair<int,int>> seen;
int dx[4]={-1,0,1,0}, dy[4]={0,1,0,-1};

map<char,string> fbody;
map<char,string> fargs;

void doCommand(char c){
    if(cmdCount>=1000) return;
    cmdCount++;
    if(c=='L') dir=(dir+3)%4;
    else if(c=='R') dir=(dir+1)%4;
    else if(c=='C'){
        int nr=rr+dx[dir], nc=cc+dy[dir];
        if(nr>=0&&nr<N&&nc>=0&&nc<M){
            rr=nr; cc=nc;
            if(grid[rr][cc]=='*' && !seen.count({rr,cc})){
                seen.insert({rr,cc});
                visited.push_back({rr,cc});
            }
        }
    }
}

char evalArg(char tok, map<char,char>&bind){
    if(tok=='L'||tok=='R'||tok=='C') return tok;
    return bind[tok];
}

void execBody(const string&body, map<char,char>&bind){
    int i=0, n=body.size();
    while(i<n){
        if(cmdCount>=1000) return;
        char ch=body[i];
        if(ch=='L'||ch=='R'||ch=='C'){
            doCommand(ch);
            i++;
        } else if(islower((unsigned char)ch)){
            char fname=ch;
            i++; 
            i++; // '('
            vector<char> actuals;
            while(body[i]!=')'){
                char tok=body[i];
                actuals.push_back(evalArg(tok,bind));
                i++;
                if(body[i]==',') i++;
            }
            i++; // ')'
            map<char,char> nb;
            string &an=fargs[fname];
            for(size_t k=0;k<an.size();k++) nb[an[k]]=actuals[k];
            execBody(fbody[fname], nb);
        } else if(isupper((unsigned char)ch)){
            char c=bind[ch];
            doCommand(c);
            i++;
        } else {
            i++;
        }
    }
}

int main(){
    string line;
    getline(cin,line);
    {
        stringstream ss(line);
        ss>>N>>M;
    }
    for(int r=0;r<N;r++){
        getline(cin,line);
        while((int)line.size()<M) line+=' ';
        grid.push_back(line.substr(0,M));
        for(int c=0;c<M;c++) if(grid[r][c]=='R'){rr=r;cc=c;dir=0;}
    }
    string rest, l;
    while(getline(cin,l)) rest+=l+"\n";
    for(char ch: rest) if(!isspace((unsigned char)ch)) prog+=ch;
    
    int i=0, n=prog.size();
    while(i<n){
        char fname=prog[i]; i++;
        i++; // '('
        string args;
        while(prog[i]!=')'){
            char a=prog[i];
            if(a!=',') args+=a;
            i++;
        }
        i++; // ')'
        i++; // '{'
        int depth=1;
        string body;
        while(depth>0){
            char c=prog[i];
            if(c=='{') depth++;
            else if(c=='}'){depth--; if(depth==0){i++;break;}}
            body+=c;
            i++;
        }
        fargs[fname]=args;
        fbody[fname]=body;
    }
    
    map<char,char> empty;
    execBody(fbody['m'], empty);
    
    printf("%d\n",(int)visited.size());
    for(auto&p:visited) printf("%d %d\n",p.first+1,p.second+1);
    return 0;
}
