#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int K;
    cin >> K;
    
    vector<int> N(K);
    int total_students = 0;
    for(int i = 0; i < K; i++){
        cin >> N[i];
        total_students += N[i];
    }
    
    vector<int> Q(K);
    for(int i = 0; i < K; i++){
        cin >> Q[i];
    }
    
    vector<int> P(total_students);
    for(int i = 0; i < total_students; i++){
        cin >> P[i];
    }
    
    vector<int> w(total_students);
    for(int i = 0; i < total_students; i++){
        cin >> w[i];
    }
    
    vector<int> assignment(total_students, -1);
    vector<int> zone_count(K, 0);
    
    // Create list of (weight, student_idx, zone_idx) for beneficial assignments
    vector<tuple<int, int, int>> beneficial;
    for(int i = 0; i < total_students; i++){
        for(int j = 0; j < K; j++){
            if(P[i] > Q[j]){
                beneficial.push_back(make_tuple(w[i], i, j));
            }
        }
    }
    
    // Sort by weight in descending order
    sort(beneficial.rbegin(), beneficial.rend());
    
    // Greedily assign beneficial assignments
    for(auto [weight, student, zone] : beneficial){
        if(zone_count[zone] < N[zone] && assignment[student] == -1){
            assignment[student] = zone;
            zone_count[zone]++;
        }
    }
    
    // Fill remaining slots with any valid zone
    for(int i = 0; i < total_students; i++){
        if(assignment[i] == -1){
            for(int j = 0; j < K; j++){
                if(zone_count[j] < N[j]){
                    assignment[i] = j;
                    zone_count[j]++;
                    break;
                }
            }
        }
    }
    
    for(int i = 0; i < total_students; i++){
        cout << assignment[i] + 1;
        if(i < total_students - 1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
