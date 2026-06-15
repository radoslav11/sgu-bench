#include <bits/stdc++.h>
using namespace std;

const double PI = acos(-1.0);
const double PERIOD = 2.0 * PI;
const double EPS = 1e-10;
const double NEG = -1e100;

int N;
double Ddist, cst;
double a_[6], b_[6], del_[6];

struct StartEvent {
    double t;
    int x, y;
};

struct ArrEvent {
    double t, val;
    int y, parent;
    bool operator<(const ArrEvent& other) const {
        return t > other.t;
    }
};

struct Node {
    int parent;
    int lane;
    double start;
};

double S(int i, double t) {
    return b_[i] * t - a_[i] * cos(t + del_[i]) + a_[i] * cos(del_[i]);
}

double integral(int i, double l, double r) {
    return S(i, r) - S(i, l);
}

double speed(int i, double t) {
    return b_[i] + a_[i] * sin(t + del_[i]);
}

double norm_phase(double x) {
    x = fmod(x, PERIOD);
    if (x < 0) x += PERIOD;
    return x;
}

vector<double> edge_phases[6][6];

void prepare_phases() {
    for (int x = 1; x <= N; ++x) {
        for (int y = 1; y <= N; ++y) if (x != y) {
            double tau = cst * abs(x - y);

            vector<double> ph;
            ph.push_back(0.0);

            // Solve speed_x(t) = speed_y(t + tau)
            // b_x + a_x sin(t+d_x) = b_y + a_y sin(t+tau+d_y)
            double A = a_[x] * cos(del_[x]) - a_[y] * cos(tau + del_[y]);
            double B = a_[x] * sin(del_[x]) - a_[y] * sin(tau + del_[y]);
            double C = b_[x] - b_[y];

            double R = hypot(A, B);
            if (R > 1e-14) {
                double v = -C / R;
                if (v >= -1.0 - 1e-12 && v <= 1.0 + 1e-12) {
                    v = max(-1.0, min(1.0, v));
                    double theta = atan2(B, A);
                    double u1 = asin(v);
                    double u2 = PI - u1;
                    ph.push_back(norm_phase(u1 - theta));
                    ph.push_back(norm_phase(u2 - theta));
                }
            } else {
                if (fabs(C) < 1e-14) ph.push_back(0.0);
            }

            sort(ph.begin(), ph.end());
            vector<double> res;
            for (double p : ph) {
                if (res.empty() || fabs(p - res.back()) > 1e-9)
                    res.push_back(p);
            }
            edge_phases[x][y] = res;
        }
    }
}

vector<StartEvent> build_starts(double T) {
    vector<StartEvent> ev;
    for (int x = 1; x <= N; ++x) {
        for (int y = 1; y <= N; ++y) if (x != y) {
            for (double ph : edge_phases[x][y]) {
                for (double t = ph; t <= T + EPS; t += PERIOD) {
                    if (t < -EPS) continue;
                    if (t <= T + EPS)
                        ev.push_back({max(0.0, t), x, y});
                }
            }
        }
    }
    sort(ev.begin(), ev.end(), [](const StartEvent& p, const StartEvent& q) {
        if (fabs(p.t - q.t) > 1e-12) return p.t < q.t;
        if (p.x != q.x) return p.x < q.x;
        return p.y < q.y;
    });
    return ev;
}

bool run_dp(double T, bool rec, vector<Node>& nodes, int& bestNode, int& bestLane) {
    vector<StartEvent> starts = build_starts(T);

    vector<double> dp(N + 1, NEG);
    vector<int> node(N + 1, -1);

    if (rec) {
        nodes.clear();
        nodes.push_back({-1, 1, 0.0});
        node[1] = 0;
    }
    dp[1] = 0.0;

    priority_queue<ArrEvent> pq;

    double cur = 0.0;
    size_t ptr = 0;

    auto advance = [&](double nt) {
        if (nt <= cur + 1e-14) {
            cur = nt;
            return;
        }
        for (int i = 1; i <= N; ++i) {
            if (dp[i] > NEG / 2)
                dp[i] += integral(i, cur, nt);
        }
        cur = nt;
    };

    while (ptr < starts.size() || !pq.empty()) {
        double nt = T;
        if (ptr < starts.size()) nt = min(nt, starts[ptr].t);
        if (!pq.empty()) nt = min(nt, pq.top().t);
        if (nt > T + EPS) break;

        advance(nt);

        while (!pq.empty() && fabs(pq.top().t - cur) < 1e-9) {
            auto e = pq.top(); pq.pop();
            if (e.val > dp[e.y] + 1e-12) {
                dp[e.y] = e.val;
                if (rec) node[e.y] = e.parent;
            }
        }

        while (ptr < starts.size() && fabs(starts[ptr].t - cur) < 1e-9) {
            int x = starts[ptr].x, y = starts[ptr].y;
            double tau = cst * abs(x - y);
            if (cur + tau <= T + 1e-9 && dp[x] > NEG / 2) {
                int nd = -1;
                if (rec) {
                    nodes.push_back({node[x], y, cur});
                    nd = (int)nodes.size() - 1;
                }
                pq.push({cur + tau, dp[x], y, nd});
            }
            ++ptr;
        }

        if (nt >= T - EPS) break;
    }

    advance(T);

    double best = NEG;
    bestLane = 1;
    for (int i = 1; i <= N; ++i) {
        if (dp[i] > best) {
            best = dp[i];
            bestLane = i;
        }
    }
    bestNode = rec ? node[bestLane] : -1;
    return best + 1e-8 >= Ddist;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> Ddist >> cst;
    for (int i = 1; i <= N; ++i) {
        cin >> a_[i] >> b_[i] >> del_[i];
    }

    prepare_phases();

    double lo = 0.0, hi = 1.0;
    vector<Node> dummy;
    int bn = -1, bl = 1;
    while (!run_dp(hi, false, dummy, bn, bl)) hi *= 2.0;

    for (int it = 0; it < 80; ++it) {
        double mid = (lo + hi) * 0.5;
        if (run_dp(mid, false, dummy, bn, bl)) hi = mid;
        else lo = mid;
    }

    vector<Node> nodes;
    run_dp(hi, true, nodes, bn, bl);

    vector<pair<int, double>> ans;
    int v = bn;
    while (v > 0) {
        ans.push_back({nodes[v].lane, nodes[v].start});
        v = nodes[v].parent;
    }
    reverse(ans.begin(), ans.end());

    cout.setf(ios::fixed);
    cout << setprecision(15) << hi << "\n";
    cout << ans.size() << "\n";
    for (auto &p : ans) {
        cout << p.first << " " << setprecision(15) << p.second << "\n";
    }

    return 0;
}
