#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y, z;
};

struct Star {
    string name;
    Point p;
};

Point operator-(const Point& a, const Point& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

Point crossProduct(const Point& a, const Point& b) {
    return {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

long long dotProduct(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool isZero(const Point& a) {
    return a.x == 0 && a.y == 0 && a.z == 0;
}

bool unsignedLexLess(const string& a, const string& b) {
    size_t n = min(a.size(), b.size());
    for (size_t i = 0; i < n; ++i) {
        unsigned char ca = static_cast<unsigned char>(a[i]);
        unsigned char cb = static_cast<unsigned char>(b[i]);
        if (ca != cb) return ca < cb;
    }
    return a.size() < b.size();
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    string line;
    getline(cin, line);

    vector<Star> stars(n);

    for (int i = 0; i < n; ++i) {
        getline(cin, line);

        size_t p3 = line.find_last_of(' ');
        stars[i].p.z = stoll(line.substr(p3 + 1));
        line = line.substr(0, p3);

        size_t p2 = line.find_last_of(' ');
        stars[i].p.y = stoll(line.substr(p2 + 1));
        line = line.substr(0, p2);

        size_t p1 = line.find_last_of(' ');
        stars[i].p.x = stoll(line.substr(p1 + 1));
        stars[i].name = line.substr(0, p1);
    }

    vector<bool> onHull(n, false);
    bool hasNonCollinearTriple = false;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            for (int k = j + 1; k < n; ++k) {
                Point a = stars[j].p - stars[i].p;
                Point b = stars[k].p - stars[i].p;
                Point normal = crossProduct(a, b);

                if (isZero(normal)) continue;
                hasNonCollinearTriple = true;

                bool hasPos = false, hasNeg = false;

                for (int t = 0; t < n; ++t) {
                    long long d = dotProduct(normal, stars[t].p - stars[i].p);
                    if (d > 0) hasPos = true;
                    if (d < 0) hasNeg = true;
                }

                if (!(hasPos && hasNeg)) {
                    for (int t = 0; t < n; ++t) {
                        long long d = dotProduct(normal, stars[t].p - stars[i].p);
                        if (d == 0) onHull[t] = true;
                    }
                }
            }
        }
    }

    if (!hasNonCollinearTriple) {
        fill(onHull.begin(), onHull.end(), true);
    }

    vector<string> answer;
    for (int i = 0; i < n; ++i) {
        if (onHull[i]) answer.push_back(stars[i].name);
    }

    sort(answer.begin(), answer.end(), unsignedLexLess);

    cout << answer.size() << '\n';
    for (const string& name : answer) {
        cout << name << '\n';
    }

    return 0;
}
