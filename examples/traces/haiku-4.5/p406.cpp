#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    cin >> n >> m;
    
    vector<vector<int>> sequences(n);
    
    // Read sequences
    for(int i = 0; i < n; i++){
        int k;
        cin >> k;
        sequences[i].resize(k);
        for(int j = 0; j < k; j++){
            cin >> sequences[i][j];
        }
    }
    
    // Process queries
    for(int q = 0; q < m; q++){
        int l;
        cin >> l;
        
        set<int> mustHave;
        set<int> mustNotHave;
        
        for(int i = 0; i < l; i++){
            int x;
            cin >> x;
            if(x > 0){
                mustHave.insert(x);
            } else {
                mustNotHave.insert(-x);
            }
        }
        
        vector<int> results;
        
        // Check each sequence
        for(int i = 0; i < n; i++){
            set<int> seqSet(sequences[i].begin(), sequences[i].end());
            
            bool valid = true;
            
            // Check if all must-have numbers are present
            for(int num : mustHave){
                if(seqSet.find(num) == seqSet.end()){
                    valid = false;
                    break;
                }
            }
            
            // Check if no must-not-have numbers are present
            if(valid){
                for(int num : mustNotHave){
                    if(seqSet.find(num) != seqSet.end()){
                        valid = false;
                        break;
                    }
                }
            }
            
            if(valid){
                results.push_back(i);
            }
        }
        
        cout << results.size() << "\n";
        for(int idx : results){
            cout << sequences[idx].size();
            for(int num : sequences[idx]){
                cout << " " << num;
            }
            cout << "\n";
        }
    }
    
    return 0;
}
