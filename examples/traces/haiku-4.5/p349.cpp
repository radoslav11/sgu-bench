#include<bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

struct Point {
    double x, y;
};

struct Segment {
    Point p1, p2;
};

double ccw(Point a, Point b, Point c) {
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

bool rayIntersectsSegment(Point origin, double angle, Segment seg) {
    Point dir = {cos(angle), sin(angle)};
    Point a = seg.p1, b = seg.p2;
    
    // Check if segment intersects the ray from origin in direction angle
    double c1 = ccw(origin, Point{origin.x + dir.x, origin.y + dir.y}, a);
    double c2 = ccw(origin, Point{origin.x + dir.x, origin.y + dir.y}, b);
    
    if ((c1 > EPS && c2 > EPS) || (c1 < -EPS && c2 < -EPS)) return false;
    
    // Check if intersection point is on correct side of origin
    double t_num = a.x * dir.y - a.y * dir.x;
    double t_denom = (b.x - a.x) * dir.y - (b.y - a.y) * dir.x;
    
    if (abs(t_denom) < EPS) {
        // Segment parallel to ray
        if (abs(t_num) < EPS) {
            // Collinear - check if segment has positive component
            return (a.x * dir.x + a.y * dir.y > EPS) || (b.x * dir.x + b.y * dir.y > EPS);
        }
        return false;
    }
    
    double t = t_num / t_denom;
    if (t < -EPS || t > 1.0 + EPS) return false;
    
    double ix = a.x + t * (b.x - a.x);
    double iy = a.y + t * (b.y - a.y);
    
    return ix * dir.x + iy * dir.y > EPS;
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<Segment> wolves(N), sheep(M);
    
    for(int i = 0; i < N; i++) {
        cin >> wolves[i].p1.x >> wolves[i].p1.y >> wolves[i].p2.x >> wolves[i].p2.y;
    }
    
    for(int i = 0; i < M; i++) {
        cin >> sheep[i].p1.x >> sheep[i].p1.y >> sheep[i].p2.x >> sheep[i].p2.y;
    }
    
    set<double> angles;
    for(auto& w : wolves) {
        angles.insert(atan2(w.p1.y, w.p1.x));
        angles.insert(atan2(w.p2.y, w.p2.x));
    }
    for(auto& s : sheep) {
        angles.insert(atan2(s.p1.y, s.p1.x));
        angles.insert(atan2(s.p2.y, s.p2.x));
    }
    
    vector<double> angleList(angles.begin(), angles.end());
    
    set<int> hitWolves;
    int shots = 0;
    
    while(hitWolves.size() < N) {
        int best = -1;
        int bestNew = -1;
        
        for(double angle : angleList) {
            bool hitsWolf = false, hitsAny = false;
            set<int> newWolves;
            
            for(int i = 0; i < N; i++) {
                if(rayIntersectsSegment({0,0}, angle, wolves[i])) {
                    hitsWolf = true;
                    if(hitWolves.find(i) == hitWolves.end()) {
                        newWolves.insert(i);
                    }
                }
            }
            
            for(int i = 0; i < M; i++) {
                if(rayIntersectsSegment({0,0}, angle, sheep[i])) {
                    hitsAny = true;
                    break;
                }
            }
            
            if(!hitsAny && newWolves.size() > bestNew) {
                best = angle;
                bestNew = newWolves.size();
            }
        }
        
        if(best < 0 || bestNew == 0) {
            cout << "No solution\n";
            return 0;
        }
        
        for(int i = 0; i < N; i++) {
            if(rayIntersectsSegment({0,0}, best, wolves[i])) {
                hitWolves.insert(i);
            }
        }
        shots++;
    }
    
    cout << shots << "\n";
    return 0;
}
