#include <bits/stdc++.h>
using namespace std;

int main(){
    string s;
    getline(cin, s);
    int n = s.size();
    // Use two arrays for linked list. Position 0 is a sentinel (leftmost).
    // We'll allocate nodes for characters.
    vector<int> nxt(n+1), prv(n+1);
    vector<char> ch(n+1);
    // node 0 = sentinel head
    nxt[0] = -1;
    prv[0] = -1;
    int cursor = 0; // cursor is at a node, insertions go after cursor
    int cnt = 0;
    for(int i=0;i<n;i++){
        char c = s[i];
        if(c=='L'){
            if(prv[cursor]!=-1) cursor = prv[cursor];
        } else if(c=='R'){
            if(nxt[cursor]!=-1) cursor = nxt[cursor];
        } else {
            cnt++;
            int id = cnt;
            ch[id]=c;
            int after = nxt[cursor];
            nxt[cursor]=id;
            prv[id]=cursor;
            nxt[id]=after;
            if(after!=-1) prv[after]=id;
            cursor = id;
        }
    }
    string res;
    res.reserve(cnt);
    int cur = nxt[0];
    while(cur!=-1){
        res.push_back(ch[cur]);
        cur = nxt[cur];
    }
    printf("%s\n", res.c_str());
    return 0;
}
