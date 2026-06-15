#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<set<int>> users(M);
    set<set<int>> uniqueSets;
    
    for(int i = 0; i < M; i++){
        int size;
        cin >> size;
        for(int j = 0; j < size; j++){
            int resource;
            cin >> resource;
            users[i].insert(resource);
        }
        uniqueSets.insert(users[i]);
    }
    
    // Greedy: start with individual resources as groups
    // Try to merge groups that don't violate constraints
    vector<set<int>> groups;
    for(int i = 1; i <= N; i++){
        set<int> g;
        g.insert(i);
        groups.push_back(g);
    }
    
    // Try to merge groups while maintaining validity
    bool changed = true;
    while(changed){
        changed = false;
        for(int i = 0; i < groups.size() && !changed; i++){
            for(int j = i+1; j < groups.size() && !changed; j++){
                // Try merging i and j
                set<int> merged = groups[i];
                merged.insert(groups[j].begin(), groups[j].end());
                
                // Check if this merge is valid
                bool valid = true;
                for(auto& user : users){
                    int countMatching = 0;
                    for(int g = 0; g < groups.size(); g++){
                        if(g != i && g != j){
                            set<int> intersection;
                            set_intersection(user.begin(), user.end(), groups[g].begin(), groups[g].end(), inserter(intersection, intersection.begin()));
                            if(intersection.size() > 0) countMatching++;
                        }
                    }
                    if(countMatching > 0){
                        valid = false;
                        break;
                    }
                }
                
                if(valid){
                    groups[i] = merged;
                    groups.erase(groups.begin() + j);
                    changed = true;
                }
            }
        }
    }
    
    cout << groups.size() << endl;
    
    return 0;
}
