#include<bits/stdc++.h>
using namespace std;

struct Segment {
    double x1, y1, x2, y2;
    char color;
};

double getY(const Segment& seg, double x) {
    if (seg.x1 == seg.x2) {
        // Vertical segment - return the minimum y
        return min(seg.y1, seg.y2);
    }
    // Linear interpolation
    double t = (x - seg.x1) / (seg.x2 - seg.x1);
    if (t < 0 || t > 1) return 1e9; // Out of range
    return seg.y1 + t * (seg.y2 - seg.y1);
}

bool containsX(const Segment& seg, double x) {
    return x >= min(seg.x1, seg.x2) && x <= max(seg.x1, seg.x2);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Segment> segments(n);
    set<double> xCoords;
    
    for(int i = 0; i < n; i++) {
        cin >> segments[i].x1 >> segments[i].y1 >> segments[i].x2 >> segments[i].y2 >> segments[i].color;
        xCoords.insert(segments[i].x1);
        xCoords.insert(segments[i].x2);
    }
    
    vector<double> xs(xCoords.begin(), xCoords.end());
    
    double sr = 0, sg = 0, sb = 0;
    
    // Process each interval
    for(int i = 0; i < (int)xs.size() - 1; i++) {
        double x1 = xs[i];
        double x2 = xs[i+1];
        double midX = (x1 + x2) / 2.0;
        double intervalLen = x2 - x1;
        
        // Find segment with minimum Y at midX
        double minY = 1e9;
        char closestColor = 'R';
        
        for(int j = 0; j < n; j++) {
            if(containsX(segments[j], midX)) {
                double y = getY(segments[j], midX);
                if(y < minY) {
                    minY = y;
                    closestColor = segments[j].color;
                }
            }
        }
        
        if(closestColor == 'R') sr += intervalLen;
        else if(closestColor == 'G') sg += intervalLen;
        else if(closestColor == 'B') sb += intervalLen;
    }
    
    cout << fixed << setprecision(2);
    cout << "R " << sr << endl;
    cout << "G " << sg << endl;
    cout << "B " << sb << endl;
    
    return 0;
}
