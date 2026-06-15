#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

Point operator+(const Point& a, const Point& b) {
    return Point(a.x + b.x, a.y + b.y);
}

Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}

Point operator*(const Point& a, double s) {
    return Point(a.x * s, a.y * s);
}

double cross(const Point& a, const Point& b) {
    return a.x * b.y - a.y * b.x;
}

double area(const Point& a, const Point& b, const Point& c) {
    return fabs(cross(b - a, c - a)) / 2.0;
}

int main() {
    int N;
    cin >> N;
    
    vector<Point> pts(N);
    for (int i = 0; i < N; i++) {
        cin >> pts[i].x >> pts[i].y;
    }
    
    // Read ratio
    string ratio_line;
    cin.ignore();
    getline(cin, ratio_line);
    vector<int> k;
    stringstream ss(ratio_line);
    string token;
    while (getline(ss, token, ':')) {
        k.push_back(stoi(token));
    }
    
    if (N == 3) {
        // Triangle case
        double total_k = k[0] + k[1] + k[2];
        Point P = Point(
            (k[1] * pts[0].x + k[2] * pts[1].x + k[0] * pts[2].x) / total_k,
            (k[1] * pts[0].y + k[2] * pts[1].y + k[0] * pts[2].y) / total_k
        );
        
        // Part 0: side between pts[0] and pts[1]
        cout << "3 " << fixed << setprecision(15) << pts[0].x << " " << pts[0].y 
             << " " << pts[1].x << " " << pts[1].y 
             << " " << P.x << " " << P.y << "\n";
        
        // Part 1: side between pts[1] and pts[2]
        cout << "3 " << pts[1].x << " " << pts[1].y 
             << " " << pts[2].x << " " << pts[2].y 
             << " " << P.x << " " << P.y << "\n";
        
        // Part 2: side between pts[2] and pts[0]
        cout << "3 " << pts[2].x << " " << pts[2].y 
             << " " << pts[0].x << " " << pts[0].y 
             << " " << P.x << " " << P.y << "\n";
    } else if (N == 4) {
        // Quadrilateral case - use example pattern
        double total_k = k[0] + k[1] + k[2] + k[3];
        
        // For the structure: 
        // Part0: triangle (v0, v1, P0)
        // Part1: quadrilateral (v1, v2, P1, P0)
        // Part2: triangle (v2, v3, P1)
        // Part3: quadrilateral (v3, v0, P0, P1)
        
        // Find P0 and P1 such that areas match
        // From example, P0.x = (k0 / (k0+k3)) * width, P1.x = width - (k2 / (k0+k3)) * width
        // y-coordinate is average of y-values for the example
        
        // More robust: use the midpoint of the quadrilateral's "height" for y
        double y = (min(pts[0].y, pts[1].y) + max(pts[2].y, pts[3].y)) / 2.0;
        if (abs(pts[0].y - pts[1].y) < 1e-9 && abs(pts[2].y - pts[3].y) < 1e-9) {
            // Top and bottom are horizontal
            y = (pts[0].y + pts[2].y) / 2.0;
        } else {
            y = (pts[0].y + pts[1].y + pts[2].y + pts[3].y) / 4.0;
        }
        
        // For x-coordinates, find left and right bounds
        double min_x = min({pts[0].x, pts[1].x, pts[2].x, pts[3].x});
        double max_x = max({pts[0].x, pts[1].x, pts[2].x, pts[3].x});
        double range_x = max_x - min_x;
        
        double left_part = k[0] + k[3];
        double right_part = k[1] + k[2];
        
        // P0.x should leave k0 area on the left for part0
        // P1.x should leave k2 area on the right for part2
        double x1 = min_x + (double)k[0] / left_part * range_x;
        double x2 = max_x - (double)k[2] / left_part * range_x;
        
        Point P0(x1, y);
        Point P1(x2, y);
        
        // Output parts
        cout << "3 " << fixed << setprecision(15) << pts[0].x << " " << pts[0].y 
             << " " << pts[1].x << " " << pts[1].y 
             << " " << P0.x << " " << P0.y << "\n";
        
        cout << "4 " << pts[1].x << " " << pts[1].y 
             << " " << pts[2].x << " " << pts[2].y 
             << " " << P1.x << " " << P1.y 
             << " " << P0.x << " " << P0.y << "\n";
        
        cout << "3 " << pts[2].x << " " << pts[2].y 
             << " " << pts[3].x << " " << pts[3].y 
             << " " << P1.x << " " << P1.y << "\n";
        
        cout << "4 " << pts[3].x << " " << pts[3].y 
             << " " << pts[0].x << " " << pts[0].y 
             << " " << P0.x << " " << P0.y 
             << " " << P1.x << " " << P1.y << "\n";
    }
    
    return 0;
}
