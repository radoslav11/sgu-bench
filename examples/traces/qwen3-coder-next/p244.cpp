#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

const double EPS = 1e-9;
const double MAX_COORD = 1e8;

// Structure to represent a point
struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
    
    Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
    Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
    Point operator*(double k) const { return Point(x * k, y * k); }
    double dot(const Point& p) const { return x * p.x + y * p.y; }
    double norm() const { return sqrt(x * x + y * y); }
};

// Calculate distance between two points
double dist(const Point& a, const Point& b) {
    return (a - b).norm();
}

// Given points A, B, C, compute:
// - foot of altitude from A to BC (H)
// - point D on BC where AD is angle bisector
// - point M midpoint of BC (median)
// Return actual lengths AH, AD, AM
void computeElements(const Point& A, const Point& B, const Point& C,
                     Point& H, Point& D, Point& M,
                     double& AH, double& AD, double& AM) {
    // Midpoint M
    M = Point((B.x + C.x) / 2.0, (B.y + C.y) / 2.0);
    
    // Altitude foot H: projection of A onto line BC
    Point BC = C - B;
    Point BA = A - B;
    double t = BA.dot(BC) / BC.dot(BC);
    t = max(0.0, min(1.0, t)); // clamp to segment (but for triangle it should be between 0 and 1)
    H = B + BC * t;
    
    // Angle bisector point D on BC
    // Angle bisector theorem: BD/DC = AB/AC
    double AB = dist(A, B);
    double AC = dist(A, C);
    double BC_len = dist(B, C);
    
    if (AB + AC < EPS) {
        // Degenerate case
        D = B;
    } else {
        double BD = (AB / (AB + AC)) * BC_len;
        double t_d = BD / BC_len;
        t_d = max(0.0, min(1.0, t_d));
        D = B + BC * t_d;
    }
    
    AH = dist(A, H);
    AD = dist(A, D);
    AM = dist(A, M);
}

