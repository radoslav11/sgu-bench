#include<bits/stdc++.h>
using namespace std;

int N;
vector<int> R;
vector<pair<double, double>> points;

double triangleArea(pair<double,double> a, pair<double,double> b, pair<double,double> c) {
    return abs(a.first*(b.second-c.second) + b.first*(c.second-a.second) + c.first*(a.second-b.second))/2.0;
}

bool checkConstraints() {
    int n = points.size();
    for(int i = 0; i < n; i++) {
        for(int j = i+1; j < n; j++) {
            for(int k = j+1; k < n; k++) {
                double area = triangleArea(points[i], points[j], points[k]);
                double sum = R[i] + R[j] + R[k];
                if(abs(area - sum) > 1e-6) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool solve(int idx) {
    if(idx == N) {
        return checkConstraints();
    }
    
    if(idx < 3) {
        // Place first 3 points
        if(idx == 0) {
            points.push_back({0, 0});
            return solve(1);
        } else if(idx == 1) {
            points.push_back({1, 0});
            return solve(2);
        } else {
            // Try different y values for third point
            for(double y = 0; y <= 10; y += 0.5) {
                points.push_back({0, y});
                if(checkConstraints() && solve(3)) {
                    return true;
                }
                points.pop_back();
            }
            return false;
        }
    } else {
        // For points after the first 3, search for valid positions
        for(double x = -5; x <= 5; x += 0.5) {
            for(double y = -5; y <= 5; y += 0.5) {
                points.push_back({x, y});
                bool valid = true;
                // Check if all triples including this point satisfy constraint
                for(int i = 0; i < idx && valid; i++) {
                    for(int j = i+1; j < idx && valid; j++) {
                        double area = triangleArea(points[i], points[j], points[idx]);
                        double sum = R[i] + R[j] + R[idx];
                        if(abs(area - sum) > 1e-6) {
                            valid = false;
                        }
                    }
                }
                if(valid && solve(idx+1)) {
                    return true;
                }
                points.pop_back();
            }
        }
        return false;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> N;
    R.resize(N);
    for(int i = 0; i < N; i++) {
        cin >> R[i];
    }
    
    if(solve(0)) {
        cout << "YES\n";
        cout << fixed << setprecision(4);
        for(auto p : points) {
            cout << p.first << " " << p.second << "\n";
        }
    } else {
        cout << "NO\n";
    }
    
    return 0;
}
