#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long x1, y1, x2, y2, n;
    cin >> x1 >> y1 >> x2 >> y2 >> n;
    
    long long dx = x2 - x1;
    long long dy = y2 - y1;
    
    if (dx == 0 && dy == 0) {
        cout << "no solution\n";
        return 0;
    }
    
    set<pair<long long, long long>> cells;
    
    // Walk along the line using Bresenham-like approach
    long long steps = abs(dx) + abs(dy);
    
    for (long long i = 0; i < steps; i++) {
        double t = (double)(i) / steps;
        double x = x1 + t * dx;
        double y = y1 + t * dy;
        
        long long cell_x = (long long)floor(x);
        long long cell_y = (long long)floor(y);
        
        cells.insert({cell_x, cell_y});
        
        if (cells.size() == n) {
            cout << cell_x << " " << cell_y << "\n";
            return 0;
        }
    }
    
    cout << "no solution\n";
    return 0;
}
