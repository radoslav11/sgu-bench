#include <bits/stdc++.h>

using namespace std;

template<typename T1, typename T2>
ostream& operator<<(ostream& out, const pair<T1, T2>& x) {
    return out << x.first << ' ' << x.second;
}

template<typename T1, typename T2>
istream& operator>>(istream& in, pair<T1, T2>& x) {
    return in >> x.first >> x.second;
}

template<typename T>
istream& operator>>(istream& in, vector<T>& a) {
    for(auto& x: a) {
        in >> x;
    }
    return in;
};

template<typename T>
ostream& operator<<(ostream& out, const vector<T>& a) {
    for(auto x: a) {
        out << x << ' ';
    }
    return out;
};

using Point = complex<long double>;

int n, a, b;
vector<Point> points;

void read() {
    cin >> n >> a >> b;
    points.assign(n + 1, Point());
    long double x, y;
    cin >> x >> y;
    points[a] = Point(x, y);
    cin >> x >> y;
    points[b] = Point(x, y);
}

void solve() {
    // The islands are vertices of a regular N-gon listed clockwise, and we know
    // two of them (indices a, b). The chord between vertices a and b subtends a
    // central angle of 2*pi*(b-a)/n, so the circumradius is dist / 2 /
    // sin(pi*(b-a)/n) and the circle centre lies on the perpendicular bisector
    // of that chord (offset by the half-chord over tan of the half-angle).
    // Once the centre and the angular position phi of vertex a are known, every
    // other vertex i sits at angle phi + 2*pi*(a-i)/n on the circle.

    if(a > b) {
        swap(a, b);
    }

    const long double PI = 3.14159265358979323846L;
    long double dist = abs(points[b] - points[a]);
    long double radius = dist / sin(PI * (b - a) / n) / 2;

    Point mid = (points[a] + points[b]) / 2.0L;
    Point center =
        mid +
        Point(
            (points[b].imag() - points[a].imag()) / tan(PI * (b - a) / n) / 2,
            -(points[b].real() - points[a].real()) / tan(PI * (b - a) / n) / 2
        );

    long double phi = asin((points[a].imag() - center.imag()) / radius);
    if(acos((points[a].real() - center.real()) / radius) > PI / 2) {
        phi = (phi >= 0 ? PI - phi : -PI - phi);
    }

    for(int i = 1; i <= n; i++) {
        if(i != a && i != b) {
            long double delta = phi + 2 * PI * (a - i) / n;
            points[i] =
                center + Point(radius * cos(delta), radius * sin(delta));
        }
    }

    cout << fixed << setprecision(6);
    for(int i = 1; i <= n; i++) {
        cout << points[i].real() << " " << points[i].imag() << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;
    for(int test = 1; test <= T; test++) {
        read();
        // cout << "Case #" << test << ": ";
        solve();
    }

    return 0;
}
