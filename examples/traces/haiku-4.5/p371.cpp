#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    int K = M - N + 1;  // number of circle lines
    
    if(K < 1 || K > N) {
        cout << "No solution\n";
        return 0;
    }
    
    // We need to distribute N - R stations among K circles (each 3-10)
    // We have M - (N-R) - (K-1) = R radial edges
    // So R = M - N + 1 - K = 0? No, let me recalculate
    
    // edges = circle_edges + connecting_edges + radial_edges
    // M = sum(circle_sizes) + (K-1) + R
    // N = sum(circle_sizes) + R
    // From second: sum(circle_sizes) = N - R
    // Substitute in first: M = N - R + K - 1 + R = N + K - 1
    // So K = M - N + 1
    
    // Now we need R such that:
    // - sum(circle_sizes) = N - R
    // - We can distribute N - R among K circles with each having 3-10 stations
    
    int R = N - 3*K;  // minimum case: K circles of size 3
    if(R < 0) {
        cout << "No solution\n";
        return 0;
    }
    
    if(N - R > 10*K) {  // maximum case: K circles of size 10
        cout << "No solution\n";
        return 0;
    }
    
    // Verify equation
    if(N + K - 1 != M) {
        cout << "No solution\n";
        return 0;
    }
    
    // Build circles: start with all size 3, then add to reach N-R
    vector<int> circle_sizes(K, 3);
    int remaining = N - R - 3*K;
    for(int i = 0; i < K && remaining > 0; i++) {
        int add = min(remaining, 7);  // max size is 10, so add at most 7
        circle_sizes[i] += add;
        remaining -= add;
    }
    
    if(remaining != 0) {
        cout << "No solution\n";
        return 0;
    }
    
    cout << K << "\n";
    int station = 1;
    vector<int> junction_stations;
    
    for(int i = 0; i < K; i++) {
        cout << circle_sizes[i];
        int start = station;
        for(int j = 0; j < circle_sizes[i]; j++) {
            cout << " " << station;
            if(j == 0) junction_stations.push_back(station);
            station++;
        }
        cout << "\n";
    }
    
    cout << R << "\n";
    for(int i = 0; i < R; i++) {
        cout << junction_stations[i % K] << " " << station << "\n";
        station++;
    }
    
    return 0;
}
