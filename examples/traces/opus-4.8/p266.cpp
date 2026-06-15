#include <bits/stdc++.h>
using namespace std;

double clampd(double x){
    if(x>1.0) return 1.0;
    if(x<-1.0) return -1.0;
    return x;
}

int main(){
    double R;
    cin >> R;
    double x1,y1,z1,x2,y2,z2;
    cin >> x1 >> y1 >> z1;
    cin >> x2 >> y2 >> z2;
    
    double pi = acos(-1.0);
    
    double d1 = sqrt(x1*x1+y1*y1+z1*z1);
    double d2 = sqrt(x2*x2+y2*y2+z2*z2);
    
    double cosA1 = R/d1; // cos of angular radius
    double cosA2 = R/d2;
    double a1 = acos(clampd(cosA1));
    double a2 = acos(clampd(cosA2));
    double sinA1 = sin(a1);
    double sinA2 = sin(a2);
    
    // areas of caps
    double A1 = 2*pi*R*R*(1-cosA1);
    double A2 = 2*pi*R*R*(1-cosA2);
    
    // angle between cap centers
    double dot = (x1*x2+y1*y2+z1*z2)/(d1*d2);
    double delta = acos(clampd(dot));
    double cosD = cos(delta);
    double sinD = sin(delta);
    
    double unionArea;
    
    if(delta >= a1 + a2 - 1e-15){
        // no overlap
        unionArea = A1 + A2;
    } else if(delta <= fabs(a1 - a2) + 1e-15){
        // one inside the other
        unionArea = max(A1, A2);
    } else {
        // partial overlap, compute lens
        double cosPhiA = (cosA2 - cosA1*cosD)/(sinA1*sinD);
        double cosPhiB = (cosA1 - cosA2*cosD)/(sinA2*sinD);
        double cosChi  = (cosD - cosA1*cosA2)/(sinA1*sinA2);
        
        double phiA = acos(clampd(cosPhiA));
        double phiB = acos(clampd(cosPhiB));
        double chi  = acos(clampd(cosChi));
        
        double lensUnit = 2*(pi - chi) - 2*phiA*cosA1 - 2*phiB*cosA2;
        double lens = lensUnit * R*R;
        
        unionArea = A1 + A2 - lens;
    }
    
    cout << fixed << setprecision(3) << unionArea << "\n";
    return 0;
}
