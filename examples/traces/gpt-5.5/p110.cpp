#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double INF = 1e100;

struct Vec {
    double x, y, z;

    Vec() : x(0), y(0), z(0) {}
    Vec(double x, double y, double z) : x(x), y(y), z(z) {}

    Vec operator + (const Vec& other) const {
        return Vec(x + other.x, y + other.y, z + other.z);
    }

    Vec operator - (const Vec& other) const {
        return Vec(x - other.x, y - other.y, z - other.z);
    }

    Vec operator * (double k) const {
        return Vec(x * k, y * k, z * k);
    }

    Vec operator / (double k) const {
        return Vec(x / k, y / k, z / k);
    }
};

double dot(const Vec& a, const Vec& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double norm(const Vec& a) {
    return sqrt(dot(a, a));
}

struct Sphere {
    Vec c;
    double r;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<Sphere> s(n);
    for (int i = 0; i < n; ++i) {
        cin >> s[i].c.x >> s[i].c.y >> s[i].c.z >> s[i].r;
    }

    Vec p, q;
    cin >> p.x >> p.y >> p.z >> q.x >> q.y >> q.z;

    Vec dir = q - p;
    dir = dir / norm(dir);

    vector<int> ans;

    for (int step = 0; step < 11; ++step) {
        double bestT = INF;
        int best = -1;

        for (int i = 0; i < n; ++i) {
            Vec oc = p - s[i].c;

            double b = dot(dir, oc);
            double c = dot(oc, oc) - s[i].r * s[i].r;
            double disc = b * b - c;

            if (disc < -EPS) continue;

            if (disc < 0) disc = 0;

            double root = sqrt(disc);
            double t1 = -b - root;
            double t2 = -b + root;

            double t = INF;
            if (t1 > 1e-7) t = t1;
            else if (t2 > 1e-7) t = t2;

            if (t < bestT) {
                bestT = t;
                best = i;
            }
        }

        if (best == -1) break;

        ans.push_back(best + 1);

        Vec hit = p + dir * bestT;
        Vec normal = (hit - s[best].c) / s[best].r;

        dir = dir - normal * (2.0 * dot(dir, normal));
        dir = dir / norm(dir);

        p = hit;
    }

    if (ans.size() > 10) {
        for (int i = 0; i < 10; ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
        cout << " etc.";
    } else {
        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i];
        }
    }

    cout << '\n';

    return 0;
}
