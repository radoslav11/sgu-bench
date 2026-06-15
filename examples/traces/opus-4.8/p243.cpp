#include <bits/stdc++.h>
using namespace std;

int N, K;
char grid[6][6];
vector<vector<vector<pair<int,int>>>> orients;
bool used[26];

vector<pair<int,int>> normalize(vector<pair<int,int>> c){
    int mr=INT_MAX,mc=INT_MAX;
    for(auto&p:c){mr=min(mr,p.first);mc=min(mc,p.second);}
    for(auto&p:c){p.first-=mr;p.second-=mc;}
    sort(c.begin(),c.end());
    return c;
}

bool solve(){
    int er=-1,ec=-1;
    for(int i=0;i<N&&er<0;i++)for(int j=0;j<N;j++)if(grid[i][j]=='.'){er=i;ec=j;break;}
    if(er<0) return true;
    for(int p=0;p<K;p++){
        if(used[p]) continue;
        for(auto&o:orients[p]){
            int ar=o[0].first, ac=o[0].second;
            bool ok=true;
            for(auto&d:o){
                int r=er+d.first-ar, c=ec+d.second-ac;
                if(r<0||r>=N||c<0||c>=N||grid[r][c]!='.'){ok=false;break;}
            }
            if(!ok) continue;
            char ch='A'+p;
            for(auto&d:o){int r=er+d.first-ar,c=ec+d.second-ac;grid[r][c]=ch;}
            used[p]=true;
            if(solve()) return true;
            used[p]=false;
            for(auto&d:o){int r=er+d.first-ar,c=ec+d.second-ac;grid[r][c]='.';}
        }
    }
    return false;
}

int main(){
    scanf("%d",&N);
    vector<string> lines(20);
    for(int i=0;i<20;i++) cin>>lines[i];
    map<char,vector<pair<int,int>>> pieces;
    for(int i=0;i<20;i++)for(int j=0;j<20;j++){
        char ch=lines[i][j];
        if(ch>='A'&&ch<='Z') pieces[ch].push_back({i,j});
    }
    K=pieces.size();
    orients.resize(K);
    int idx=0;
    for(auto&pr:pieces){
        auto base=pr.second;
        set<vector<pair<int,int>>> seen;
        auto cur=base;
        for(int r=0;r<4;r++){
            vector<pair<int,int>> rot;
            for(auto&c:cur) rot.push_back({c.second,-c.first});
            cur=rot;
            auto nc=normalize(cur);
            if(seen.insert(nc).second) orients[idx].push_back(nc);
        }
        auto nb=normalize(base);
        if(seen.insert(nb).second) orients[idx].push_back(nb);
        idx++;
    }
    for(int i=0;i<N;i++)for(int j=0;j<N;j++)grid[i][j]='.';
    solve();
    for(int i=0;i<N;i++){for(int j=0;j<N;j++)putchar(grid[i][j]);putchar('\n');}
    return 0;
}
