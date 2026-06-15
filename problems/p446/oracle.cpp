#include <bits/stdc++.h>
using namespace std;

// Epsilon for floating-point comparisons
const double EPS = 1e-9;
const double PI  = acos(-1.0);

// Normalize an angle to the range [0, 2*PI)
double normalize_angle(double a) {
    // fmod might return negative
    a = fmod(a, 2*PI);
    if (a < 0) a += 2*PI;
    return a;
}

// Rotate a complex number (vector) by angle 'ang'
complex<double> rotate_vec(const complex<double>& v, double ang) {
    // exp(i*ang) = cos(angle) + i*sin(angle)
    return v * polar(1.0, ang);
}

// Attempt to match A against B after rotating B by 'rot'
// with a cyclic shift of 'offset'. Returns true if all match.
bool match_with_rotation(
    const vector<complex<double>>& A,
    const vector<complex<double>>& B,
    double rot,
    int offset
) {
    int n = A.size();
    for (int j = 0; j < n; ++j) {
        // rotate B[ (offset + j) % n ]
        complex<double> bv = rotate_vec(B[(offset+j) % n], rot);
        // Compare real and imag parts to A[j]
        if (fabs(bv.real() - A[j].real()) > EPS ||
            fabs(bv.imag() - A[j].imag()) > EPS) {
            return false;
        }
    }
    return true;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<complex<double>> P1(n), P2(n);

    // Read first constellation
    for(int i = 0; i < n; i++){
        double x, y;
        cin >> x >> y;
        P1[i] = complex<double>(x, y);
    }
    // Read second constellation
    for(int i = 0; i < n; i++){
        double x, y;
        cin >> x >> y;
        P2[i] = complex<double>(x, y);
    }

    // If there's only one star, rotation is 0
    if(n == 1){
        cout << fixed << setprecision(11) << 0.0 << "\n";
        return 0;
    }

    // Compute centroids
    complex<double> c1(0,0), c2(0,0);
    for(int i = 0; i < n; i++){
        c1 += P1[i];
        c2 += P2[i];
    }
    c1 /= double(n);
    c2 /= double(n);

    // Build (angle, distance, vector) tuples for each point set
    auto build_data = [&](const vector<complex<double>>& P, complex<double> cent){
        vector< tuple<double,double,complex<double>> > data;
        for(auto& pt : P) {
            complex<double> v = pt - cent; 
            double dist = abs(v);
            double ang  = (dist < EPS) ? 0.0 : normalize_angle(arg(v));
            data.emplace_back(ang, dist, v);
        }
        // Sort by (angle asc, distance asc)
        sort(data.begin(), data.end(),
             [](auto &A, auto &B){
                 if (get<0>(A) != get<0>(B))
                     return get<0>(A) < get<0>(B);
                 return get<1>(A) < get<1>(B);
             });
        return data;
    };

    auto D1 = build_data(P1, c1);
    auto D2 = build_data(P2, c2);

    // Extract only the sorted vectors
    vector<complex<double>> A(n), B(n);
    for(int i = 0; i < n; i++){
        A[i] = get<2>(D1[i]);
        B[i] = get<2>(D2[i]);
    }

    double best = 2*PI;  // we will minimize over [0, 2π)

    // Try aligning A[0] with each B[i]
    for(int i = 0; i < n; i++){
        double lenA = abs(A[0]);
        double lenB = abs(B[i]);
        // lengths must match
        if (fabs(lenA - lenB) > EPS) continue;
        // compute candidate rotation Δ = angle(A[0]) - angle(B[i])
        double angA = normalize_angle(arg(A[0]));
        double angB = normalize_angle(arg(B[i]));
        double delta = normalize_angle(angA - angB);

        // Test both Δ and its complement 2π-Δ
        for(int t = 0; t < 2; ++t) {
            double rot = (t == 0 ? delta : normalize_angle(2*PI - delta));
            // As we sorted, B[i] was at index i; after rotation we shift by -i
            if (match_with_rotation(A, B, rot, i)) {
                double cand = min(rot, 2*PI - rot);
                if (cand < best) best = cand;
            }
        }
    }

    // Output result in [0, π]
    if (best > PI) best = 2*PI - best;
    cout << fixed << setprecision(11) << best << "\n";
    return 0;
}
