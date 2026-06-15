#include <bits/stdc++.h>
using namespace std;

string process(const string& s){
    string res; bool it=false;
    int n=s.size();
    for(int i=0;i<n;){
        char c=s[i];
        if(isalpha((unsigned char)c)){
            if(!it){res+="<i>";it=true;}
            res+=c;i++;
        } else {
            if(it){res+="</i>";it=false;}
            if(c=='+'||c=='-'||c=='*'||c=='/'){
                res+="&nbsp;";res+=c;res+="&nbsp;";i++;
            } else if(c=='^'||c=='_'){
                bool sup=(c=='^'); i++;
                string inner;
                if(i<n&&s[i]=='{'){
                    int depth=1;i++;
                    while(i<n&&depth>0){
                        if(s[i]=='{')depth++;
                        else if(s[i]=='}'){depth--;if(depth==0){i++;break;}}
                        inner+=s[i];i++;
                    }
                } else { if(i<n){inner+=s[i];i++;} }
                res+= sup?"<sup>":"<sub>";
                res+=process(inner);
                res+= sup?"</sup>":"</sub>";
            } else {
                res+=c;i++;
            }
        }
    }
    if(it)res+="</i>";
    return res;
}

int main(){
    string line;
    while(getline(cin,line)){
        size_t a=line.find('$');
        size_t b=line.rfind('$');
        string content;
        if(a!=string::npos&&b!=string::npos&&b>a){
            content=line.substr(a+1,b-a-1);
        }
        string stripped;
        for(char c:content) if(!isspace((unsigned char)c)) stripped+=c;
        cout<<process(stripped)<<"\n";
    }
}
