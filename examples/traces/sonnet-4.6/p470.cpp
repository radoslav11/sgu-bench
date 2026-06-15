#include <bits/stdc++.h>
using namespace std;

int main(){
    long long X, Y, Z;
    int cas = 1;
    while(cin >> X >> Y >> Z){
        long long edges = (X-1)*Y*Z + X*(Y-1)*Z + X*Y*(Z-1);
        
        // Count odd degree vertices
        // For each axis, positions: 2 boundary (degree 1 each) if size>1, else 0; interior (size-2) positions with degree 2
        // degree = dx+dy+dz, odd when sum is odd
        // Let's count by parity
        // px = count of positions with odd contribution in x: boundary count if X>1 else 0
        long long bx = (X>1)?2:0, ix = (X>1)?X-2:X; // boundary, interior counts
        long long by = (Y>1)?2:0, iy = (Y>1)?Y-2:Y;
        long long bz = (Z>1)?2:0, iz = (Z>1)?Z-2:Z;
        // odd parity position: odd number of odd contributions
        // odd_x means boundary in x (contribution 1, odd)
        // even_x means interior in x (contribution 2, even) or X=1 (contribution 0, even)
        long long ox=bx, ex=ix; if(X==1){ex=1;ox=0;}
        long long oy=by, ey=iy; if(Y==1){ey=1;oy=0;}
        long long oz=bz, ez=iz; if(Z==1){ez=1;oz=0;}
        
        long long odd_count = ox*oy*ez + ox*ey*oz + ex*oy*oz + ox*oy*oz;
        // odd+odd+even, odd+even+odd, even+odd+odd, odd+odd+odd
        // Wait: odd count = number with odd total = sum of products where odd number of factors are "odd"
        odd_count = ox*oy*ez + ox*ey*oz + ex*oy*oz + ox*oy*oz;
        
        long long extra = (odd_count <= 2) ? 0 : odd_count/2 - 1;
        cout << "Case #" << cas++ << ": " << edges + extra << "\n";
    }
}
