#include <bits/stdc++.h>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<pair<double, double>> holes(n);
    for (int i = 0; i < n; i++) {
        cin >> holes[i].first >> holes[i].second;
    }
    
    long long count = 0;
    
    // For each hole as Shureg's position
    for (int i = 0; i < n; i++) {
        // For each pair of other holes as Ruslans' positions
        for (int j = 0; j < n; j++) {
            if (j == i) continue;
            for (int k = j + 1; k < n; k++) {
                if (k == i) continue;
                
                // Calculate vectors from Shureg to the two moles
                double v1x = holes[j].first - holes[i].first;
                double v1y = holes[j].second - holes[i].second;
                double v2x = holes[k].first - holes[i].first;
                double v2y = holes[k].second - holes[i].second;
                
                // Calculate dot product and magnitudes
                double dotProduct = v1x * v2x + v1y * v2y;
                double mag1 = sqrt(v1x * v1x + v1y * v1y);
                double mag2 = sqrt(v2x * v2x + v2y * v2y);
                
                if (mag1 == 0 || mag2 == 0) continue;
                
                double cosA = dotProduct / (mag1 * mag2);
                
                // Check if angle is acute (cosA > 0)
                if (cosA <= 0) continue;
                
                // Calculate angle in degrees
                double angleRad = acos(cosA);
                double angleDeg = angleRad * 180.0 / M_PI;
                
                // Check if angle is less than 90 degrees
                if (angleDeg >= 90.0) continue;
                
                // Calculate floor(90 / A)
                int left = (int)floor(90.0 / angleDeg);
                
                // Calculate third digit after decimal point of cos A
                int right = (int)floor(1000.0 * cosA) % 10;
                
                // Check condition
                if (left == right) {
                    count++;
                }
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
