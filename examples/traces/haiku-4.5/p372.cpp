#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K, N;
    cin >> K >> N;
    
    vector<tuple<int, int, int>> teas; // cost, kind, index(1-based)
    
    for(int i = 1; i <= N; i++){
        int c, s;
        cin >> c >> s;
        teas.push_back({c, s, i});
    }
    
    sort(teas.begin(), teas.end());
    
    vector<int> selected;
    for(int i = 0; i < K && i < N; i++){
        selected.push_back(i);
    }
    
    if((int)selected.size() < K){
        cout << "Impossible\n";
        return 0;
    }
    
    vector<int> result;
    vector<bool> used(K, false);
    
    for(int pos = 0; pos < K; pos++){
        bool placed = false;
        
        for(int i = 0; i < K; i++){
            if(used[i]) continue;
            
            int kind = get<1>(teas[i]);
            
            if(pos >= 2){
                int kind1 = get<1>(teas[selected[result[pos-1]]]);
                int kind2 = get<1>(teas[selected[result[pos-2]]]);
                if(kind == kind1 && kind == kind2) continue;
            }
            
            result.push_back(i);
            used[i] = true;
            placed = true;
            break;
        }
        
        if(!placed){
            cout << "Impossible\n";
            return 0;
        }
    }
    
    for(int i = 0; i < (int)result.size(); i++){
        if(i > 0) cout << " ";
        cout << get<2>(teas[selected[result[i]]]);
    }
    cout << "\n";
    
    return 0;
}
