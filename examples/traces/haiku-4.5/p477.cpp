#include<bits/stdc++.h>
using namespace std;

int countOpenings(vector<long long>& times, long long t) {
    if(times.empty()) return 0;
    int count = 1;
    for(int i = 1; i < times.size(); i++) {
        if(times[i] - times[i-1] > 2*t) {
            count++;
        }
    }
    return count;
}

vector<pair<long long, long long>> getSegments(vector<long long>& times, long long t) {
    vector<pair<long long, long long>> segs;
    if(times.empty()) return segs;
    
    long long start = times[0] - t;
    for(int i = 1; i < times.size(); i++) {
        if(times[i] - times[i-1] > 2*t) {
            segs.push_back({start, times[i-1] + t});
            start = times[i] - t;
        }
    }
    segs.push_back({start, times.back() + t});
    return segs;
}

bool checkConstraint(vector<pair<long long, long long>>& seg1, vector<pair<long long, long long>>& seg2, long long d) {
    for(auto& s1 : seg1) {
        for(auto& s2 : seg2) {
            long long overlapStart = max(s1.first, s2.first);
            long long overlapEnd = min(s1.second, s2.second);
            if(overlapStart <= overlapEnd && overlapEnd - overlapStart > d) {
                return false;
            }
        }
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n, m;
    long long d;
    cin >> n >> m >> d;
    
    vector<long long> p(n), q(m);
    for(int i = 0; i < n; i++) cin >> p[i];
    for(int i = 0; i < m; i++) cin >> q[i];
    
    set<long long> candidates;
    candidates.insert(1);
    candidates.insert(d);
    
    for(int i = 1; i < n; i++) {
        long long gap = p[i] - p[i-1];
        candidates.insert(gap / 2);
        if(gap / 2 > 0) candidates.insert(gap / 2 - 1);
        candidates.insert(gap / 2 + 1);
    }
    
    for(int i = 1; i < m; i++) {
        long long gap = q[i] - q[i-1];
        candidates.insert(gap / 2);
        if(gap / 2 > 0) candidates.insert(gap / 2 - 1);
        candidates.insert(gap / 2 + 1);
    }
    
    int minOpenings = INT_MAX;
    long long bestT1 = -1, bestT2 = -1;
    
    for(long long t1 : candidates) {
        if(t1 < 1 || t1 > 1e9) continue;
        for(long long t2 : candidates) {
            if(t2 < 1 || t2 > 1e9) continue;
            
            auto seg1 = getSegments(p, t1);
            auto seg2 = getSegments(q, t2);
            
            if(checkConstraint(seg1, seg2, d)) {
                int openings = countOpenings(p, t1) + countOpenings(q, t2);
                if(openings < minOpenings) {
                    minOpenings = openings;
                    bestT1 = t1;
                    bestT2 = t2;
                }
            }
        }
    }
    
    if(bestT1 == -1) {
        cout << "No solution\n";
    } else {
        cout << bestT1 << " " << bestT2 << "\n";
    }
    
    return 0;
}
