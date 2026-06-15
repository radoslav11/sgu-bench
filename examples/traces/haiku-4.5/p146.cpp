#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double L;
    int N;
    cin >> L >> N;
    
    double total_distance = 0.0;
    
    for (int i = 0; i < N; i++) {
        long long T;
        long long V;
        cin >> T >> V;
        total_distance += (double)T * (double)V;
    }
    
    // Find position on the ring road
    double position = fmod(total_distance, L);
    
    // The shortest distance is the minimum of going forward or backward
    double shortest = min(position, L - position);
    
    cout << fixed << setprecision(4) << shortest << endl;
    
    return 0;
}
