#include<bits/stdc++.h>
using namespace std;

typedef long double ld;

struct Point {
    ld x, y;
    Point(ld x=0, ld y=0): x(x), y(y) {}
    Point operator+(const Point& p) const { return Point(x+p.x, y+p.y); }
    Point operator-(const Point& p) const { return Point(x-p.x, y-p.y); }
    Point operator*(ld t) const { return Point(x*t, y*t); }
};

ld cross(Point a, Point b) {
    return a.x*b.y - a.y*b.x;
}

ld polygonArea(vector<Point> p) {
    ld a = 0;
    int n = p.size();
    for(int i = 0; i < n; i++) {
        a += cross(p[i], p[(i+1)%n]);
    }
    return abs(a) / 2.0;
}

int main(){
    ios_base::precision(15);
    int n;
    cin >> n;
    
    vector<Point> v(n);
    for(int i = 0; i < n; i++) {
        cin >> v[i].x >> v[i].y;
    }
    
    vector<int> k(n);
    for(int i = 0; i < n; i++) {
        cin >> k[i];
        if(i < n-1) {
            char c;
            cin >> c;
        }
    }
    
    ld totalArea = polygonArea(v);
    int sumK = 0;
    for(int ki : k) sumK += ki;
    
    Point center(0, 0);
    for(int i = 0; i < n; i++) center = center + v[i];
    center = center * (1.0L/n);
    
    for(int i = 0; i < n; i++) {
        int next = (i+1) % n;
        ld target = totalArea * k[i] / sumK;
        
        vector<Point> tri = {v[i], v[next], center};
        ld baseArea = polygonArea(tri);
        
        ld ratio = sqrt(target / baseArea);
        Point p = v[i] + (center - v[i]) * ratio;
        
        cout << "3 " << v[i].x << " " << v[i].y << " " << v[next].x << " " << v[next].y << " " << p.x << " " << p.y << "\n";
    }
    
    return 0;
}
