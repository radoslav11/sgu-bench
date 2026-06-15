#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    if (N == 0) {
        cout << fixed << setprecision(3) << 0.0 << endl;
        return 0;
    }
    
    // Optimal: star configuration
    // All N points connected to center A0, placed on unit circle
    double totalScore = 0;
    
    for (int i = 1; i <= N; i++) {
        // After adding i-th point
        int M = i + 1;  // i+1 segments meeting at A0
        double D;
        
        if (i == 1) {
            D = 1.0;  // Distance from A0 to first point
        } else {
            // Maximum distance is between two points on unit circle
            // For i points on circle: max distance is 2*sin(pi*i/(2*i)) = 2*sin(pi/2) = 2 for i>=2
            D = 2.0;
        }
        
        totalScore += M * D;
    }
    
    cout << fixed << setprecision(3) << totalScore << endl;
    
    return 0;
}
