#include<bits/stdc++.h>
using namespace std;

int N, K;
set<set<int>> forbidden;
vector<int> assignment; // assignment[i] = team number for student i
long long result = 0;

void backtrack(int student) {
    if (student == 3 * N) {
        result++;
        return;
    }
    
    // Try assigning current student to an existing team or a new team
    set<int> usedTeams;
    for (int i = 0; i < student; i++) {
        usedTeams.insert(assignment[i]);
    }
    
    // Try each existing team
    for (int team : usedTeams) {
        // Count students in this team
        int count = 0;
        vector<int> members;
        for (int i = 0; i < student; i++) {
            if (assignment[i] == team) {
                count++;
                members.push_back(i + 1); // 1-indexed
            }
        }
        
        if (count < 3) {
            members.push_back(student + 1); // 1-indexed
            
            // Check if this would form a forbidden triple
            bool isForbidden = false;
            if (members.size() == 3) {
                set<int> triple(members.begin(), members.end());
                if (forbidden.count(triple)) {
                    isForbidden = true;
                }
            }
            
            if (!isForbidden) {
                assignment[student] = team;
                backtrack(student + 1);
            }
        }
    }
    
    // Try a new team (only if we haven't created too many teams)
    int numTeams = usedTeams.size();
    if (numTeams < N) {
        assignment[student] = numTeams;
        backtrack(student + 1);
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N >> K;
    
    for (int i = 0; i < K; i++) {
        int a, b, c;
        cin >> a >> b >> c;
        set<int> triple = {a, b, c};
        forbidden.insert(triple);
    }
    
    assignment.assign(3 * N, -1);
    backtrack(0);
    
    cout << result << endl;
    
    return 0;
}
