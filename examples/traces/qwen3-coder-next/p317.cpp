#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <iomanip>
#include <climits>
#include <cstring>
#include <map>
#include <set>

using namespace std;

struct Horse {
    long long v, d;
    Horse(long long v, long long d) : v(v), d(d) {}
};

struct State {
    double time;
    int pos; // position index in stables (0..n-1)
    int stable_id; // index of stable
    State(double t, int p, int sid) : time(t), pos(p), stable_id(sid) {}
    
    bool operator>(const State& other) const {
        return time > other.time;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long B;
    int N;
    cin >> B >> N;
    
    vector<pair<long long, vector<Horse>>> stables;
    
    for (int i = 0; i < N; i++) {
        long long X, M;
        cin >> X >> M;
        vector<Horse> horses;
        for (int j = 0; j < M; j++) {
            long long v, d;
            cin >> v >> d;
            horses.push_back(Horse(v, d));
        }
        stables.emplace_back(X, horses);
    }
    
    // Sort stables by position
    sort(stables.begin(), stables.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
    
    // Remove duplicates: combine stables at same position
    vector<pair<long long, vector<Horse>>> merged;
    for (int i = 0; i < N; ) {
        long long pos = stables[i].first;
        vector<Horse> allHorses;
        
        int j = i;
        while (j < N && stables[j].first == pos) {
            for (const auto& h : stables[j].second) {
                allHorses.push_back(h);
            }
            j++;
        }
        
        merged.emplace_back(pos, allHorses);
        i = j;
    }
    
    stables = merged;
    N = stables.size();
    
    // dp[i] = minimum time to reach position stables[i].first
    // We'll use Dijkstra-like approach since time is non-negative
    // But note: from a stable, we can go to any stable ahead within distance d/v
    
    // Let dist[i] = minimum time to reach stable i (position = stables[i].first)
    // We start at position 0, which may or may not have a stable
    
    // Add position 0 if not present
    if (N == 0 || stables[0].first > 0) {
        stables.insert(stables.begin(), {0, {}});
        N = stables.size();
    }
    
    // If there's a stable at 0, we have horses there; otherwise, we need one at 0
    // But problem says: "There are N stables on the straight line from A to B", A is 0, B is positive
    // It's possible no stable at 0, but then we can't start!
    // However, the example has stable at 0.
    
    // Check if we have a stable at 0
    bool hasStableAt0 = false;
    for (int i = 0; i < N; i++) {
        if (stables[i].first == 0) {
            hasStableAt0 = true;
            break;
        }
    }
    
    if (!hasStableAt0) {
        cout << -1 << endl;
        return 0;
    }
    
    vector<double> dist(N, 1e18);
    
    // Priority queue: (time, stable_id)
    priority_queue<State, vector<State>, greater<State>> pq;
    
    // Find all horses at position 0
    for (int i = 0; i < N; i++) {
        if (stables[i].first == 0) {
            for (const auto& horse : stables[i].second) {
                // From stable i, we can go to any position up to 0 + horse.d
                // But we can only change horses at stables, so we can reach stables j where:
                // 0 <= stables[j].first <= horse.d
                double timeToJ = (double)stables[j].first / horse.v;
                // But we don't know j yet, so we'll push the initial state
            }
            dist[i] = 0.0;
            pq.push(State(0.0, (int)stables[i].first, i));
        }
    }
    
    // Actually, better: from a stable at position p with a horse (v, d), we can reach any stable at position q such that p <= q <= p + d
    // The time to reach q is current_time + (q - p) / v
    
    // We'll do Dijkstra where nodes are stable indices
    // But note: we might not use all horses from a stable; we want the best time to reach each stable
    
    // Let's use Dijkstra on stable indices
    // Initialize: all stables at position 0 with dist=0, push them
    
    fill(dist.begin(), dist.end(), 1e18);
    
    for (int i = 0; i < N; i++) {
        if (stables[i].first == 0) {
            dist[i] = 0.0;
            pq.push(State(0.0, (int)i, i));
        }
    }
    
    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();
        
        int curId = cur.stable_id;
        double curTime = cur.time;
        
        if (curTime > dist[curId]) continue;
        
        // From stable curId, try all horses
        for (const auto& horse : stables[curId].second) {
            long long maxDist = horse.d;
            long long maxPos = stables[curId].first + maxDist;
            
            // Find all stables j such that stables[j].first > stables[curId].first and stables[j].first <= maxPos
            // Since stables are sorted by position, we can binary search
            int lo = curId + 1;
            int hi = N - 1;
            int jStart = -1;
            
            // First stable with position > curPos
            jStart = curId + 1;
            if (jStart >= N) continue;
            
            for (int j = jStart; j < N && stables[j].first <= maxPos; j++) {
                long long distToJ = stables[j].first - stables[curId].first;
                double timeToJ = curTime + (double)distToJ / horse.v;
                
                if (timeToJ < dist[j]) {
                    dist[j] = timeToJ;
                    pq.push(State(timeToJ, j, j));
                }
            }
        }
    }
    
    // Now, we need to get to B. We can either:
    // 1. Reach a stable and then ride to B with that stable's horse (if we have a horse at that stable that can cover the remaining distance)
    // 2. Or reach B directly from a stable without stopping at another stable
    
    // So we consider: from stable i, with a horse (v, d), if stables[i].first + d >= B, then we can go from stable i to B
    // time = dist[i] + (B - stables[i].first) / v
    
    double ans = 1e18;
    
    for (int i = 0; i < N; i++) {
        if (dist[i] >= 1e17) continue;
        
        for (const auto& horse : stables[i].second) {
            long long distRemaining = B - stables[i].first;
            if (distRemaining <= horse.d) {
                double timeToB = dist[i] + (double)distRemaining / horse.v;
                ans = min(ans, timeToB);
            }
        }
    }
    
    // Also, what if we can go from stable i to a position between stables and then to B? 
    // But since we can only change horses at stables, the last horse must be taken from some stable and used to go to B.
    // So the above covers it.
    
    if (ans >= 1e17) {
        cout << -1 << endl;
    } else {
        cout << fixed << setprecision(10) << ans << endl;
    }
    
    return 0;
}
