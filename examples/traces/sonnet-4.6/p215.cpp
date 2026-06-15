#include<bits/stdc++.h>
using namespace std;
map<string,string>defs;
string norm(string s){for(auto&c:s)c=tolower(c);return s;}
long long resolve(string s){
    set<string>v;
    while(1){
        if(v.count(s))return 0;
        v.insert(s);
        if(defs.count(s))s=defs[s];
        else{
            if(!s.empty()&&(isdigit(s[0])))return stoll(s);
            return 0;
        }
    }
}
bool reaches(string from,string target){
    set<string>v;
    while(1){
        if(from==target)return true;
        if(v.count(from))return false;
        v.insert(from);
        if(defs.count(from))from=defs[from];
        else return false;
    }
}
vector<string>toks;
int tp;
long long expr();
long long pw(long long a,long long b){
    if(b==0)return 1;
    long long h=pw(a,b/2),r=h*h;
    if(b%2)r*=a;
    return r;
}
long long atom(){
    if(tp<(int)toks.size()&&toks[tp]=="("){
        tp++;
        long long v=expr();
        if(tp<(int)toks.size()&&toks[tp]==")")tp++;
        return v;
    }
    return resolve(toks[tp++]);
}
long long unary(){
    if(tp<(int)toks.size()&&(toks[tp]=="+"||toks[tp]=="-")){
        string op=toks[tp++];
        long long v=unary();
        return op=="+"?v:-v;
    }
    return atom();
}
long long factor(){
    long long v=unary();
    if(tp<(int)toks.size()&&toks[tp]=="^"){
        tp++;
        long long r=factor();// right-assoc: recurse
        return pw(v,r);
    }
    return v;
}
// Wait, this is wrong! factor() calls unary() then if ^, calls factor() recursively.
// But the right-hand side of ^ should also go through unary first.
// Actually "factor()" = unary (^ factor)? 
// The right-hand operand of ^ is a factor (for right-assoc).
// But unary has highest priority... 
// Grammar:
// expr = term ((+|-) term)*
// term = factor ((*|/|%) factor)*
// factor = unary (^ factor)?   <- right assoc
// unary = (+|-) unary | atom
// This is correct! Each unary starts fresh.
