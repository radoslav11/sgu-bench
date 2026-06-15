#include<bits/stdc++.h>
using namespace std;

struct Building {
    double L, R, H;
};

bool canIlluminate(double sx, double sy, double px, double py, vector<Building>& buildings) {
    // Check if light at (sx, sy) can illuminate point (px, py)
    // Line segment from (sx, sy) to (px, py) shouldn't intersect building interiors
    
    double minX = min(sx, px), maxX = max(sx, px);
    double minY = min(sy, py), maxY = max(sy, py);
    
    for(auto& b : buildings) {
        // Check if building interior intersects the line segment
        // A building interior is (L, R) x (0, H) (excluding boundary)
        
        if(b.L >= maxX || b.R <= minX) continue; // No horizontal overlap
        
        // Check if line segment passes through building interior
        // Parametric form: (sx + t(px-sx), sy + t(py-sy)), t in [0,1]
        
        double x1 = sx, y1 = sy, x2 = px, y2 = py;
        double dx = x2 - x1, dy = y2 - y1;
        
        // Check intersection with (L, R) x (0, H)
        if(dx == 0) {
            // Vertical line
            if(x1 > b.L && x1 < b.R && ((y1 > 0 && y1 < b.H) || (y2 > 0 && y2 < b.H))) {
                double ymin = min(y1, y2), ymax = max(y1, y2);
                if(ymax > 0 && ymin < b.H) return false;
            }
        } else {
            // Check parametric intersection
            for(double t = 0; t <= 1; t += 0.01) {
                double x = x1 + t * dx, y = y1 + t * dy;
                if(x > b.L && x < b.R && y > 0 && y < b.H) return false;
            }
        }
    }
    return true;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int T;
    cin >> T;
    
    while(T--) {
        int N;
        cin >> N;
        
        vector<Building> buildings(N);
        for(int i = 0; i < N; i++) {
            cin >> buildings[i].L >> buildings[i].R >> buildings[i].H;
        }
        
        int lights = 0;
        
        // For each side edge that needs illumination
        for(int i = 0; i < N; i++) {
            // Left side at x = L, y in [0, H]
            // Right side at x = R, y in [0, H]
            
            // Check left side
            bool leftCovered = false;
            // Try placing light at right end of rightmost building that can see it
            for(int j = 0; j < N && !leftCovered; j++) {
                if(canIlluminate(buildings[j].R, buildings[j].H, buildings[i].L, buildings[i].H, buildings)) {
                    lights++;
                    leftCovered = true;
                }
            }
            
            // Check right side  
            bool rightCovered = false;
            for(int j = 0; j < N && !rightCovered; j++) {
                if(canIlluminate(buildings[j].L, buildings[j].H, buildings[i].R, buildings[i].H, buildings)) {
                    lights++;
                    rightCovered = true;
                }
            }
        }
        
        cout << lights << "\n";
    }
    
    return 0;
}
