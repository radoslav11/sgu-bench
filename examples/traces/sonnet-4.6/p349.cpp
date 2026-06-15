#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, M;
    cin >> N >> M;
    
    auto getInterval = [](double x1,double y1,double x2,double y2) -> pair<double,double> {
        double a1 = atan2(y1,x1), a2 = atan2(y2,x2);
        if(a1 > a2) swap(a1,a2);
        return {a1, a2};
    };
    
    vector<pair<double,double>> wolves(N), sheep(M);
    for(int i=0;i<N;i++){
        double x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2;
        wolves[i] = getInterval(x1,y1,x2,y2);
    }
    for(int i=0;i<M;i++){
        double x1,y1,x2,y2; cin>>x1>>y1>>x2>>y2;
        sheep[i] = getInterval(x1,y1,x2,y2);
    }
    
    // Merge sheep intervals
    sort(sheep.begin(),sheep.end());
    vector<pair<double,double>> merged;
    for(auto& s: sheep){
        if(!merged.empty() && s.first <= merged.back().second)
            merged.back().second = max(merged.back().second, s.second);
        else merged.push_back(s);
    }
    
    // Greedy: sort wolves by right endpoint
    sort(wolves.begin(),wolves.end(),[](auto&a,auto&b){return a.second<b.second;});
    
    int shots = 0;
    double last_shot = -1e18;
    for(auto& w: wolves){
        if(last_shot >= w.first) continue; // already stabbed
        // Find rightmost valid point in [w.first, w.second]
        double pos = w.second;
        // Check sheep intervals overlapping [w.first, w.second]
        bool found = false;
        double cur = w.second;
        while(cur >= w.first){
            // find sheep interval containing cur
            auto it = upper_bound(merged.begin(),merged.end(),make_pair(cur,1e18));
            if(it != merged.begin()){
                --it;
                if(it->first <= cur && cur <= it->second){
                    cur = it->first - 1e-9;
                    continue;
                }
            }
            found = true; pos = cur; break;
        }
        if(!found) { cout << "No solution\n"; return 0; }
        last_shot = pos; shots++;
    }
    cout << shots << "\n";
}
