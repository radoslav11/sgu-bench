#include <bits/stdc++.h>
using namespace std;

int main(){
    int N, N1, N2;
    cin >> N >> N1 >> N2;
    double x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    
    double alpha = (N2 - N1) * 2.0 * M_PI / N;
    double c = cos(alpha), s = sin(alpha);
    
    double dx = x2 - x1, dy = y2 - y1;
    double det = 2.0 - 2.0*c;
    
    double u, v;
    if(fabs(det) < 1e-12){
        // Shouldn't happen for valid input
        u = 0; v = 0;
    } else {
        u = ((c-1)*dx - s*dy) / det;
        v = (s*dx + (c-1)*dy) / det;
    }
    
    double cx = x1 - u;
    double cy = y1 - v;
    
    // Vertex N1 relative to center: (u, v)
    // Rotate back to vertex 1: counter-clockwise by (N1-1)*2pi/N
    double beta = (N1 - 1) * 2.0 * M_PI / N;
    double cb = cos(beta), sb = sin(beta);
    // counter-clockwise rotation
    double r1x = cb*u - sb*v;
    double r1y = sb*u + cb*v;
    
    cout << fixed << setprecision(6);
    for(int k = 1; k <= N; k++){
        double ang = (k-1) * 2.0 * M_PI / N;
        double ck = cos(ang), sk = sin(ang);
        // clockwise rotation of (r1x, r1y)
        double rx = ck*r1x + sk*r1y;
        double ry = -sk*r1x + ck*r1y;
        cout << cx+rx << " " << cy+ry << "\n";
    }
    return 0;
}
