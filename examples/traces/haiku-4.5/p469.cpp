#include<bits/stdc++.h>
using namespace std;

struct Ghost {
    double x, y, z, r;
};

double getMaxRadius(double px, double py, double pz, double W, double H, double D, vector<Ghost>& ghosts) {
    double radius = min({px, W - px, py, H - py, pz, D - pz});
    
    for (auto& ghost : ghosts) {
        double dx = px - ghost.x;
        double dy = py - ghost.y;
        double dz = pz - ghost.z;
        double dist = sqrt(dx*dx + dy*dy + dz*dz);
        double maxR = dist - ghost.r;
        radius = min(radius, maxR);
    }
    
    return max(0.0, radius);
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    double W, H, D;
    cin >> W >> H >> D;
    
    int N;
    cin >> N;
    
    vector<Ghost> ghosts(N);
    for (int i = 0; i < N; i++) {
        cin >> ghosts[i].x >> ghosts[i].y >> ghosts[i].z >> ghosts[i].r;
    }
    
    double bestRadius = -1;
    double bestX = W/2, bestY = H/2, bestZ = D/2;
    
    // Try center
    double r = getMaxRadius(W/2, H/2, D/2, W, H, D, ghosts);
    if (r > bestRadius) {
        bestRadius = r;
        bestX = W/2;
        bestY = H/2;
        bestZ = D/2;
    }
    
    // Random sampling
    srand(42);
    for (int iter = 0; iter < 100000; iter++) {
        double px = (double)rand() / RAND_MAX * W;
        double py = (double)rand() / RAND_MAX * H;
        double pz = (double)rand() / RAND_MAX * D;
        
        double r = getMaxRadius(px, py, pz, W, H, D, ghosts);
        if (r > bestRadius) {
            bestRadius = r;
            bestX = px;
            bestY = py;
            bestZ = pz;
        }
    }
    
    // Try points near ghosts
    for (auto& ghost : ghosts) {
        for (double angle1 = 0; angle1 < 2*M_PI; angle1 += M_PI/6) {
            for (double angle2 = 0; angle2 < M_PI; angle2 += M_PI/6) {
                for (double dist = 0.1; dist < 100; dist += 5) {
                    double px = ghost.x + dist * sin(angle2) * cos(angle1);
                    double py = ghost.y + dist * sin(angle2) * sin(angle1);
                    double pz = ghost.z + dist * cos(angle2);
                    
                    if (px >= 0 && px <= W && py >= 0 && py <= H && pz >= 0 && pz <= D) {
                        double r = getMaxRadius(px, py, pz, W, H, D, ghosts);
                        if (r > bestRadius) {
                            bestRadius = r;
                            bestX = px;
                            bestY = py;
                            bestZ = pz;
                        }
                    }
                }
            }
        }
    }
    
    cout << fixed << setprecision(10);
    cout << bestX << " " << bestY << " " << bestZ << "\n";
    
    return 0;
}
