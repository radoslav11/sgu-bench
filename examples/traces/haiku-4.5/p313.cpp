#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, L;
    cin >> n >> L;
    
    vector<pair<int,int>> houses(n), offices(n);
    
    for(int i = 0; i < n; i++){
        cin >> houses[i].first;
        houses[i].second = i;
    }
    
    for(int i = 0; i < n; i++){
        cin >> offices[i].first;
        offices[i].second = i;
    }
    
    auto dist = [&](int a, int b) -> long long {
        long long d1 = (b - a + L) % L;
        long long d2 = (a - b + L) % L;
        return min(d1, d2);
    };
    
    // Sort houses and offices by location
    sort(houses.begin(), houses.end());
    sort(offices.begin(), offices.end());
    
    long long minCost = LLONG_MAX;
    int bestRotation = 0;
    
    // Try all rotations
    for(int rotation = 0; rotation < n; rotation++){
        long long cost = 0;
        for(int i = 0; i < n; i++){
            cost += dist(houses[i].first, offices[(i + rotation) % n].first);
        }
        if(cost < minCost){
            minCost = cost;
            bestRotation = rotation;
        }
    }
    
    // Create result array
    vector<int> result(n);
    
    for(int i = 0; i < n; i++){
        int houseOriginalIdx = houses[i].second;
        int officeIdx = (i + bestRotation) % n;
        int officeOriginalIdx = offices[officeIdx].second;
        result[houseOriginalIdx] = officeOriginalIdx + 1; // 1-indexed
    }
    
    cout << minCost << "\n";
    for(int i = 0; i < n; i++){
        cout << result[i];
        if(i < n-1) cout << " ";
    }
    cout << "\n";
    
    return 0;
}
