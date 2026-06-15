#include<bits/stdc++.h>
using namespace std;

int N, K;
vector<int> pref[205]; // preferences for each student
vector<int> assignment[205]; // which students go to each university
int studentAssignment[205]; // which university does each student go to (-1 if none)

bool tryAssignment(vector<int>& universities) {
    // Try to assign students to the given universities
    // Each student goes to at most one university
    // Each university needs at least 2 students
    
    // Reset assignments
    for(int i = 0; i < 205; i++) {
        assignment[i].clear();
        studentAssignment[i] = -1;
    }
    
    // Try backtracking with greedy approach
    // For each university, try to find at least 2 students
    
    function<bool(int)> backtrack = [&](int univIdx) -> bool {
        if(univIdx == K) {
            // Check if all universities have at least 2 students
            for(int u : universities) {
                if(assignment[u].size() < 2) return false;
            }
            return true;
        }
        
        int u = universities[univIdx];
        
        // Find students who can go to this university and aren't assigned yet
        vector<int> candidates;
        for(int s = 0; s < N; s++) {
            if(studentAssignment[s] == -1) {
                for(int pu : pref[s]) {
                    if(pu == u) {
                        candidates.push_back(s);
                        break;
                    }
                }
            }
        }
        
        // Try assigning 2 or more students to this university
        if(candidates.size() < 2) return false;
        
        // Try all combinations of at least 2 students
        int numCandidates = candidates.size();
        for(int mask = 0; mask < (1 << numCandidates); mask++) {
            if(__builtin_popcount(mask) < 2) continue;
            
            // Assign these students
            vector<int> assigned;
            for(int i = 0; i < numCandidates; i++) {
                if(mask & (1 << i)) {
                    int s = candidates[i];
                    assignment[u].push_back(s);
                    studentAssignment[s] = u;
                    assigned.push_back(s);
                }
            }
            
            if(backtrack(univIdx + 1)) return true;
            
            // Unassign
            for(int s : assigned) {
                studentAssignment[s] = -1;
            }
            assignment[u].clear();
        }
        
        return false;
    };
    
    return backtrack(0);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> K;
    
    set<int> allUniversities;
    
    for(int i = 0; i < N; i++) {
        int m;
        cin >> m;
        for(int j = 0; j < m; j++) {
            int u;
            cin >> u;
            pref[i].push_back(u);
            allUniversities.insert(u);
        }
    }
    
    vector<int> univList(allUniversities.begin(), allUniversities.end());
    
    // Try all combinations of K universities
    function<bool(int, vector<int>&)> tryCombinations = [&](int idx, vector<int>& selected) -> bool {
        if(selected.size() == K) {
            if(tryAssignment(selected)) return true;
            return false;
        }
        
        if(idx == univList.size()) return false;
        
        // Include univList[idx]
        selected.push_back(univList[idx]);
        if(tryCombinations(idx + 1, selected)) return true;
        selected.pop_back();
        
        // Don't include univList[idx]
        if(tryCombinations(idx + 1, selected)) return true;
        
        return false;
    };
    
    vector<int> selected;
    if(K == 0 || tryCombinations(0, selected)) {
        cout << "YES\n";
        if(K > 0) {
            for(int u : selected) {
                cout << assignment[u].size();
                for(int s : assignment[u]) {
                    cout << " " << (s + 1);
                }
                cout << "\n";
            }
        }
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
