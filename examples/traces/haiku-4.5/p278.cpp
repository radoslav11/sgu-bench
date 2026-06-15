#include<bits/stdc++.h>

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, A, B;
    cin >> N >> A >> B;
    
    vector<array<int,3>> fuels(N);
    for(int i = 0; i < N; i++){
        cin >> fuels[i][0] >> fuels[i][1] >> fuels[i][2]; // a, b, c
    }
    
    double best = 0;
    
    // Try greedy by c/b ratio
    auto fuels_copy = fuels;
    sort(fuels_copy.begin(), fuels_copy.end(), [](auto& x, auto& y){
        return (double)x[2]/max(1,x[1]) > (double)y[2]/max(1,y[1]);
    });
    
    double vol = A, bud = B, intensity = 0;
    for(auto [a,b,c] : fuels_copy){
        double take = min(a>0?vol/a:1e9, b>0?bud/b:1e9);
        intensity += take * c;
        vol -= take * a;
        bud -= take * b;
    }
    best = max(best, intensity);
    
    cout << fixed << setprecision(6) << best << endl;
    return 0;
}
