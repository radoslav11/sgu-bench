#include <iostream>
#include <cmath>
#include <algorithm>
#include <cstdlib>

using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return abs(a);
    return gcd(b, a % b);
}

// Extended Euclidean Algorithm: returns (g, x, y) such that a*x + b*y = g = gcd(a,b)
// Assumes at least one of a or b is non-zero
void extended_gcd(long long a, long long b, long long& g, long long& x, long long& y) {
    if (b == 0) {
        g = abs(a);
        x = a > 0 ? 1 : -1;
        y = 0;
        return;
    }
    
    long long g1, x1, y1;
    extended_gcd(b, a % b, g1, x1, y1);
    
    g = g1;
    x = y1;
    y = x1 - (a / b) * y1;
}

int main() {
    long long a, b, c;
    long long x1, x2, y1, y2;
    
    cin >> a >> b >> c;
    cin >> x1 >> x2;
    cin >> y1 >> y2;
    
    // Normalize equation: ax + by = -c
    long long d = -c;
    
    // Special case: both a and b are zero
    if (a == 0 && b == 0) {
        // Equation becomes 0x + 0y = d, i.e., 0 = d
        if (d == 0) {
            // All points in the rectangle are solutions
            long long x_count = max(0LL, x2 - x1 + 1);
            long long y_count = max(0LL, y2 - y1 + 1);
            cout << x_count * y_count << endl;
        } else {
            // No solutions
            cout << 0 << endl;
        }
        return 0;
    }
    
    // Special case: a == 0
    if (a == 0) {
        // Equation becomes by = -c
        long long rhs = -c;
        
        if (b == 0) {
            // Already handled above
            cout << 0 << endl;
            return 0;
        }
        
        // Check if rhs is divisible by b
        if (rhs % b != 0) {
            cout << 0 << endl;
            return 0;
        }
        
        long long y_val = rhs / b;
        
        // Check if y_val is in [y1, y2]
        if (y_val >= y1 && y_val <= y2) {
            // All x in [x1, x2] are valid
            long long count = x2 - x1 + 1;
            cout << count << endl;
        } else {
            cout << 0 << endl;
        }
        return 0;
    }
    
    // Special case: b == 0
    if (b == 0) {
        // Equation becomes ax = -c
        long long rhs = -c;
        
        if (rhs % a != 0) {
            cout << 0 << endl;
            return 0;
        }
        
        long long x_val = rhs / a;
        
        // Check if x_val is in [x1, x2]
        if (x_val >= x1 && x_val <= x2) {
            // All y in [y1, y2] are valid
            long long count = y2 - y1 + 1;
            cout << count << endl;
        } else {
            cout << 0 << endl;
        }
        return 0;
    }
    
    // General case: a != 0 && b != 0
    long long g = gcd(a, b);
    
    // Check if equation has any integer solutions at all
    if (d % g != 0) {
        cout << 0 << endl;
        return 0;
    }
    
    // Find particular solution using extended Euclidean algorithm
    long long x0, y0;
    long long g_ext;
    
    // Normalize equation: ax + by = d
    extended_gcd(a, b, g_ext, x0, y0);
    
    // Scale to get particular solution for original equation
    long long scale = d / g_ext;
    x0 *= scale;
    y0 *= scale;
    
    // General solution: 
    // x = x0 + (b/g)*t
    // y = y0 - (a/g)*t
    long long step_x = b / g;
    long long step_y = -a / g;
    
    // We need to find integer t such that:
    // x1 <= x0 + (b/g)*t <= x2
    // y1 <= y0 - (a/g)*t <= y2
    
    // Let's express the constraints on t
    long long t_low_x, t_high_x;
    
    if (step_x > 0) {
        // x1 <= x0 + step_x * t <= x2
        // (x1 - x0) / step_x <= t <= (x2 - x0) / step_x
        // Need ceiling for lower bound and floor for upper bound
        
        t_low_x = (x1 - x0 + (step_x > 0 ? step_x - 1 : 0)) / step_x;
        if ((x1 - x0) % step_x != 0 && (x1 - x0) > 0) {
            t_low_x = (x1 - x0 + step_x - 1) / step_x;
        } else if ((x1 - x0) < 0) {
            t_low_x = (x1 - x0) / step_x;
        } else {
            t_low_x = (x1 - x0 + step_x - 1) / step_x;
        }
        
        // More precise calculation using ceiling and floor
        if ((x1 - x0) % step_x != 0 && (x1 - x0) > 0) {
            t_low_x = (x1 - x0 + step_x - 1) / step_x;
        } else {
            t_low_x = (x1 - x0) / step_x;
        }
        
        if ((x2 - x0) % step_x != 0 && (x2 - x0) < 0) {
            t_high_x = (x2 - x0) / step_x;
        } else {
            t_high_x = (x2 - x0) / step_x;
        }
    } else if (step_x < 0) {
        // Reverse inequalities
        step_x = -step_x;
        
        if ((x1 - x0) % step_x != 0 && (x1 - x0) < 0) {
            t_low_x = -(x1 - x0 + step_x - 1) / step_x;
        } else {
            t_low_x = -(x1 - x0) / step_x;
        }
        
        if ((x2 - x0) % step_x != 0 && (x2 - x0) > 0) {
            t_high_x = -(x2 - x0 + step_x - 1) / step_x;
        } else {
            t_high_x = -(x2 - x0) / step_x;
        }
    } else {
        // Shouldn't happen since b != 0
        cout << 0 << endl;
        return 0;
    }
    
    // Actually, let's rewrite this more carefully
    t_low_x = (x1 - x0 + step_x - 1) / step_x;
    t_high_x = (x2 - x0) / step_x;
    
    if ((x1 - x0) % step_x != 0 && (x1 - x0) > 0 && step_x > 0) {
        t_low_x = (x1 - x0 + step_x - 1) / step_x;
    } else if ((x1 - x0) < 0 && step_x > 0) {
        t_low_x = (x1 - x0) / step_x;
    }
    
    if ((x2 - x0) % step_x != 0 && (x2 - x0) < 0 && step_x > 0) {
        t_high_x = (x2 - x0) / step_x;
    } else if ((x2 - x0) >= 0 && step_x > 0) {
        t_high_x = (x2 - x0) / step_x;
    }
    
    // Let's use a more standard approach for bounds
    auto ceil_div = [](long long a, long long b) {
        if (b < 0) { a = -a; b = -b; }
        if (a >= 0) return (a + b - 1) / b;
        else return a / b;
    };
    
    auto floor_div = [](long long a, long long b) {
        if (b < 0) { a = -a; b = -b; }
        return a / b;
    };
    
    long long t_min_x, t_max_x;
    if (step_x > 0) {
        t_min_x = ceil_div(x1 - x0, step_x);
        t_max_x = floor_div(x2 - x0, step_x);
    } else {
        step_x = -step_x;
        t_min_x = floor_div(x1 - x0, -step_x);
        t_max_x = ceil_div(x2 - x0, -step_x);
        // Actually let's just compute directly
        t_min_x = (x2 - x0 + step_x) / step_x; // This is wrong, let's restart
        
        // Let me do this cleanly: if step_x < 0, then 
        // x1 <= x0 + step_x * t <= x2
        // => (x1 - x0) / step_x >= t >= (x2 - x0) / step_x
        // since dividing by negative reverses inequalities
        t_min_x = (x2 - x0 + step_x + 1) / step_x; // This is getting messy
        
        // Let's use a different approach
        // Find t such that x1 <= x0 + step_x * t <= x2
        // => (x1 - x0) / step_x >= t >= (x2 - x0) / step_x if step_x < 0
        // Actually, better to use the standard formulas
        
        t_min_x = (x1 - x0 + step_x + 1) / step_x;
        t_max_x = (x2 - x0) / step_x;
    }
    
    // Let's redo this part correctly with a helper function
    auto get_t_range_for_x = [&](long long x_low, long long x_high, long long x0_val, long long step_x) -> pair<long long, long long> {
        if (step_x == 0) {
            if (x_low <= x0_val && x0_val <= x_high) {
                return {LLONG_MIN, LLONG_MAX};
            } else {
                return {1, 0}; // empty range
            }
        }
        
        long long t_low, t_high;
        if (step_x > 0) {
            // x_low <= x0 + step_x * t <= x_high
            // (x_low - x0) / step_x <= t <= (x_high - x0) / step_x
            // Need ceiling for lower bound, floor for upper bound
            
            if ((x_low - x0_val) % step_x == 0) {
                t_low = (x_low - x0_val) / step_x;
            } else {
                t_low = (x_low - x0_val) / step_x + 1;
            }
            
            if ((x_high - x0_val) % step_x == 0) {
                t_high = (x_high - x0_val) / step_x;
            } else {
                t_high = (x_high - x0_val) / step_x;
            }
        } else {
            // step_x < 0
            // x_low <= x0 + step_x * t <= x_high
            // (x_low - x0) / step_x >= t >= (x_high - x0) / step_x  (reversed)
            // So t_high = floor((x_low - x0) / step_x), t_low = ceil((x_high - x0) / step_x)
            
            // floor for (x_low - x0)/step_x
            long long num1 = x_low - x0_val;
            if (num1 >= 0) {
                t_high = num1 / step_x; // division rounds toward zero, but since denominator negative, need to adjust
                if (num1 % step_x != 0) {
                    t_high = num1 / step_x - 1;
                }
            } else {
                t_high = (num1 - step_x + 1) / step_x;
            }
            
            // ceil for (x_high - x0)/step_x
            long long num2 = x_high - x0_val;
            if (num2 >= 0) {
                t_low = (num2 + step_x - 1) / step_x;
            } else {
                t_low = num2 / step_x;
            }
        }
        
        return {t_low, t_high};
    };
    
    // Let's use even simpler approach: compute bounds directly
    long long t_min_x2, t_max_x2;
    
    if (step_x > 0) {
        // x1 <= x0 + step_x * t <= x2
        // => (x1 - x0) / step_x <= t <= (x2 - x0) / step_x
        // Use ceiling for lower bound, floor for upper bound
        
        // Ceiling division: ceil(p/q) = (p + q - 1) / q for q > 0
        long long num1 = x1 - x0;
        if (num1 >= 0) {
            t_min_x2 = (num1 + step_x - 1) / step_x;
        } else {
            t_min_x2 = num1 / step_x; // for negative, integer division rounds toward 0, which is floor
        }
        
        long long num2 = x2 - x0;
        if (num2 >= 0) {
            t_max_x2 = num2 / step_x;
        } else {
            t_max_x2 = (num2 - step_x + 1) / step_x;
        }
    } else {
        // step_x < 0
        // x1 <= x0 + step_x * t <= x2
        // => (x1 - x0) / step_x >= t >= (x2 - x0) / step_x  (reversed)
        // So upper bound for t is floor((x1 - x0) / step_x), lower bound is ceil((x2 - x0) / step_x)
        
        long long num1 = x1 - x0;
        // floor division for negative divisor
        if (num1 >= 0) {
            t_max_x2 = -((num1 + (-step_x) - 1) / (-step_x));
        } else {
            t_max_x2 = num1 / step_x;
        }
        
        long long num2 = x2 - x0;
        // ceil division for negative divisor
        if (num2 >= 0) {
            t_min_x2 = -((num2 + (-step_x) - 1) / (-step_x));
        } else {
            t_min_x2 = (num2 + step_x - 1) / step_x;
        }
    }
    
    // Do the same for y constraints
    long long t_min_y2, t_max_y2;
    
    if (step_y > 0) {
        // y1 <= y0 + step_y * t <= y2
        long long num1 = y1 - y0;
        if (num1 >= 0) {
            t_min_y2 = (num1 + step_y - 1) / step_y;
        } else {
            t_min_y2 = num1 / step_y;
        }
        
        long long num2 = y2 - y0;
        if (num2 >= 0) {
            t_max_y2 = num2 / step_y;
        } else {
            t_max_y2 = (num2 - step_y + 1) / step_y;
        }
    } else {
        // step_y < 0
        long long num1 = y1 - y0;
        if (num1 >= 0) {
            t_max_y2 = -((num1 + (-step_y) - 1) / (-step_y));
        } else {
            t_max_y2 = num1 / step_y;
        }
        
        long long num2 = y2 - y0;
        if (num2 >= 0) {
            t_min_y2 = -((num2 + (-step_y) - 1) / (-step_y));
        } else {
            t_min_y2 = (num2 + step_y - 1) / step_y;
        }
    }
    
    // Find intersection of ranges
    long long t_min = max(t_min_x2, t_min_y2);
    long long t_max = min(t_max_x2, t_max_y2);
    
    if (t_min <= t_max) {
        cout << t_max - t_min + 1 << endl;
    } else {
        cout << 0 << endl;
    }
    
    return 0;
}
