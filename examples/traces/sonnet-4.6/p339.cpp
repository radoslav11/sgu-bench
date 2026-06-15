#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    multiset<pair<int,int>> segs;
    
    char op;
    int L, R;
    while(cin >> op >> L >> R){
        if(op == '+'){
            int cnt = 0;
            for(auto& s : segs){
                if(s.first >= L && s.second <= R) cnt++;
            }
            cout << cnt << "\n";
            segs.insert({L, R});
        } else {
            auto it = segs.find({L, R});
            if(it != segs.end()) segs.erase(it);
        }
    }
    return 0;
}
