#include <bits/stdc++.h>
using namespace std;

int n, L;
string s;
string ans;

int distinctCount(const string& t){
    set<string> ss;
    int p = t.size();
    for(int i=0;i<p;i++){
        string cur;
        for(int j=i;j<p;j++){
            cur.push_back(t[j]);
            ss.insert(cur);
        }
    }
    return (int)ss.size();
}

int usedLetters(const string& t){
    bool seen[26]={false};
    for(char c: t) seen[c-'a']=true;
    int cnt=0;
    for(int i=0;i<26;i++) if(seen[i]) cnt++;
    return cnt;
}

bool dfs(){
    int p = s.size();
    int D = distinctCount(s);
    if(D > n) return false;
    if(p == L){
        if(D == n){ ans = s; return true; }
        return false;
    }
    int t = L - p; // remaining chars
    // each remaining char adds at least 1 distinct substring
    if(D + t > n) return false;
    long long maxF = 0;
    for(int k=p+1;k<=L;k++) maxF += k;
    if(D + maxF < n) return false;
    
    int used = usedLetters(s); // letters used are contiguous 'a'..'a'+used-1
    for(int ci=0; ci<=used && ci<26; ci++){
        s.push_back('a'+ci);
        if(dfs()) return true;
        s.pop_back();
    }
    return false;
}

int main(){
    cin >> n;
    // smallest L with L*(L+1)/2 >= n
    L = 1;
    while((long long)L*(L+1)/2 < n) L++;
    s = "";
    dfs();
    cout << ans << "\n";
    return 0;
}
