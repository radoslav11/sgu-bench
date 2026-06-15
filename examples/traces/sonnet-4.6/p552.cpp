#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n;
    cin >> n;
    
    unordered_map<string, int> cnt;
    cnt.reserve(1 << 20);
    
    for(int i = 0; i < n; i++){
        int k;
        cin >> k;
        vector<string> props(k);
        for(int j = 0; j < k; j++){
            cin >> props[j];
        }
        for(int mask = 1; mask < (1 << k); mask++){
            vector<string> sub;
            for(int b = 0; b < k; b++){
                if(mask & (1 << b)){
                    sub.push_back(props[b]);
                }
            }
            sort(sub.begin(), sub.end());
            string key = "";
            for(int s = 0; s < (int)sub.size(); s++){
                if(s > 0) key += "|";
                key += sub[s];
            }
            cnt[key]++;
        }
    }
    
    int m;
    cin >> m;
    
    while(m--){
        int l;
        cin >> l;
        vector<string> conds(l);
        for(int j = 0; j < l; j++){
            cin >> conds[j];
        }
        sort(conds.begin(), conds.end());
        string key = "";
        for(int s = 0; s < l; s++){
            if(s > 0) key += "|";
            key += conds[s];
        }
        auto it = cnt.find(key);
        if(it != cnt.end()){
            cout << it->second << "\n";
        } else {
            cout << 0 << "\n";
        }
    }
    
    return 0;
}
