#include<bits/stdc++.h>
using std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    map<long long, long long> cells; // cell number -> cube count
    
    auto getTowers = [&]() {
        vector<pair<long long, long long>> towers; // start, end of each tower
        if(cells.empty()) return towers;
        
        auto it = cells.begin();
        long long start = it->first;
        long long end = it->first;
        
        for(auto it2 = next(it); it2 != cells.end(); ++it2) {
            if(it2->first == end + 1) {
                end = it2->first;
            } else {
                towers.push_back({start, end});
                start = it2->first;
                end = it2->first;
            }
        }
        towers.push_back({start, end});
        
        return towers;
    };
    
    for(int i = 0; i < n; i++) {
        string cmd;
        cin >> cmd;
        
        if(cmd == "put") {
            long long x, c;
            cin >> x >> c;
            cells[x] += c;
        }
        else if(cmd == "tput") {
            long long t, x, c;
            cin >> t >> x >> c;
            auto towers = getTowers();
            long long cellNum = towers[t-1].first + x - 1;
            cells[cellNum] += c;
        }
        else if(cmd == "towers") {
            auto towers = getTowers();
            cout << towers.size() << " towers\n";
        }
        else if(cmd == "cubes") {
            long long t;
            cin >> t;
            auto towers = getTowers();
            long long sum = 0;
            for(long long cell = towers[t-1].first; cell <= towers[t-1].second; cell++) {
                sum += cells[cell];
            }
            cout << sum << " cubes in " << t << "th tower\n";
        }
        else if(cmd == "length") {
            long long t;
            cin >> t;
            auto towers = getTowers();
            long long len = towers[t-1].second - towers[t-1].first + 1;
            cout << "length of " << t << "th tower is " << len << "\n";
        }
        else if(cmd == "tcubes") {
            long long t, x;
            cin >> t >> x;
            auto towers = getTowers();
            long long cellNum = towers[t-1].first + x - 1;
            cout << cells[cellNum] << " cubes in " << x << "th column of " << t << "th tower\n";
        }
    }
    
    return 0;
}
