#include<bits/stdc++.h>
using namespace std;

int main(){
    int N, u, t;
    cin >> N >> u >> t;
    
    vector<int> w(N), v(N);
    double minTimeNeeded = 0;
    
    for(int i = 0; i < N; i++){
        cin >> w[i] >> v[i];
        minTimeNeeded += (double)w[i] / u;
    }
    
    if(minTimeNeeded > t + 1e-9){
        cout << -1 << endl;
        return 0;
    }
    
    // We need to distribute time optimally
    // For each river, if we spend time t_i crossing:
    // distance from river i = v_i * t_i + sqrt(u^2 - (w_i/t_i)^2) * t_i
    
    vector<double> times(N);
    double remainingTime = t - minTimeNeeded;
    
    // Start with minimum time for each river
    for(int i = 0; i < N; i++){
        times[i] = (double)w[i] / u;
    }
    
    // Distribute remaining time - greedily to maximize benefit
    // Benefit of adding dt to river i: dD/dt_i = v_i + d/dt_i[sqrt(u^2 - w_i^2/t_i^2) * t_i]
    // = v_i + sqrt(u^2 - w_i^2/t_i^2) + t_i * (-w_i^2/t_i^3) / sqrt(u^2 - w_i^2/t_i^2)
    
    // Simpler: distribute equally or use some heuristic
    double extraPerRiver = remainingTime / N;
    for(int i = 0; i < N; i++){
        times[i] += extraPerRiver;
    }
    
    double totalDistance = 0;
    for(int i = 0; i < N; i++){
        double crossSpeed = w[i] / times[i];
        double parallelSpeed = sqrt(max(0.0, (double)u*u - crossSpeed*crossSpeed));
        totalDistance += (v[i] + parallelSpeed) * times[i];
    }
    
    cout << fixed << setprecision(10) << totalDistance << endl;
    for(int i = 0; i < N; i++){
        cout << fixed << setprecision(10) << times[i];
        if(i < N-1) cout << " ";
    }
    cout << endl;
    
    return 0;
}
