#include <bits/stdc++.h>
using namespace std;

struct Tok { int type; string s; char c; };
vector<Tok> toks;
size_t tp;

struct Item { bool term; char c; string name; };
typedef vector<Item> Prod;

string lower(string s){ for(auto&ch:s) ch=tolower(ch); return s; }

vector<Prod> parseSegList();

bool peekWord(const string&w){ return tp<toks.size()&&toks[tp].type==0&&toks[tp].s==w; }

vector<Prod> crossAppend(const vector<Prod>&a, const vector<Prod>&b){
    vector<Prod> r;
    for(auto&x:a)for(auto&y:b){ Prod p=x; for(auto&i:y)p.push_back(i); r.push_back(p);}
    return r;
}

vector<Prod> parseSegment(){
    if(peekWord("if")){
        vector<Prod> result;
        bool elseError=false;
        while(true){
            tp++; tp++; tp++;
            char ch=toks[tp].c; tp++;
            tp++; tp++;
            tp++; tp++;
            vector<Prod> inner = parseSegList();
            tp++;
            for(auto&pr:inner){ Prod p; Item it; it.term=true; it.c=ch; p.push_back(it); for(auto&i:pr)p.push_back(i); result.push_back(p);}
            if(peekWord("else")){
                tp++;
                if(peekWord("if")) continue;
                else if(peekWord("error")){ tp++; tp++; elseError=true; break; }
                else break;
            } else { tp++; break; }
        }
        if(!elseError) result.push_back(Prod());
        return result;
    } else {
        string name=toks[tp].s; tp++; tp++;
        vector<Prod> r; Prod p; Item it; it.term=false; it.name=name; p.push_back(it); r.push_back(p);
        return r;
    }
}

vector<Prod> parseSegList(){
    vector<Prod> cur; cur.push_back(Prod());
    while(!(peekWord("end")||peekWord("else"))){
        vector<Prod> seg=parseSegment();
        cur=crossAppend(cur,seg);
    }
    return cur;
}

string formatProd(const Prod&p){
    string r,lit;
    for(auto&it:p){
        if(it.term) lit+=it.c;
        else{ if(!lit.empty()){ r+="'"+lit+"'"; lit.clear(); } r+="<"+it.name+">"; }
    }
    if(!lit.empty()) r+="'"+lit+"'";
    return r;
}

int main(){
    string all;
    { stringstream ss; ss<<cin.rdbuf(); all=ss.str(); }
    size_t i=0,n=all.size();
    while(i<n){
        char c=all[i];
        if(isspace((unsigned char)c)){ i++; continue; }
        if(isalpha((unsigned char)c)){
            string w; while(i<n&&isalpha((unsigned char)all[i])){ w+=all[i]; i++; }
            Tok t; t.type=0; t.s=lower(w); toks.push_back(t);
        } else if(c=='\''){
            i++; char ch=all[i]; i++; i++;
            Tok t; t.type=1; t.c=ch; toks.push_back(t);
        } else { Tok t; t.type=2; t.c=c; t.s=string(1,c); toks.push_back(t); i++; }
    }
    map<string,vector<Prod>> grammar;
    tp=0;
    while(tp<toks.size()){
        if(peekWord("procedure")||peekWord("function")){
            tp++;
            if(tp<toks.size()&&toks[tp].type==0){
                string name=toks[tp].s; tp++;
                if(tp<toks.size()&&toks[tp].type==2&&toks[tp].s==":"){
                    while(tp<toks.size()&&!(toks[tp].type==2&&toks[tp].s==";")) tp++;
                    if(tp<toks.size())tp++;
                    continue;
                }
                if(tp<toks.size()&&toks[tp].type==2&&toks[tp].s==";") tp++;
                if(peekWord("forward")){
                    tp++;
                    if(tp<toks.size()&&toks[tp].type==2&&toks[tp].s==";")tp++;
                    continue;
                } else if(peekWord("begin")){
                    if(name=="skip"||name=="peek"||name=="error"){
                        int depth=0;
                        while(tp<toks.size()){
                            if(peekWord("begin")){depth++;tp++;}
                            else if(peekWord("end")){depth--;tp++; if(depth==0){ if(tp<toks.size()&&toks[tp].type==2&&toks[tp].s==";")tp++; break;}}
                            else tp++;
                        }
                        continue;
                    }
                    tp++;
                    vector<Prod> prods=parseSegList();
                    tp++;
                    if(tp<toks.size()&&toks[tp].type==2&&toks[tp].s==";")tp++;
                    grammar[name]=prods;
                    continue;
                }
            }
        } else tp++;
    }
    vector<string> lines;
    for(auto&kv:grammar){
        vector<string> rhs;
        for(auto&p:kv.second) rhs.push_back(formatProd(p));
        sort(rhs.begin(),rhs.end());
        string ln="<"+kv.first+">::=";
        for(size_t k=0;k<rhs.size();k++){ if(k)ln+="|"; ln+=rhs[k]; }
        lines.push_back(ln);
    }
    sort(lines.begin(),lines.end());
    for(auto&l:lines) cout<<l<<"\n";
    return 0;
}
