#include <bits/stdc++.h>
using namespace std;
int main() {
    string line, full = "";
    while (getline(cin, line)) { if (!full.empty()) full += " "; full += line; }
    vector<string> sents;
    string cur = "";
    for (int i = 0; i < (int)full.size(); i++) {
        if (full[i] == '.') { sents.push_back(cur); cur = ""; if (i+1 < (int)full.size() && full[i+1] == ' ') i++; }
        else cur += full[i];
    }
    if (!cur.empty()) sents.push_back(cur);
    auto lower = [](string s){for(auto&c:s)c=tolower(c);return s;};
    int total=0; vector<string> results;
    for (auto &s:sents) {
        if(s.empty()) continue;
        vector<string> words; istringstream iss(s); string w;
        while(iss>>w) words.push_back(w);
        for(int i=0;i<(int)words.size();i++){
            string lw=lower(words[i]);
            if(lw=="can"||lw=="may"||lw=="must"||lw=="should"){
                string res=""; for(int j=0;j<(int)words.size();j++){if(j>0)res+=" ";res+=words[j];if(j==i)res+=" not";}
                results.push_back(res+"."); total++;
            }
            if(lw=="is"||lw=="are"){
                int skip=-1;
                if(i+1<(int)words.size()){string nw=lower(words[i+1]);if(nw=="a"||nw=="the")skip=i+1;}
                string res=""; bool first=true;
                for(int j=0;j<(int)words.size();j++){if(j==skip)continue;if(!first)res+=" ";first=false;res+=words[j];if(j==i)res+=" no";}
                results.push_back(res+"."); total++;
            }
        }
    }
    cout<<total<<"\n"; for(auto&r:results)cout<<r<<"\n";
}
