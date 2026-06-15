#include <iostream>
#include <vector>
using namespace std;

int main() {
    int N;
    cin >> N;
    
    vector<int> color0, color1;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            int pos = (i - 1) * N + j;
            if ((i + j) % 2 == 0) {
                color0.push_back(pos);
            } else {
                color1.push_back(pos);
            }
        }
    }
    
    // Round 1: audience moves odd times, so must be at opposite parity from start
    // Start position (1,1) has (1+1)%2=0, so same parity as color0
    // After odd moves: audience at color1 only
    // So remove all color0 positions (audience can't be there)
    
    int k1 = N;
    if (N % 2 == 0) {
        // Make it odd
        k1++;
    }
    
    cout << k1;
    if (N % 2 == 1) {
        // For odd N, center is color0 but we need to keep it for round 2
        int center_val = ((N+1)/2 - 1) * N + (N+1)/2;
        for (int c : color0) {
            if (c != center_val) {
                cout << " " << c;
            }
        }
    } else {
        // For even N, just remove all color0
        for (int c : color0) {
            cout << " " << c;
        }
    }
    cout << "\n";
    
    // Round 2: audience moves odd times from color1, so must be at color0
    // Remove all positions that are still present after round 1, except none should remain
    int k2 = k1 + 2;
    cout << k2;
    
    if (N % 2 == 1) {
        // After round 1, only color0 without center remains: color0 minus center
        // But audience can't be there after odd moves, so remove all of them
        int center_val = ((N+1)/2 - 1) * N + (N+1)/2;
        for (int c : color0) {
            if (c != center_val) {
                cout << " " << c;
            }
        }
        // Also need to remove color1 positions? No, audience must be at color0 after round 2
        // But we want only one position to remain at the end
        
        // Actually, after round 2, audience moves from color1 (remaining positions after round 1)
        // to color0. So remove all color0 except the center
        for (int c : color1) {
            cout << " " << c;
        }
    } else {
        // For even N, after round 1 removed color0, so audience is at color1
        // After odd moves in round 2, must be at color0, so remove all color1
        for (int c : color1) {
            cout << " " << c;
        }
    }
    cout << "\n";
    
    return 0;
}
