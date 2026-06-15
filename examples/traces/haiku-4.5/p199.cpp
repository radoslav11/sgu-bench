#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<tuple<long long, long long, int>> people(n);
    for(int i = 0; i < n; i++){
        long long s, b;
        cin >> s >> b;
        people[i] = {s, b, i+1};
    }
    
    sort(people.begin(), people.end());
    
    // Find longest increasing subsequence of beauties
    vector<int> lis_indices;
    vector<long long> lis_vals;
    for(int i = 0; i < n; i++){
        long long b = get<1>(people[i]);
        auto it = lower_bound(lis_vals.begin(), lis_vals.end(), b);
        if(it == lis_vals.end()){
            lis_vals.push_back(b);
            lis_indices.push_back(i);
        } else {
            *it = b;
            lis_indices[it - lis_vals.begin()] = i;
        }
    }
    
    // Find longest decreasing subsequence of beauties
    vector<int> lds_indices;
    vector<long long> lds_vals;
    for(int i = 0; i < n; i++){
        long long b = get<1>(people[i]);
        auto it = lower_bound(lds_vals.begin(), lds_vals.end(), -b);
        if(it == lds_vals.end()){
            lds_vals.push_back(-b);
            lds_indices.push_back(i);
        } else {
            *it = -b;
            lds_indices[it - lds_vals.begin()] = i;
        }
    }
    
    vector<int> result;
    if(lis_vals.size() >= lds_vals.size()){
        for(int idx : lis_indices) result.push_back(get<2>(people[idx]));
    } else {
        for(int idx : lds_indices) result.push_back(get<2>(people[idx]));
    }
    
    cout << result.size() << "\n";
    for(int x : result) cout << x << " ";
    cout << "\n";
    
    return 0;
}
