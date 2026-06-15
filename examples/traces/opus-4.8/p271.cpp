#include <bits/stdc++.h>
using namespace std;
int main(){
    int N,M,K;
    scanf("%d %d %d",&N,&M,&K);
    deque<string> dq;
    vector<string> lower;
    bool rev=false;
    for(int i=0;i<N;i++){
        char buf[16]; scanf("%s",buf); string s=buf;
        if((int)dq.size() < K) dq.push_back(s);
        else lower.push_back(s);
    }
    reverse(lower.begin(), lower.end());
    auto pushTop=[&](const string&s){ if(!rev) dq.push_front(s); else dq.push_back(s); };
    auto popBottom=[&]()->string{ string r; if(!rev){ r=dq.back(); dq.pop_back(); } else { r=dq.front(); dq.pop_front(); } return r; };
    char line[32];
    for(int i=0;i<M;i++){
        scanf("%s",line);
        if(line[0]=='A'){
            string name;
            for(int j=4;line[j] && line[j]!=')';j++) name+=line[j];
            if(K==0) lower.push_back(name);
            else { pushTop(name); if((int)dq.size()>K) lower.push_back(popBottom()); }
        } else rev=!rev;
    }
    if(!rev){ for(auto&s:dq) printf("%s\n",s.c_str()); }
    else { for(auto it=dq.rbegin();it!=dq.rend();++it) printf("%s\n",it->c_str()); }
    for(auto it=lower.rbegin();it!=lower.rend();++it) printf("%s\n",it->c_str());
    return 0;
}
