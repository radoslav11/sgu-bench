#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

const double EPS = 1e-7;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    
    Point operator*(double scalar) const {
        return Point(x * scalar, y * scalar);
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    int n;
    std::cin >> n;
    
    std::vector<Point> midpoints(n);
    for (int i = 0; i < n; i++) {
        std::cin >> midpoints[i].x >> midpoints[i].y;
    }
    
    // Check consistency condition for even n
    if (n % 2 == 0) {
        Point S(0, 0);
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                S = S + midpoints[i];
            } else {
                S = S - midpoints[i];
            }
        }
        if (std::abs(S.x) > EPS || std::abs(S.y) > EPS) {
            std::cout << "NO" << std::endl;
            return 0;
        }
    }
    
    std::vector<Point> vertices(n);
    
    if (n % 2 == 1) {
        // Odd n: alternating sum starting with +
        Point V0(0, 0);
        for (int i = 0; i < n; i++) {
            if (i % 2 == 0) {
                V0 = V0 + midpoints[i];
            } else {
                V0 = V0 - midpoints[i];
            }
        }
        vertices[0] = V0;
    } else {
        // Even n: use the formula
        double coefficient = 2.0 / n;
        Point sum(0, 0);
        for (int j = 0; j < n - 1; j++) {
            int weight = n - 1 - j;
            double sign = (j % 2 == 0) ? 1.0 : -1.0;
            Point term = midpoints[j] * (weight * sign);
            sum = sum + term;
        }
        vertices[0] = sum * coefficient;
    }
    
    // Compute remaining vertices
    for (int i = 1; i < n; i++) {
        vertices[i] = midpoints[i - 1] * 2 - vertices[i - 1];
    }
    
    std::cout << "YES" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < n; i++) {
        std::cout << vertices[i].x << " " << vertices[i].y << std::endl;
    }
    
    return 0;
}
