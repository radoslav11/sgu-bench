#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<string> contest1(n), contest2(n);
    
    for(int i = 0; i < n; i++){
        cin >> contest1[i];
    }
    for(int i = 0; i < n; i++){
        cin >> contest2[i];
    }
    
    // Find maximum M where top M teams are the same set
    int maxM = 0;
    for(int m = 1; m <= n; m++){
        set<string> s1(contest1.begin(), contest1.begin() + m);
        set<string> s2(contest2.begin(), contest2.begin() + m);
        if(s1 == s2){
            maxM = m;
        }
    }
    
    // Output first maxM from contest1
    vector<string> result;
    set<string> used;
    
    for(int i = 0; i < maxM; i++){
        result.push_back(contest1[i]);
        used.insert(contest1[i]);
    }
    
    // Collect remaining teams and sort them
    vector<string> remaining;
    for(int i = 0; i < n; i++){
        if(used.find(contest1[i]) == used.end()){
            remaining.push_back(contest1[i]);
        }
    }
    sort(remaining.begin(), remaining.end());
    
    for(auto& team : remaining){
        result.push_back(team);
    }
    
    for(auto& team : result){
        cout << team << "\n";
    }
    
    return 0;
}
