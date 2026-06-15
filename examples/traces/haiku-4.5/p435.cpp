#include<bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    
    vector<double> x(n), y(n), r(n);
    for(int i = 0; i < n; i++){
        cin >> x[i] >> y[i] >> r[i];
    }
    
    double burned = 0, inverse = 0;
    double minx = 1e9, maxx = -1e9, miny = 1e9, maxy = -1e9;
    
    for(int i = 0; i < n; i++){
        minx = min(minx, x[i] - r[i]);
        maxx = max(maxx, x[i] + r[i]);
        miny = min(miny, y[i] - r[i]);
        maxy = max(maxy, y[i] + r[i]);
    }
    
    int samples = 10000000;
    for(int s = 0; s < samples; s++){
        double px = minx + (maxx - minx) * rand() / RAND_MAX;
        double py = miny + (maxy - miny) * rand() / RAND_MAX;
        
        int count = 0;
        for(int i = 0; i < n; i++){
            double dist = sqrt((px-x[i])*(px-x[i]) + (py-y[i])*(py-y[i]));
            if(dist < r[i]) count++;
        }
        
        if(count % 2 == 1) burned += 1;
        else if(count > 0) inverse += 1;
    }
    
    double area = (maxx - minx) * (maxy - miny);
    burned = burned * area / samples;
    inverse = inverse * area / samples;
    
    cout << fixed << setprecision(5) << burned << " " << inverse << endl;
    
    return 0;
}
