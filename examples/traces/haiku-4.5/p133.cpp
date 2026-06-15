#include<iostream>
#include<vector>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    vector<pair<long long, long long>> outposts(n);
    
    for(int i = 0; i < n; i++){
        cin >> outposts[i].first >> outposts[i].second;
    }
    
    int redundant_count = 0;
    
    // For each outpost i
    for(int i = 0; i < n; i++){
        long long a_i = outposts[i].first;
        long long b_i = outposts[i].second;
        
        // Check if there exists outpost j that dominates i
        bool is_redundant = false;
        for(int j = 0; j < n; j++){
            if(i == j) continue;
            
            long long a_j = outposts[j].first;
            long long b_j = outposts[j].second;
            
            // Check if j dominates i
            if(a_j < a_i && b_j > b_i){
                is_redundant = true;
                break;
            }
        }
        
        if(is_redundant){
            redundant_count++;
        }
    }
    
    cout << redundant_count << endl;
    
    return 0;
}
