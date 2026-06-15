#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, r;
    cin >> n >> r;
    
    vector<int> groups(n);
    for(int i = 0; i < n; i++){
        cin >> groups[i];
    }
    
    multiset<int> tables;
    
    for(int i = 0; i < n; i++){
        int remaining = groups[i];
        vector<int> toInsert;
        
        while(remaining > 0){
            if(tables.empty()){
                int take = min(remaining, r);
                remaining -= take;
                toInsert.push_back(r - take);
            } else {
                auto it = prev(tables.end());
                int space = *it;
                tables.erase(it);
                
                if(space >= 2 && remaining >= 2){
                    int take = min(remaining, space);
                    remaining -= take;
                    if(space - take > 0) toInsert.push_back(space - take);
                } else if(space >= remaining && remaining >= 2){
                    int take = remaining;
                    remaining = 0;
                    if(space - take > 0) toInsert.push_back(space - take);
                } else {
                    toInsert.push_back(space);
                }
            }
        }
        
        for(int x : toInsert) tables.insert(x);
    }
    
    cout << tables.size() << endl;
    
    return 0;
}
