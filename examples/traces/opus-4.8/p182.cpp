#include <bits/stdc++.h>
using namespace std;

string s;
int pos;

vector<int> ntype; // 0 var, 1 not, 2 binop
vector<int> na, nb; // children / varidx / op
vector<int> nop;

int newVar(int v){ ntype.push_back(0); na.push_back(v); nb.push_back(-1); nop.push_back(-1); return ntype.size()-1; }
int newNot(int c){ ntype.push_back(1); na.push_back(c); nb.push_back(-1); nop.push_back(-1); return ntype.size()-1; }
int newBin(int op,int l,int r){ ntype.push_back(2); na.push_back(l); nb.push_back(r); nop.push_back(op); return ntype.size()-1; }

bool match(const string& t){
    if(pos+(int)t.size()<=(int)s.size() && s.compare(pos,t.size(),t)==0) return true;
    return false;
}

int parseExpr();

int parsePrimary(){
    if(s[pos]=='('){
        pos++; // (
        int e=parseExpr();
        pos++; // )
        return e;
    } else {
        int v=s[pos]-'a';
        pos++;
        return newVar(v);
    }
}

int parseNot(){
    if(pos<(int)s.size() && s[pos]=='!'){
        pos++;
        int c=parseNot();
        return newNot(c);
    }
    return parsePrimary();
}

int parseAnd(){
    int left=parseNot();
    while(pos<(int)s.size() && s[pos]=='&'){
        pos++;
        int r=parseNot();
        left=newBin(1,left,r);
    }
    return left;
}

int parseExpr(){
    int left=parseAnd();
    while(pos<(int)s.size()){
        int op=-1;
        if(match("||")){ op=0; pos+=2; }
        else if(match("<=>")){ op=2; pos+=3; }
        else if(match("=>")){ op=3; pos+=2; }
        else if(match("#")){ op=4; pos+=1; }
        else break;
        int r=parseAnd();
        left=newBin(op,left,r);
    }
    return left;
}

bool assign[10];

bool eval(int node){
    int t=ntype[node];
    if(t==0) return assign[na[node]];
    if(t==1) return !eval(na[node]);
    bool l=eval(na[node]);
    bool r=eval(nb[node]);
    switch(nop[node]){
        case 0: return l||r;
        case 1: return l&&r;
        case 2: return l==r;
        case 3: return (!l)||r;
        case 4: return l^r;
    }
    return false;
}

int main(){
    getline(cin,s);
    // strip trailing whitespace/newline
    while(!s.empty() && (s.back()=='\n'||s.back()=='\r'||s.back()==' ')) s.pop_back();
    pos=0;
    int root=parseExpr();
    
    bool used[10]={false};
    for(int i=0;i<10;i++){
        // check if variable used
    }
    // determine used variables by scanning nodes
    for(int i=0;i<(int)ntype.size();i++){
        if(ntype[i]==0) used[na[i]]=true;
    }
    vector<int> vs;
    for(int i=0;i<10;i++) if(used[i]) vs.push_back(i);
    
    int m=vs.size();
    
    string out;
    out.reserve(40000);
    bool first=true;
    
    int total=1<<m;
    for(int mask=0;mask<total;mask++){
        for(int i=0;i<10;i++) assign[i]=false;
        for(int i=0;i<m;i++){
            if(mask&(1<<i)) assign[vs[i]]=true;
        }
        if(eval(root)){
            // build term
            if(!first) out+="||";
            first=false;
            for(int i=0;i<m;i++){
                if(i) out+="&";
                if(mask&(1<<i)){
                    out+=(char)('a'+vs[i]);
                } else {
                    out+="!";
                    out+=(char)('a'+vs[i]);
                }
            }
        }
    }
    
    if(first){
        // contradiction
        int v=vs.empty()?0:vs[0];
        out+=(char)('a'+v);
        out+="&!";
        out+=(char)('a'+v);
    }
    
    cout<<out<<"\n";
    return 0;
}
