#include<bits/stdc++.h>
using namespace std;

int main(){
    int a, b, n;
    cin >> a >> b >> n;
    
    vector<int> counts(n);
    for(int i = 0; i < n; i++){
        cin >> counts[i];
    }
    
    int boys_with_teacher = 0;
    int girls_with_teacher = b;
    
    vector<pair<int,int>> result;
    
    for(int i = 0; i < n; i++){
        int target = counts[i];
        int current_total = boys_with_teacher + girls_with_teacher;
        
        // We need to reach target
        int needed = target - current_total;
        
        // Try to find valid join/leave combination
        bool found = false;
        for(int boys_join = 0; boys_join <= a - boys_with_teacher && boys_join <= needed + girls_with_teacher; boys_join++){
            int girls_leave = boys_join - needed;
            
            if(girls_leave >= 0 && girls_leave <= girls_with_teacher){
                // Valid move
                boys_with_teacher += boys_join;
                girls_with_teacher -= girls_leave;
                result.push_back({boys_join, girls_leave});
                found = true;
                break;
            }
        }
        
        if(!found){
            cout << "ERROR\n";
            return 0;
        }
    }
    
    for(auto& p : result){
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}
