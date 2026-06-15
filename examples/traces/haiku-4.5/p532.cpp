#include<bits/stdc++.h>
using namespace std;

struct Segment {
    int a, b, c; // for horizontal: y, x1, x2 (x1 < x2)
                 // for vertical: x, y1, y2 (y1 < y2)
};

bool isCovered(int start, int end, vector<Segment>& segments) {
    // Check if [start, end] is completely covered by segments
    vector<pair<int,int>> intervals;
    for(auto& seg : segments) {
        int a = seg.a, b = seg.b, c = seg.c;
        // seg covers [b, c]
        if(c < b) swap(b, c);
        intervals.push_back({b, c});
    }
    
    sort(intervals.begin(), intervals.end());
    
    int covered = start;
    for(auto& interval : intervals) {
        if(interval.first > covered) return false;
        covered = max(covered, interval.second);
    }
    
    return covered >= end;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<Segment> horizontal, vertical;
    set<int> xs, ys;
    
    for(int i = 0; i < n; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        
        if(y1 == y2) {
            // horizontal segment
            if(x1 > x2) swap(x1, x2);
            horizontal.push_back({y1, x1, x2});
            xs.insert(x1);
            xs.insert(x2);
            ys.insert(y1);
        } else {
            // vertical segment
            if(y1 > y2) swap(y1, y2);
            vertical.push_back({x1, y1, y2});
            xs.insert(x1);
            ys.insert(y1);
            ys.insert(y2);
        }
    }
    
    vector<int> xcoords(xs.begin(), xs.end());
    vector<int> ycoords(ys.begin(), ys.end());
    
    long long count = 0;
    
    // Try all combinations of rectangles
    for(int i = 0; i < xcoords.size(); i++) {
        for(int j = i+1; j < xcoords.size(); j++) {
            for(int k = 0; k < ycoords.size(); k++) {
                for(int l = k+1; l < ycoords.size(); l++) {
                    int x1 = xcoords[i], x2 = xcoords[j];
                    int y1 = ycoords[k], y2 = ycoords[l];
                    
                    // Check bottom side (y1)
                    vector<Segment> bottomSegs;
                    for(auto& seg : horizontal) {
                        if(seg.a == y1) bottomSegs.push_back(seg);
                    }
                    if(!isCovered(x1, x2, bottomSegs)) continue;
                    
                    // Check top side (y2)
                    vector<Segment> topSegs;
                    for(auto& seg : horizontal) {
                        if(seg.a == y2) topSegs.push_back(seg);
                    }
                    if(!isCovered(x1, x2, topSegs)) continue;
                    
                    // Check left side (x1)
                    vector<Segment> leftSegs;
                    for(auto& seg : vertical) {
                        if(seg.a == x1) leftSegs.push_back(seg);
                    }
                    if(!isCovered(y1, y2, leftSegs)) continue;
                    
                    // Check right side (x2)
                    vector<Segment> rightSegs;
                    for(auto& seg : vertical) {
                        if(seg.a == x2) rightSegs.push_back(seg);
                    }
                    if(!isCovered(y1, y2, rightSegs)) continue;
                    
                    count++;
                }
            }
        }
    }
    
    cout << count << endl;
    
    return 0;
}
