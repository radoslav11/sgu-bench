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

const double EPS = 1e-9;
const double PI = acos(-1.0L);

int n;
vector<pair<double, double>> stars1, stars2;

void read() {
    cin >> n;
    stars1.resize(n);
    stars2.resize(n);
    cin >> stars1;
    cin >> stars2;
}

double normalize_angle(double angle) {
    if(angle < 0) {
        angle += 2 * PI;
    }

    angle = fmod(angle, 2 * PI);
    return angle;
}

pair<double, double> rotate_vector(pair<double, double> v, double angle) {
    double cos_a = cos(angle);
    double sin_a = sin(angle);
    return {v.first * cos_a - v.second * sin_a,
            v.first * sin_a + v.second * cos_a};
}

vector<pair<double, double>> get_all_vectors(
    const vector<pair<double, double>>& stars
) {
    int m = stars.size();
    double cx = 0, cy = 0;
    for(auto& s: stars) {
        cx += s.first;
        cy += s.second;
    }

    cx /= m;
    cy /= m;

    vector<tuple<double, double, pair<double, double>>> star_data;
    for(int i = 0; i < m; i++) {
        double dx = stars[i].first - cx;
        double dy = stars[i].second - cy;

        double angle;
        if(fabs(dx) < EPS && fabs(dy) < EPS) {
            angle = numeric_limits<double>::infinity();
        } else {
            angle = normalize_angle(atan2(dy, dx));
        }

        double distance = sqrt(dx * dx + dy * dy);
        star_data.push_back({angle, distance, {dx, dy}});
    }

    sort(star_data.begin(), star_data.end(), [](const auto& a, const auto& b) {
        if(get<0>(a) != get<0>(b)) {
            return get<0>(a) < get<0>(b);
        }
        return get<1>(a) < get<1>(b);
    });

    vector<pair<double, double>> vectors;
    for(auto& item: star_data) {
        vectors.push_back(get<2>(item));
    }

    return vectors;
}

bool vectors_match(
    const vector<pair<double, double>>& vectors1,
    const vector<pair<double, double>>& vectors2, double rotation_angle,
    int offset
) {
    int m = vectors1.size();
    vector<pair<double, double>> rotated(m);
    for(int i = 0; i < m; i++) {
        rotated[i] = rotate_vector(vectors2[i], rotation_angle);
    }

    for(int i = 0; i < m; i++) {
        auto v1 = vectors1[i];
        auto v2 = rotated[(i + offset) % m];
        if(fabs(v1.first - v2.first) > EPS ||
           fabs(v1.second - v2.second) > EPS) {
            return false;
        }
    }

    return true;
}

void solve() {
    // Each photo is a set of stars whose centroid is translation invariant, so
    // we represent each star by its vector from the centroid. Within a photo we
    // sort these vectors by (polar angle, length), giving a canonical cyclic
    // ordering that is the same up to rotation.
    //
    // A rotation maps vectors1 onto vectors2 with some cyclic offset. We anchor
    // on vectors1[0] and try matching it against each vectors2[i] of equal
    // length, which fixes a candidate rotation angle = angle(v1) - angle(v2).
    // For each candidate we verify the whole set matches under that rotation
    // and cyclic offset (checking both the angle and its 2*pi complement), then
    // keep the smallest valid angle. The answer lies in [0, pi], so for each
    // accepted rotation we also consider 2*pi - angle.

    if(n == 1) {
        cout << fixed << setprecision(11) << 0.0 << '\n';
        return;
    }

    auto vectors1 = get_all_vectors(stars1);
    auto vectors2 = get_all_vectors(stars2);

    double min_angle = numeric_limits<double>::infinity();

    for(int i = 0; i < n; i++) {
        auto v1 = vectors1[0];
        auto v2 = vectors2[i];

        double v1_len = sqrt(v1.first * v1.first + v1.second * v1.second);
        double v2_len = sqrt(v2.first * v2.first + v2.second * v2.second);

        if(v1_len < EPS || v2_len < EPS) {
            continue;
        }

        if(fabs(v1_len - v2_len) > EPS) {
            continue;
        }

        double v1_angle = atan2(v1.second, v1.first);
        double v2_angle = atan2(v2.second, v2.first);
        double rotation_angle = normalize_angle(v1_angle - v2_angle);

        if(vectors_match(vectors1, vectors2, rotation_angle, i) ||
           vectors_match(vectors1, vectors2, 2 * PI - rotation_angle, i)) {
            min_angle = min(min_angle, rotation_angle);
            min_angle = min(min_angle, 2 * PI - rotation_angle);
        }
    }

    cout << fixed << setprecision(11) << min_angle << '\n';
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