// Main solver
bool solve(double h, double d, double m) {
    // Try to construct triangle with A at (0, h), BC on x-axis (y=0)
    // This simplifies the problem: altitude from A is vertical, length = h
    // So set A = (0, h), B = (-x, 0), C = (y, 0), with x >= 0, y >= 0, and x + y > 0
    
    // Then:
    // M = midpoint of BC = ((-x + y)/2, 0)
    // AM = distance from A to M = sqrt(((y - x)/2)^2 + h^2)
    // => ((y - x)/2)^2 + h^2 = m^2
    // => (y - x)^2 = 4*(m^2 - h^2)
    // So we require m >= h (otherwise no solution)
    
    if (m < h - EPS) {
        return false;
    }
    
    // Now consider the angle bisector length formula:
    // AD^2 = AB * AC * (1 - (BC^2 / (AB + AC)^2))
    // Let AB = sqrt(x^2 + h^2), AC = sqrt(y^2 + h^2), BC = x + y
    // We need AD = d
    
    // Let's denote:
    // Let u = y - x, v = y + x
    // From AM: u^2 = 4*(m^2 - h^2) => u = sqrt(4*(m^2 - h^2)) = 2*sqrt(m^2 - h^2)
    // So y - x = u (we can assume u >= 0 without loss of generality, otherwise swap B and C)
    
    double u = 2 * sqrt(max(0.0, m*m - h*h));
    // So y = x + u
    
    // Now define function f(x) = AD^2 - d^2
    // We need to find x >= 0 such that f(x) = 0
    
    // AD^2 = AB * AC * (1 - (BC^2 / (AB + AC)^2))
    // AB = sqrt(x^2 + h^2), AC = sqrt((x+u)^2 + h^2), BC = x + (x+u) = 2x + u
    
    auto f = [&](double x) -> double {
        if (x < 0) return 1e18;
        double AB = sqrt(x*x + h*h);
        double AC = sqrt((x+u)*(x+u) + h*h);
        double BC = 2*x + u;
        
        if (BC < EPS) return 1e18;
        
        double term = 1.0 - (BC * BC) / ((AB + AC) * (AB + AC));
        if (term < 0) term = 0;
        
        double AD2 = AB * AC * term;
        return AD2 - d*d;
    };
    
    // Check if solution exists
    // As x -> 0+, f(0) = AB0 * AC0 * (1 - BC0^2/(AB0+AC0)^2) - d^2
    // AB0 = h, AC0 = sqrt(u*u + h*h), BC0 = u
    double f0 = f(0.0);
    
    // As x -> infinity:
    // AB ~ x, AC ~ x+u ~ x, BC ~ 2x
    // term ~ 1 - (4x^2)/( (2x)^2 ) = 1 - 1 = 0
    // AB*AC ~ x^2, but term ~ 1 - (4x^2)/(4x^2) = 0, more precisely:
    // term = 1 - (4x^2) / ( (x + x+u)^2 ) = 1 - (4x^2)/( (2x+u)^2 ) = 1 - 4x^2/(4x^2 + 4ux + u^2) 
    // = (4ux + u^2)/(4x^2 + 4ux + u^2) ~ (4ux)/(4x^2) = u/x -> 0
    // So AD^2 ~ x * x * (u/x) = x * u -> infinity? Wait, let me recalc:
    // Actually, better use known formula: length of angle bisector from A is:
    // AD = (2 * b * c * cos(A/2)) / (b + c)
    // or standard formula: AD^2 = bc * (1 - (a^2/(b+c)^2))
    // So AD^2 = AB * AC - (AB * AC * BC^2) / (AB + AC)^2
    
    // Let's compute limit as x->infinity:
    // AB ~ x, AC ~ x, BC ~ 2x
    // AD^2 ~ x * x * (1 - (4x^2)/( (2x)^2 )) = x^2 * (1 - 1) = 0? That can't be right.
    
    // Actually:
    // (AB + AC)^2 = (x + sqrt(x^2 + u^2 + 2ux + h^2))^2 ~ (x + x + u/2)^2 = (2x + u/2)^2 = 4x^2 + ...
    // BC^2 = (2x + u)^2 = 4x^2 + 4ux + u^2
    // So BC^2/(AB+AC)^2 ~ (4x^2 + 4ux) / (4x^2 + 2ux) -> 1
    // So term = 1 - something -> 0
    // But how fast?
    // Let AB = x * sqrt(1 + (h/x)^2) = x (1 + h^2/(2x^2) - ...)
    // AC = (x+u) * sqrt(1 + h^2/(x+u)^2) = (x+u)(1 + h^2/(2(x+u)^2) - ...) = x+u + h^2/(2(x+u)) - ...
    // AB + AC = 2x + u + h^2/(2x) + ... (approx)
    // (AB+AC)^2 = 4x^2 + 4x*u + u^2 + ...
    // BC^2 = (2x + u)^2 = 4x^2 + 4x*u + u^2
    // So BC^2/(AB+AC)^2 = 1 - O(1/x^2)
    // So term = O(1/x^2)
    // AB * AC ~ x*(x+u) ~ x^2
    // So AD^2 ~ x^2 * (c/x^2) = c (constant!)
    
    // Known formula: for large triangle with fixed altitude, angle bisector length approaches h / cos(A/2)
    // But angle A approaches 0 as x->infinity? Actually no, as x->infinity with fixed h, angle at A approaches 0
    // So cos(A/2) -> cos(0) = 1, so AD -> h
    
    // Actually, let me verify with a specific example: h=1, u=0 (so x=y), then triangle is isosceles
    // AB = AC = sqrt(x^2+1), BC=2x
    // AD^2 = AB^2 * (1 - (4x^2)/(4 AB^2)) = (x^2+1) * (1 - x^2/(x^2+1)) = (x^2+1)*(1/(x^2+1)) = 1
    // So AD = 1 = h. Interesting!
    
    // So in the isosceles case (u=0), AD = h always.
    // In general, as x->infinity, AD approaches h (the altitude).
    
    // So f(x) = AD^2 - d^2, and as x->infinity, AD->h, so f(x)-> h^2 - d^2.
    
    // Therefore:
    // - If d > h, then as x->infinity, f(x) -> h^2 - d^2 < 0
    //   At x=0, we need to check sign of f(0)
    // - If d < h, then as x->infinity, f(x) -> positive, but at x=0 might be negative or positive
    
    // Let's compute f(0) explicitly:
    double AB0 = h;
    double AC0 = sqrt(u*u + h*h);
    double BC0 = u;
    double term0 = 1.0 - (BC0 * BC0) / ((AB0 + AC0) * (AB0 + AC0));
    double AD0_2 = AB0 * AC0 * term0;
    double f0_val = AD0_2 - d*d;
    
    // And limit as x->inf:
    double finf_val = h*h - d*d;
    
    // Now we need to see if f(x)=0 has a solution for x>=0
    
    // Case 1: d > h
    // Then finf_val < 0
    // If f0_val > 0, then there's a root in (0, infinity)
    // If f0_val <= 0, then no root (since f is continuous and goes from non-positive to negative)
    
    // Case 2: d < h
    // Then finf_val > 0
    // If f0_val < 0, then there's a root in (0, infinity)
    // If f0_val >= 0, then might be no root or two roots
    
    // Case 3: d == h
    // Then finf_val = 0, and in isosceles case (u=0) f(x)=0 for all x
    // But for u != 0, need to check
    
    // Actually, in the isosceles case (u=0), we saw AD = h always, so f(x)=0 for all x
    // So if u=0 and d=h, then any x works
    
    // Let's handle the isosceles case first: u = 0
    if (fabs(u) < EPS) {
        // Then y = x, so triangle is isosceles with AB=AC
        // In this case, AD = h (as shown above)
        if (fabs(d - h) < EPS) {
            // Solution exists: take x=1 (arbitrary positive)
            double x_val = 1.0;
            double y_val = x_val; // since u=0
            Point A(0, h);
            Point B(-x_val, 0);
            Point C(y_val, 0);
            Point H, D, M;
            double AH, AD, AM;
            computeElements(A, B, C, H, D, M, AH, AD, AM);
            // Verify
            if (fabs(AH - h) < 1e-6 && fabs(AD - d) < 1e-6 && fabs(AM - m) < 1e-6) {
                cout << "YES" << endl;
                cout << fixed << setprecision(8) << A.x << " " << A.y << endl;
                cout << fixed << setprecision(8) << B.x << " " << B.y << endl;
                cout << fixed << setprecision(8) << C.x << " " << C.y << endl;
                return true;
            }
        }
        // If d != h, then no solution in isosceles case, and since u=0 is the only possibility for m=h, 
        // and if m>h we wouldn't be in u=0 case, so for u=0 we only have solution when d=h
        return false;
    }
    
    // Now u > 0
    
    // We'll try to find x such that f(x)=0 using bisection or Newton, but need to bracket the root
    
    // First, check the behavior:
    // Let's try x=0 and a large x, say x=1000
    
    auto test_x = [&](double x) -> pair<double, double> {
        double AB = sqrt(x*x + h*h);
        double AC = sqrt((x+u)*(x+u) + h*h);
        double BC = 2*x + u;
        double term = 1.0 - (BC * BC) / ((AB + AC) * (AB + AC));
        double AD2 = AB * AC * term;
        return {AD2, sqrt(AD2)};
    };
    
    auto [ad2_0, ad_0] = test_x(0.0);
    auto [ad2_inf, ad_inf] = test_x(1000.0);
    
    // Actually, better to compute the theoretical limit: as x->inf, AD->h
    // So let's use x=1e6 for "infinity"
    auto [ad2_large, ad_large] = test_x(1e6);
    
    // Now, let's define the function we want to solve: g(x) = AD(x) - d
    auto g = [&](double x) -> double {
        double AB = sqrt(x*x + h*h);
        double AC = sqrt((x+u)*(x+u) + h*h);
        double BC = 2*x + u;
        double term = 1.0 - (BC * BC) / ((AB + AC) * (AB + AC));
        double AD2 = AB * AC * term;
        if (AD2 < 0) AD2 = 0;
        return sqrt(AD2) - d;
    };
    
    // We'll search in [0, XMAX] where XMAX is large enough
    double XMAX = 1e6;
    
    double g0 = g(0.0);
    double gXMAX = g(XMAX);
    
    // Check if solution exists
    bool solution_exists = false;
    double x_sol = -1;
    
    if (fabs(g0) < EPS) {
        solution_exists = true;
        x_sol = 0.0;
    } else if (fabs(gXMAX) < EPS) {
        solution_exists = true;
        x_sol = XMAX;
    } else if (g0 * gXMAX < 0) {
        // Root in (0, XMAX)
        // Use bisection
        double left = 0.0, right = XMAX;
        for (int iter = 0; iter < 100; iter++) {
            double mid = (left + right) / 2.0;
            double gm = g(mid);
            if (fabs(gm) < EPS) {
                x_sol = mid;
                solution_exists = true;
                break;
            }
            if (g0 * gm < 0) {
                right = mid;
                gXMAX = gm; // update for next iteration if needed
            } else {
                left = mid;
                g0 = gm;
            }
        }
        if (fabs(g(x_sol)) > EPS) {
            // Try to use the last mid
            x_sol = (left + right) / 2.0;
            if (fabs(g(x_sol)) < EPS) {
                solution_exists = true;
            }
        } else {
            solution_exists = true;
        }
    } else {
        // Both same sign, but maybe there's a local minimum/maximum
        // Check derivative or try a few points
        // Since as x->inf, g(x)-> h - d, and at x=0, g(0)=ad_0 - d
        // If d is between ad_0 and h, then there must be a root
        
        // Actually, we can check if d is between min(ad_0, h) and max(ad_0, h)
        // But is AD(x) monotonic? Not necessarily, but in practice for triangles it often is.
        
        // Let's try to find if there's a solution by checking a few points
        // or use the fact that AD(x) is continuous and goes from ad_0 to h
        
        double min_ad = min(ad_0, ad_large);
        double max_ad = max(ad_0, ad_large);
        
        if (d >= min_ad - EPS && d <= max_ad + EPS) {
            // There might be a solution; try bisection in [0, XMAX]
            double left = 0.0, right = XMAX;
            // But if g0 and gXMAX have same sign, and d is in between, then there must be at least one root?
            // Only if the function is continuous (which it is) and we're in the right interval
            // However, if both g0 and gXMAX are positive and d < min, then no solution; if d > max, no solution
            // But if d is between, and both same sign, then there must be two roots (one min, one max) or none
            // This is getting complicated.
            
            // Alternative: use the known formula for angle bisector and solve numerically with more points
            // Or try to use the median condition to reduce variables
            
            // Let me recall: we have two conditions:
            // 1. AM = m => (y-x)^2 = 4(m^2 - h^2) => u = 2*sqrt(m^2 - h^2) [fixed]
            // 2. AD = d
            // And we're varying x (with y = x + u)
            
            // Maybe try to use a different approach: assume B=(-1,0), C=(c,0), A=(0,h)
            // Then M = ((c-1)/2, 0), AM^2 = ((c-1)/2)^2 + h^2 = m^2 => (c-1)^2 = 4(m^2 - h^2)
            // So c = 1 ± 2*sqrt(m^2 - h^2)
            // Take c = 1 + 2*sqrt(m^2 - h^2) (so c > 1, triangle oriented left to right)
            
            // Then AB = sqrt(1 + h^2), AC = sqrt(c^2 + h^2), BC = c + 1
            // Then AD^2 = AB * AC * (1 - (BC^2)/(AB+AC)^2)
            
            // So we can compute AD directly from c, and check if it equals d
            
            // This is actually what we're doing, but with x=1 (B at (-1,0)) and y=c
            // In our previous parametrization, x=1, y=c, u = c - (-1)? Wait no.
            // In our first parametrization: B=(-x,0), C=(y,0), so BC length = x+y
            // Midpoint x-coordinate = (y - x)/2, so AM^2 = ((y-x)/2)^2 + h^2 = m^2
            // So y - x = 2*sqrt(m^2 - h^2) = u
            // If we fix x=1, then y = 1 + u
            // Then AB = sqrt(1 + h^2), AC = sqrt((1+u)^2 + h^2), BC = 1 + 1 + u = 2 + u
            // Then compute AD and see if it equals d.
            
            // So let's try x=1:
            double ad_test = g(1.0) + d;
            if (fabs(ad_test - d) < 1e-6) {
                solution_exists = true;
                x_sol = 1.0;
            } else {
                // Try to solve more robustly with multiple attempts
                // Try different starting points
                vector<double> candidates = {0.0, 0.1, 1.0, 10.0, 100.0, 1000.0};
                for (double cand : candidates) {
                    if (fabs(g(cand)) < 1e-6) {
                        solution_exists = true;
                        x_sol = cand;
                        break;
                    }
                }
                if (!solution_exists) {
                    // Check the function values at candidates
                    double min_g = 1e18, max_g = -1e18;
                    for (double cand : candidates) {
                        double val = g(cand);
                        if (val < min_g) min_g = val;
                        if (val > max_g) max_g = val;
                    }
                    if (min_g <= 0 && max_g >= 0) {
                        // Root exists in the range of candidates
                        // Use bisection between min and max x where g changes sign
                        double left_x = 0.0, right_x = 1000.0;
                        double left_g = g0, right_g = gXMAX;
                        // Find sign change
                        int signs = 0;
                        for (int i = 0; i < (int)candidates.size()-1; i++) {
                            double x1 = candidates[i], x2 = candidates[i+1];
                            double g1 = g(x1), g2 = g(x2);
                            if (g1 * g2 <= 0) {
                                left_x = x1;
                                right_x = x2;
                                left_g = g1;
                                right_g = g2;
                                signs = 1;
                                break;
                            }
                        }
                        if (signs || (g0 * gXMAX <= 0)) {
                            for (int iter = 0; iter < 100; iter++) {
                                double mid = (left_x + right_x) / 2.0;
                                double gm = g(mid);
                                if (fabs(gm) < 1e-9) {
                                    x_sol = mid;
                                    solution_exists = true;
                                    break;
                                }
                                if (left_g * gm <= 0) {
                                    right_x = mid;
                                    right_g = gm;
                                } else {
                                    left_x = mid;
                                    left_g = gm;
                                }
                            }
                            if (fabs(g(x_sol)) > 1e-6) {
                                x_sol = (left_x + right_x) / 2.0;
                                solution_exists = fabs(g(x_sol)) < 1e-6;
                            } else {
                                solution_exists = true;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (solution_exists) {
        double x_val = x_sol;
        double y_val = x_val + u;
        Point A(0, h);
        Point B(-x_val, 0);
        Point C(y_val, 0);
        
        // Verify
        Point H, D, M;
        double AH, AD, AM;
        computeElements(A, B, C, H, D, M, AH, AD, AM);
        
        if (fabs(AH - h) < 1e-6 && fabs(AD - d) < 1e-6 && fabs(AM - m) < 1e-6) {
            cout << "YES" << endl;
            cout << fixed << setprecision(8) << A.x << " " << A.y << endl;
            cout << fixed << setprecision(8) << B.x << " " << B.y << endl;
            cout << fixed << setprecision(8) << C.x << " " << C.y << endl;
            return true;
        }
    }
    
    return false;
}

int main() {
    double h, d, m;
    cin >> h >> d >> m;
    
    // Check triangle inequality conditions for existence
    // Note: In any triangle, the median length satisfies: m_a >= h_a/2? Not exactly.
    // But we know: in a triangle, the median is at least the altitude times something? 
    // Actually, from geometry: the median to side a is m_a = 0.5 * sqrt(2b^2 + 2c^2 - a^2)
    // And altitude h_a = (2 * area) / a
    // But we don't know the sides.
    
    // Our method requires m >= h (since AM^2 = ((y-x)/2)^2 + h^2 >= h^2)
    // So if m < h, no solution.
    
    if (solve(h, d, m)) {
        return 0;
    } else {
        // Try swapping the roles? But the problem states: height AH, bisector AD, median AM
        // So AH is altitude from A, AD is bisector from A, AM is median from A
        // So our setup is correct: A is the vertex from which all three are drawn.
        
        // However, maybe the triangle is oriented differently. But we assumed A at (0,h) and BC on x-axis, which is general due to translation/rotation.
        
        // One more possibility: maybe the foot of the altitude is outside the segment BC (obtuse triangle)
        // Our earlier assumption that H is on segment BC might be wrong!
        // In an obtuse triangle, the altitude from A falls outside BC.
        
        // So we need to consider that case too.
        
        // Let me reconsider: 
        // Let A = (0, h), B = (0,0), C = (c, 0) with c > 0.
        // Then BC is on x-axis from (0,0) to (c,0).
        // Altitude from A to BC (x-axis) is vertical, foot H = (0,0) if we drop perpendicular? No.
        // Actually, BC is on x-axis, so altitude from A=(0,h) to BC (x-axis) is always to (0,0) only if we set it that way, but generally:
        // Let A = (p, h), B = (0,0), C = (c,0). Then the foot H of perpendicular from A to BC (x-axis) is (p, 0).
        // Then AH = h (good).
        // M = midpoint of BC = (c/2, 0)
        // AM = sqrt((p - c/2)^2 + h^2) = m  => (p - c/2)^2 = m^2 - h^2
        // So we require m >= h, and p = c/2 ± sqrt(m^2 - h^2)
        // D is on BC where AD is angle bisector.
        // By angle bisector theorem: BD/DC = AB/AC
        // AB = sqrt(p^2 + h^2), AC = sqrt((p - c)^2 + h^2)
        // BD = distance from B to D = x (say), DC = c - x
        // So x / (c - x) = AB / AC => x = c * AB / (AB + AC)
        // Then AD^2 = (p - x)^2 + h^2 = d^2
        
        // So now we have two variables: c and p (with p related to c by p = c/2 ± u, u = sqrt(m^2 - h^2))
        // Let's take p = c/2 + u (the other sign can be tried too)
        // Then x = c * AB / (AB + AC)
        // AB = sqrt(p^2 + h^2), AC = sqrt((p - c)^2 + h^2) = sqrt((c/2 + u - c)^2 + h^2) = sqrt((u - c/2)^2 + h^2)
        
        // So AD^2 = (p - x)^2 + h^2 = (c/2 + u - x)^2 + h^2 = d^2
        
        // Define function of c:
        auto try_solution = [&](double sign) -> bool {
            // p = c/2 + sign * u, where u = sqrt(m^2 - h^2)
            double u = sqrt(max(0.0, m*m - h*h));
            double p0 = sign * u; // p = c/2 + p0
            
            // Now define g(c) = AD - d
            auto g_c = [&](double c) -> double {
                if (c <= 0) return 1e18;
                double p = c/2 + p0;
                double AB = sqrt(p*p + h*h);
                double AC = sqrt((p - c)*(p - c) + h*h);
                
                // Angle bisector point D on BC: BD/DC = AB/AC
                double BD = c * AB / (AB + AC);
                double x_d = BD; // since B at 0, C at c, D at x_d
                double AD = sqrt((p - x_d)*(p - x_d) + h*h);
                return AD - d;
            };
            
            // Try c from small to large
            // As c->0+, AB ~ |p|, AC ~ |p|, BD ~ 0, so D->B, AD->AB ~ |p|
            // As c->inf, AB ~ c/2, AC ~ c/2, BD ~ c * (c/2) / (c) = c/2, so D-> midpoint, AD->AM = m
            // So g_c(c) -> m - d as c->inf
            
            // Try c=1, c=10, c=100, etc.
            for (double c_val = 0.1; c_val < 1e6; c_val *= 10) {
                double val = g_c(c_val);
                if (fabs(val) < 1e-6) {
                    double p = c_val/2 + p0;
                    Point A(p, h);
                    Point B(0, 0);
                    Point C(c_val, 0);
                    Point H(p, 0);
                    Point D;
                    // Compute D properly
                    double AB = dist(A, B);
                    double AC = dist(A, C);
                    double BC = c_val;
                    double BD = AB * BC / (AB + AC);
                    D = Point(BD, 0);
                    Point M(c_val/2, 0);
                    
                    double AH_len = dist(A, H);
                    double AD_len = dist(A, D);
                    double AM_len = dist(A, M);
                    
                    if (fabs(AH_len - h) < 1e-6 && fabs(AD_len - d) < 1e-6 && fabs(AM_len - m) < 1e-6) {
                        cout << "YES" << endl;
                        cout << fixed << setprecision(8) << A.x << " " << A.y << endl;
                        cout << fixed << setprecision(8) << B.x << " " << B.y << endl;
                        cout << fixed << setprecision(8) << C.x << " " << C.y << endl;
                        return true;
                    }
                }
            }
            
            // Bisection between c=0.01 and c=1e6
            double left_c = 0.01, right_c = 1e6;
            double g_left = g_c(left_c);
            double g_right = g_c(right_c);
            
            if (g_left * g_right > 0) {
                // Try to find a sign change
                for (double c_val = 0.01; c_val < 1e6; c_val *= 10) {
                    double g_mid = g_c(c_val);
                    if (c_val > 0.01) {
                        double g_prev = g_c(c_val/10);
                        if (g_prev * g_mid <= 0) {
                            left_c = c_val/10;
                            right_c = c_val;
                            g_left = g_prev;
                            g_right = g_mid;
                            goto bisect;
                        }
                    }
                }
                return false;
            }
            
            bisect:
            for (int iter = 0; iter < 100; iter++) {
                double mid_c = (left_c + right_c) / 2.0;
                double g_mid = g_c(mid_c);
                if (fabs(g_mid) < 1e-9) {
                    double p = mid_c/2 + p0;
                    Point A(p, h);
                    Point B(0, 0);
                    Point C(mid_c, 0);
                    Point H(p, 0);
                    Point D;
                    double AB = dist(A, B);
                    double AC = dist(A, C);
                    double BC = mid_c;
                    double BD = AB * BC / (AB + AC);
                    D = Point(BD, 0);
                    
                    double AH_len = dist(A, H);
                    double AD_len = dist(A, D);
                    double AM_len = dist(A, Point(mid_c/2, 0));
                    
                    if (fabs(AH_len - h) < 1e-6 && fabs(AD_len - d) < 1e-6 && fabs(AM_len - m) < 1e-6) {
                        cout << "YES" << endl;
                        cout << fixed << setprecision(8) << A.x << " " << A.y << endl;
                        cout << fixed << setprecision(8) << B.x << " " << B.y << endl;
                        cout << fixed << setprecision(8) << C.x << " " << C.y << endl;
                        return true;
                    }
                }
                if (g_left * g_mid <= 0) {
                    right_c = mid_c;
                    g_right = g_mid;
                } else {
                    left_c = mid_c;
                    g_left = g_mid;
                }
            }
            return false;
        };
        
        // Try both signs for p = c/2 ± u
        if (try_solution(1.0) || try_solution(-1.0)) {
            return 0;
        }
        
        cout << "NO" << endl;
        return 0;
    }
    
    return 0;
}
