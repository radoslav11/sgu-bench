#include <bits/stdc++.h>
using namespace std;

unordered_map<string,string> def;

bool isNum(const string&s){
    if(s.empty())return false;
    for(char c:s) if(!isdigit((unsigned char)c)) return false;
    return true;
}
string canon(string s){
    if(isNum(s)){
        int i=0; while(i+1<(int)s.size()&&s[i]=='0')i++;
        return s.substr(i);
    } else {
        for(char&c:s)c=tolower((unsigned char)c);
        return s;
    }
}
// resolve operand to final key
string resolveKey(string k){
    int steps=0;
    while(def.count(k)){
        k=def[k];
        if(++steps>100000)break;
    }
    return k;
}
long long resolveVal(const string&op){
    string k=resolveKey(canon(op));
    if(isNum(k)) return stoll(k);
    return 0;
}

string S; int pos;
void skip(){ while(pos<(int)S.size()&&isspace((unsigned char)S[pos]))pos++; }

long long parseExpr();
long long parsePow();

long long parseAtom(){
    skip();
    if(pos<(int)S.size()&&S[pos]=='('){
        pos++; long long v=parseExpr(); skip();
        if(pos<(int)S.size()&&S[pos]==')')pos++;
        return v;
    }
    if(pos<(int)S.size()&&(S[pos]=='+'||S[pos]=='-')){
        char c=S[pos];pos++;
        long long v=parseAtom();
        return c=='-'?-v:v;
    }
    // identifier or number
    string tok;
    if(pos<(int)S.size()&&isdigit((unsigned char)S[pos])){
        while(pos<(int)S.size()&&isdigit((unsigned char)S[pos]))tok+=S[pos++];
    } else {
        while(pos<(int)S.size()&&isalnum((unsigned char)S[pos]))tok+=S[pos++];
    }
    return resolveVal(tok);
}
long long parsePow(){
    long long base=parseAtom();
    skip();
    if(pos<(int)S.size()&&S[pos]=='^'){
        pos++;
        long long exp=parsePow();
        long long r=1;
        for(long long i=0;i<exp;i++) r*=base;
        return r;
    }
    return base;
}
long long mydiv(long long a,long long b){
    long long q=llabs(a)/llabs(b);
    if((a<0)!=(b<0))q=-q;
    return q;
}
long long mymod(long long a,long long b){
    long long r=llabs(a)%llabs(b);
    if((a<0)!=(b<0))r=-r;
    return r;
}
long long parseTerm(){
    long long v=parsePow();
    while(true){
        skip();
        if(pos<(int)S.size()&&(S[pos]=='*'||S[pos]=='/'||S[pos]=='%')){
            char c=S[pos];pos++;
            long long r=parsePow();
            if(c=='*')v=v*r;
            else if(c=='/')v=mydiv(v,r);
            else v=mymod(v,r);
        } else break;
    }
    return v;
}
long long parseExpr(){
    long long v=parseTerm();
    while(true){
        skip();
        if(pos<(int)S.size()&&(S[pos]=='+'||S[pos]=='-')){
            char c=S[pos];pos++;
            long long r=parseTerm();
            if(c=='+')v+=r; else v-=r;
        } else break;
    }
    return v;
}

bool createsCycle(const string&a,const string&b){
    // following from b, do we reach a?
    string k=b; int steps=0;
    while(true){
        if(k==a)return true;
        if(!def.count(k))return false;
        k=def[k];
        if(++steps>200000)return false;
    }
}

int main(){
    string line;
    while(getline(cin,line)){
        // trim
        int i=0;while(i<(int)line.size()&&isspace((unsigned char)line[i]))i++;
        if(i>=(int)line.size())continue;
        if(line.compare(i,5,"print")==0){
            S=line.substr(i+5); pos=0;
            cout<<parseExpr()<<"\n";
        } else if(line.compare(i,6,"define")==0){
            string rest=line.substr(i+6);
            stringstream ss(rest);
            string o1,o2; ss>>o1>>o2;
            string a=canon(o1), b=canon(o2);
            if(def.count(a))continue; // already defined
            if(a==b)continue;
            if(createsCycle(a,b))continue;
            def[a]=b;
        }
    }
    return 0;
}
