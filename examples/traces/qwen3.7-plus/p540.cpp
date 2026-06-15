#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdio>

using namespace std;

struct Event {
    double pos;
    int type;
    bool operator<(const Event& o) const {
        if (abs(pos - o.pos) > 1e-9) return pos < o.pos;
        return type < o.type;
    }
};

struct Light {
    double x, r, g, d;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    double s, vmin, vmax;
    if (!(cin >> n >> s >> vmin >> vmax)) return 0;
    
    vector<Light> lights(n);
    for (int i = 0; i < n; ++i) {
        cin >> lights[i].x >> lights[i].r >> lights[i].g >> lights[i].d;
    }
    
    vector<Event> events;
    events.reserve(n * 150 * 2 + 10);
    events.push_back({vmin, 0});
    events.push_back({vmax, 0});
    
    for (int i = 0; i < n; ++i) {
        double x = lights[i].x;
        double r = lights[i].r;
        double g = lights[i].g;
        double d = lights[i].d;
        double T = r + g;
        
        for (int k = 0; ; ++k) {
            double den_A = d + r + k * T;
            double A = x / den_A;
            
            double den_B = d + k * T;
            double B = (den_B == 0) ? 1e9 : x / den_B;
            
            if (B < vmin - 1e-7) {
                break;
            }
            
            if (A > vmax + 1e-7) {
                continue;
            }
            
            events.push_back({A, 1});
            events.push_back({B, -1});
            if (A >= vmin - 1e-7 && A <= vmax + 1e-7) {
                events.push_back({A, 0});
            }
        }
    }
    
    sort(events.begin(), events.end());
    
    int best_cov = 1e9;
    double best_v = -1.0;
    int current_cov = 0;
    
    size_t i_ev = 0;
    while (i_ev < events.size()) {
        size_t j = i_ev;
        while (j < events.size() && abs(events[j].pos - events[i_ev].pos) < 1e-9) {
            j++;
        }
        
        for (size_t k = i_ev; k < j; ++k) {
            if (events[k].type == -1) {
                current_cov--;
            }
        }
        
        double pos = events[i_ev].pos;
        if (pos >= vmin - 1e-7 && pos <= vmax + 1e-7) {
            if (current_cov < best_cov) {
                best_cov = current_cov;
                best_v = pos;
            } else if (current_cov == best_cov) {
                if (best_v < 0 || pos > best_v) {
                    best_v = pos;
                }
            }
        }
        
        for (size_t k = i_ev; k < j; ++k) {
            if (events[k].type == 1) {
                current_cov++;
            }
        }
        
        i_ev = j;
    }
    
    vector<int> red_lights;
    for (int i = 0; i < n; ++i) {
        double x = lights[i].x;
        double r = lights[i].r;
        double g = lights[i].g;
        double d = lights[i].d;
        double T = r + g;
        
        bool red = false;
        for (int k = 0; ; ++k) {
            double den_A = d + r + k * T;
            double A = x / den_A;
            double den_B = d + k * T;
            double B = (den_B == 0) ? 1e9 : x / den_B;
            
            if (B < vmin - 1e-7) break;
            if (A > vmax + 1e-7) continue;
            
            if (best_v > A + 1e-9 && best_v < B - 1e-9) {
                red = true;
                break;
            }
        }
        if (red) {
            red_lights.push_back(i + 1);
        }
    }
    
    printf("%.10f\n", best_v);
    printf("%zu\n", red_lights.size());
    if (red_lights.empty()) {
        printf("\n");
    } else {
        for (size_t i = 0; i < red_lights.size(); ++i) {
            printf("%d%c", red_lights[i], (i + 1 == red_lights.size() ? '\n' : ' '));
        }
    }
    
    return 0;
}
