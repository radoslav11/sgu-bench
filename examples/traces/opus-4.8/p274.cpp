#include <bits/stdc++.h>
using namespace std;

bool isSymbol(char c){
    return (c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-';
}
bool isLetter(char c){
    return (c>='a'&&c<='z')||(c>='A'&&c<='Z');
}
// word: one or more symbols
bool isWord(const string&s){
    if(s.empty())return false;
    for(char c:s) if(!isSymbol(c))return false;
    return true;
}
// prefix: words separated by '.'
bool isPrefix(const string&s){
    if(s.empty())return false;
    // split by '.'
    string cur;
    vector<string> parts;
    for(char c:s){
        if(c=='.'){parts.push_back(cur);cur.clear();}
        else cur.push_back(c);
    }
    parts.push_back(cur);
    for(auto&p:parts) if(!isWord(p))return false;
    return true;
}
bool isDomain(const string&s){
    if(s.size()!=2&&s.size()!=3)return false;
    for(char c:s) if(!isLetter(c))return false;
    return true;
}

int main(){
    int n;
    string line;
    getline(cin,line);
    n=stoi(line);
    for(int i=0;i<n;i++){
        if(!getline(cin,line)) line="";
        // remove trailing \r if any
        while(!line.empty()&&(line.back()=='\r'||line.back()=='\n')) line.pop_back();
        // find '@'
        int atpos=-1; int atcount=0;
        for(int j=0;j<(int)line.size();j++) if(line[j]=='@'){atpos=j;atcount++;}
        bool ok=false;
        if(atcount==1){
            string left=line.substr(0,atpos);
            string right=line.substr(atpos+1);
            // right = prefix . domain ; find last '.'
            int dotpos=-1;
            for(int j=0;j<(int)right.size();j++) if(right[j]=='.') dotpos=j;
            if(dotpos!=-1){
                string rpre=right.substr(0,dotpos);
                string dom=right.substr(dotpos+1);
                if(isPrefix(left)&&isPrefix(rpre)&&isDomain(dom)) ok=true;
            }
        }
        cout<<(ok?"YES":"NO")<<"\n";
    }
    return 0;
}
