#include <bits/stdc++.h>
using namespace std;

int countDistinct(const string &s) {
    int n = s.size();
    // suffix automaton
    vector<int> len(2*n+5,0), link(2*n+5,-1), cnt(2*n+5,0);
    vector<array<int,26>> ch(2*n+5);
    for(auto &x:ch) x.fill(-1);
    int last=0,sz=1;
    auto extend=[&](int c){
        int cur=sz++;len[cur]=len[last]+1;
        int p=last;
        while(p!=-1&&ch[p][c]==-1){ch[p][c]=cur;p=link[p];}
        if(p==-1)link[cur]=0;
        else{int q=ch[p][c];if(len[q]==len[p]+1)link[cur]=q;else{int clone=sz++;len[clone]=len[p]+1;link[clone]=link[q];ch[clone]=ch[q];while(p!=-1&&ch[p][c]==q){ch[p][c]=clone;p=link[p];}link[q]=link[cur]=clone;}}
        last=cur;
    };
    for(char c:s)extend(c-'a');
    int res=0;for(int i=1;i<sz;i++)res+=len[i]-len[link[i]];
    return res;
}

int main(){
    int n; cin>>n;
    for(int L=1;L<=30;L++){
        string cur="";
        bool found=false;
        function<bool(int)> solve=[&](int pos)->bool{
            if(pos==L){return countDistinct(cur)==n;}
            for(char c='a';c<='z';c++){
                cur.push_back(c);
                int ds=countDistinct(cur);
                int rem=L-pos-1;
                if(ds+rem<=n && n<=ds+rem*(rem+1)/2+rem*(pos+1)){
                    if(solve(pos+1)){cur.resize(pos);cur.push_back(c);return true;}
                }
                cur.pop_back();
            }
            return false;
        };
        if(solve(0)){cout<<cur;return 0;}
    }
}
