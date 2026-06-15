#include <bits/stdc++.h>
using namespace std;
int cnt[10];
int total;
string result;
set<string> failed;

string key(int rem){
    string s;
    for(int i=0;i<10;i++) s+=('0'+cnt[i]);
    s+=('a'+rem);
    return s;
}

bool dfs(int rem, int placed){
    if(placed==total) return rem==0;
    string k=key(rem);
    if(failed.count(k)) return false;
    for(int d=0;d<10;d++){
        if(cnt[d]==0) continue;
        if(placed==0 && d==0) continue;
        cnt[d]--;
        result.push_back('0'+d);
        if(dfs((rem*10+d)%17, placed+1)) { cnt[d]++; return true;}
        result.pop_back();
        cnt[d]++;
    }
    failed.insert(k);
    return false;
}

int main(){
    string n; cin>>n;
    for(char c:n) cnt[c-'0']++;
    total=n.size();
    if(dfs(0,0)) cout<<result<<endl;
    else cout<<-1<<endl;
}
