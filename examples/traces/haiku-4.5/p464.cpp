#include <bits/stdc++.h>
using namespace std;

int N;
double F1, F2, V;
map<pair<int,int>, pair<double,double>> memo;

pair<double,double> solve(int a1, int a2);

double getProb(double F, double b) {
    return 1.0 - pow(0.99, (1.0 - F) * b);
}

pair<double,double> solve(int a1, int a2) {
    if (a1 == N) return {V, 0.0};
    if (a2 == N) return {0.0, V};
    
    auto key = make_pair(a1, a2);
    if (memo.count(key)) return memo[key];
    
    double bestDiff = -1e18;
    double optB1 = 0, optB2 = 0;
    
    for (double b1 = 0; b1 <= 200; b1 += 0.1) {
        for (double b2 = 0; b2 <= 200; b2 += 0.1) {
            double p1 = getProb(F1, b1);
            double p2 = getProb(F2, b2);
            
            auto [v1_11, v2_11] = solve(a1 + 1, a2 + 1);
            auto [v1_10, v2_10] = solve(a1 + 1, a2);
            auto [v1_01, v2_01] = solve(a1, a2 + 1);
            auto [v1_00, v2_00] = solve(a1, a2);
            
            double ev1 = p1*p2*0.5*v1_11 + p1*(1-p2)*v1_10 + (1-p1)*p2*v1_01 + (1-p1)*(1-p2)*v1_00 - b1;
            double ev2 = p1*p2*0.5*v2_11 + p1*(1-p2)*v2_10 + (1-p1)*p2*v2_01 + (1-p1)*(1-p2)*v2_00 - b2;
            
            if (ev1 - ev2 > bestDiff) {
                bestDiff = ev1 - ev2;
                optB1 = b1;
                optB2 = b2;
            }
        }
    }
    
    double p1 = getProb(F1, optB1);
    double p2 = getProb(F2, optB2);
    
    auto [v1_11, v2_11] = solve(a1 + 1, a2 + 1);
    auto [v1_10, v2_10] = solve(a1 + 1, a2);
    auto [v1_01, v2_01] = solve(a1, a2 + 1);
    auto [v1_00, v2_00] = solve(a1, a2);
    
    double ev1 = p1*p2*0.5*v1_11 + p1*(1-p2)*v1_10 + (1-p1)*p2*v1_01 + (1-p1)*(1-p2)*v1_00 - optB1;
    double ev2 = p1*p2*0.5*v2_11 + p1*(1-p2)*v2_10 + (1-p1)*p2*v2_01 + (1-p1)*(1-p2)*v2_00 - optB2;
    
    return memo[key] = {ev1, ev2};
}

int main(){
    scanf("%d %lf %lf %lf", &N, &F1, &F2, &V);
    
    auto [p1, p2] = solve(0, 0);
    double total = p1 + p2;
    printf("%.9f %.9f\n", p1/total, p2/total);
    
    return 0;
}
