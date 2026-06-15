#include <iostream>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef long double ld;

ld xA, yA, zA, xB, yB, zB;
ld delta_theta_target;

// Integrand for theta: h(z, C) = (C * sqrt(1 + 2*z^2)) / ((1 + z^2) * sqrt(1 + z^2 - C^2))
// Integrand for S: sqrt((1 + 2*z^2) / (1 + z^2 - C^2))

// Case where C > 1 and the geodesic turns at z_min = sgn(zA) * sqrt(C^2 - 1).
// For the integral, we use substitution z = z_min + u^2, so dz = 2u du.
// The singularity at z = z_min is canceled by the u in dz.

ld h_sub(ld u, ld z_min, ld C) {
    ld z = z_min + u * u;
    return (2.0 * C * sqrt(1.0 + 2.0 * z * z)) / (1.0 + z * z);
}

ld S_sub(ld u, ld z_min, ld C) {
    ld z = z_min + u * u;
    return 2.0 * sqrt(1.0 + 2.0 * z * z);
}

ld h_direct(ld z, ld C) {
    return (C * sqrt(1.0 + 2.0 * z * z)) / ((1.0 + z * z) * sqrt(1.0 + z * z - C * C));
}

ld S_direct(ld z, ld C) {
    return sqrt((1.0 + 2.0 * z * z) / (1.0 + z * z - C * C));
}

ld simpson_h_sub(ld u_max, ld z_min, ld C) {
    if (u_max <= 0) return 0;
    int n = 1000;
    ld h = u_max / n;
    ld res = h_sub(0, z_min, C) + h_sub(u_max, z_min, C);
    for (int i = 1; i < n; i++) {
        res += (i % 2 == 0 ? 2 : 4) * h_sub(i * h, z_min, C);
    }
    return res * h / 3.0;
}

ld simpson_S_sub(ld u_max, ld z_min, ld C) {
    if (u_max <= 0) return 0;
    int n = 1000;
    ld h = u_max / n;
    ld res = S_sub(0, z_min, C) + S_sub(u_max, z_min, C);
    for (int i = 1; i < n; i++) {
        res += (i % 2 == 0 ? 2 : 4) * S_sub(i * h, z_min, C);
    }
    return res * h / 3.0;
}

ld simpson_h_direct(ld z1, ld z2, ld C) {
    int n = 1000;
    ld h = (z2 - z1) / n;
    ld res = h_direct(z1, C) + h_direct(z2, C);
    for (int i = 1; i < n; i++) {
        res += (i % 2 == 0 ? 2 : 4) * h_direct(z1 + i * h, C);
    }
    return res * h / 3.0;
}

ld simpson_S_direct(ld z1, ld z2, ld C) {
    int n = 1000;
    ld h = (z2 - z1) / n;
    ld res = S_direct(z1, C) + S_direct(z2, C);
    for (int i = 1; i < n; i++) {
        res += (i % 2 == 0 ? 2 : 4) * S_direct(z1 + i * h, C);
    }
    return res * h / 3.0;
}

ld get_theta(ld C) {
    if (C <= 1.0) {
        return abs(simpson_h_direct(zA, zB, C));
    } else {
        ld z_min = (zA > 0 ? 1.0 : -1.0) * sqrt(C * C - 1.0);
        return simpson_h_sub(sqrt(abs(zA - z_min)), z_min, C) + 
               simpson_h_sub(sqrt(abs(zB - z_min)), z_min, C);
    }
}

ld get_S(ld C) {
    if (C <= 1.0) {
        return abs(simpson_S_direct(zA, zB, C));
    } else {
        ld z_min = (zA > 0 ? 1.0 : -1.0) * sqrt(C * C - 1.0);
        return simpson_S_sub(sqrt(abs(zA - z_min)), z_min, C) + 
               simpson_S_sub(sqrt(abs(zB - z_min)), z_min, C);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (!(cin >> xA >> yA >> zA >> xB >> yB >> zB)) return 0;

    ld thetaA = atan2(yA, xA);
    ld thetaB = atan2(yB, xB);
    delta_theta_target = abs(thetaA - thetaB);
    if (delta_theta_target > M_PI) delta_theta_target = 2.0 * M_PI - delta_theta_target;

    if (abs(zA - zB) < 1e-9 && abs(delta_theta_target) < 1e-9) {
        cout << fixed << setprecision(10) << 0.0 << endl;
        return 0;
    }
    if (abs(zA - zB) < 1e-9) {
        cout << fixed << setprecision(10) << (ld)sqrt(1.0 + zA * zA) * delta_theta_target << endl;
        return 0;
    }

    ld ans = 1e18;
    bool found = false;

    // Case 1: C in [0, 1]
    // delta_theta(C) is increasing
    ld low = 0, high = 1.0;
    for (int i = 0; i < 60; i++) {
        ld mid = (low + high) / 2.0;
        if (get_theta(mid) < delta_theta_target) low = mid;
        else high = mid;
    }
    if (abs(get_theta(low) - delta_theta_target) < 1e-4) {
        ans = min(ans, get_S(low));
        found = true;
    }

    // Case 2: C in (1, inf) (only if zA, zB same sign)
    if ((zA > 0 && zB > 0) || (zA < 0 && zB < 0)) {
        // delta_theta(C) is decreasing
        low = 1.0; high = 1e6;
        for (int i = 0; i < 60; i++) {
            ld mid = (low + high) / 2.0;
            if (get_theta(mid) > delta_theta_target) low = mid;
            else high = mid;
        }
        if (abs(get_theta(low) - delta_theta_target) < 1e-4) {
            ans = min(ans, get_S(low));
            found = true;
        }
    }

    if (!found) {
        // This shouldn't happen given the surface properties
        // Let's try to output something as a fallback
        cout << fixed << setprecision(10) << (double)ans << endl;
    } else {
        cout << fixed << setprecision(10) << (double)ans << endl;
    }

    return 0;
}

