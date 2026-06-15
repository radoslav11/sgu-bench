#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    multiset<pair<int,int>> segments;
    string op;
    
    while(cin >> op){
        int L, R;
        cin >> L >> R;
        
        if(op == "+"){
            // Count segments inside [L, R]
            int count = 0;
            for(auto& seg : segments){
                // A segment [a,b] is inside [L,R] if L <= a and b <= R
                if(L <= seg.first && seg.second <= R){
                    count++;
                }
            }
            cout << count << "\n";
            segments.insert({L, R});
        }
        else{ // op == "-"
            // Remove one occurrence of segment [L, R]
            auto it = segments.find({L, R});
            if(it != segments.end()){
                segments.erase(it);
            }
        }
    }
    
    return 0;
}
