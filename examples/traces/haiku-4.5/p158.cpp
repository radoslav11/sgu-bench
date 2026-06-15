#include <bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int L, M;
    cin >> L >> M;
    
    vector<int> P(M);
    for(int i = 0; i < M; i++){
        cin >> P[i];
    }
    
    int N;
    cin >> N;
    
    vector<int> D(N);
    D[0] = 0;
    for(int i = 1; i < N; i++){
        cin >> D[i];
    }
    
    auto evaluate = [&](double S) -> double {
        double sum = 0;
        for(int i = 0; i < M; i++){
            double minDist = 1e9;
            for(int j = 0; j < N; j++){
                double dist = abs(D[j] + S - P[i]);
                minDist = min(minDist, dist);
            }
            sum += minDist;
        }
        return sum;
    };
    
    set<double> candidates;
    
    // Boundary positions
    candidates.insert(0);
    candidates.insert((double)(L - D[N-1]));
    
    // Critical positions where passenger aligns with a door
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N; j++){
            double S = P[i] - D[j];
            if(S >= 0 && S <= L - D[N-1]){
                candidates.insert(S);
            }
        }
    }
    
    // Positions where passenger is equidistant from two consecutive doors
    for(int i = 0; i < M; i++){
        for(int j = 0; j < N - 1; j++){
            // |D[j] + S - P[i]| = |D[j+1] + S - P[i]|
            // If both on same side: equal anyway
            // If on opposite sides: D[j] + S - P[i] = -(D[j+1] + S - P[i])
            // D[j] + S - P[i] = -D[j+1] - S + P[i]
            // 2S = P[i] - D[j] + P[i] - D[j+1]
            double S = (P[i] - D[j] + P[i] - D[j+1]) / 2.0;
            if(S >= 0 && S <= L - D[N-1]){
                candidates.insert(S);
            }
        }
    }
    
    double bestS = 0;
    double bestValue = evaluate(0);
    
    for(double S : candidates){
        double value = evaluate(S);
        if(value > bestValue + 1e-9){
            bestValue = value;
            bestS = S;
        }
    }
    
    cout << fixed << setprecision(1) << bestS << " " << bestValue << endl;
    
    return 0;
}
