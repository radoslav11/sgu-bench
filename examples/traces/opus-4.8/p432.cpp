#include <bits/stdc++.h>
using namespace std;
string low(string s){for(auto&c:s)c=tolower(c);return s;}
int main(){
    std::stringstream ss;
    ss<<cin.rdbuf();
    string text=ss.str();
    string clean;
    for(char c:text){ if(c=='\n'||c=='\r')continue; clean+=c;}
    text=clean;
    vector<vector<string>> sentences;
    {
        string cur;
        vector<string> parts;
        for(char c:text){
            if(c=='.'){ parts.push_back(cur); cur="";}
            else cur+=c;
        }
        for(auto&p:parts){
            int i=0; while(i<(int)p.size()&&p[i]==' ')i++;
            string s=p.substr(i);
            if(s.empty())continue;
            vector<string> words;
            string w;
            for(char c:s){ if(c==' '){ if(!w.empty()){words.push_back(w);w="";} } else w+=c;}
            if(!w.empty())words.push_back(w);
            if(!words.empty())sentences.push_back(words);
        }
    }
    vector<string> results;
    for(auto&words:sentences){
        int n=words.size();
        for(int i=0;i<n;i++){
            string lw=low(words[i]);
            if(lw=="can"||lw=="may"||lw=="must"||lw=="should"){
                vector<string> nw;
                for(int j=0;j<n;j++){nw.push_back(words[j]); if(j==i)nw.push_back("not");}
                string out; for(int j=0;j<(int)nw.size();j++){ if(j)out+=' '; out+=nw[j];} out+='.';
                results.push_back(out);
            }
            if(lw=="is"||lw=="are"){
                vector<string> nw;
                int skip=-1;
                if(i+1<n){ string nx=low(words[i+1]); if(nx=="a"||nx=="the") skip=i+1;}
                for(int j=0;j<n;j++){
                    if(j==skip)continue;
                    nw.push_back(words[j]);
                    if(j==i)nw.push_back("no");
                }
                string out; for(int j=0;j<(int)nw.size();j++){ if(j)out+=' '; out+=nw[j];} out+='.';
                results.push_back(out);
            }
        }
    }
    cout<<results.size()<<"\n";
    for(auto&r:results)cout<<r<<"\n";
}
