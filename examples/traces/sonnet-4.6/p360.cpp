#include <bits/stdc++.h>
using namespace std;
struct Function { string name; vector<string> params; string body; };
map<string,Function> funcs;
int N,M,robotR,robotC,robotDir;
char grid[55][55];
vector<pair<int,int>> visited;
set<pair<int,int>> visitedSet;
void executeCmd(char cmd){
    int dr[]={-1,0,1,0},dc[]={0,1,0,-1};
    if(cmd=='L')robotDir=(robotDir+3)%4;
    else if(cmd=='R')robotDir=(robotDir+1)%4;
    else{
        int nr=robotR+dr[robotDir],nc=robotC+dc[robotDir];
        if(nr>=0&&nr<N&&nc>=0&&nc<M){robotR=nr;robotC=nc;}
        pair<int,int>p={robotR,robotC};
        if(grid[robotR][robotC]=='*'&&!visitedSet.count(p)){visited.push_back(p);visitedSet.insert(p);}
    }
}
void executeFunction(const string&,map<string,char>&);
void executeBody(const string&body,map<string,char>&env){
    int i=0,n=body.size();
    while(i<n){
        char c=body[i];
        if(c=='L'||c=='R'||c=='C'){executeCmd(c);i++;}
        else if(isupper(c)){executeCmd(env[string(1,c)]);i++;}
        else if(islower(c)){
            string fn(1,c);i++;i++;
            vector<char>args;
            while(body[i]!=')'){
                char ac=body[i];
                if(ac=='L'||ac=='R'||ac=='C')args.push_back(ac);
                else if(isupper(ac))args.push_back(env[string(1,ac)]);
                i++;
                if(i<n&&body[i]==',')i++;
            }
            i++;
            map<string,char>ne;
            Function&f=funcs[fn];
            for(int j=0;j<(int)f.params.size();j++)ne[f.params[j]]=args[j];
            executeFunction(fn,ne);
        }else i++;
    }
}
void executeFunction(const string&fn,map<string,char>&env){executeBody(funcs[fn].body,env);}
int main(){
    cin>>N>>M;
    for(int i=0;i<N;i++){cin>>grid[i];for(int j=0;j<M;j++)if(grid[i][j]=='R'){robotR=i;robotC=j;}}
    robotDir=0;
    string all,tok;while(cin>>tok)all+=tok;
    int i=0;
    while(i<(int)all.size()){
        if(!islower(all[i])){i++;continue;}
        Function f;f.name=string(1,all[i]);i++;i++;
        while(all[i]!=')'){if(isupper(all[i]))f.params.push_back(string(1,all[i]));i++;}
        i++;i++;
        int depth=1;string body;
        while(depth>0){if(all[i]=='{')depth++;else if(all[i]=='}')depth--;if(depth>0)body+=all[i];i++;}
        f.body=body;funcs[f.name]=f;
    }
    map<string,char>env;
    executeFunction("m",env);
    cout<<visited.size()<<"\n";
    for(auto&p:visited)cout<<p.first+1<<" "<<p.second+1<<"\n";
}
