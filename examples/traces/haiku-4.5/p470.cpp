#include <bits/stdc++.h>
using namespace std;

int main(){
    int X, Y, Z;
    int caseNum = 1;
    
    while(cin >> X >> Y >> Z){
        // Count edges in each direction
        long long edgesX = (long long)(X - 1) * Y * Z;
        long long edgesY = (long long)X * (Y - 1) * Z;
        long long edgesZ = (long long)X * Y * (Z - 1);
        long long totalEdges = edgesX + edgesY + edgesZ;
        
        // Count odd degree vertices (corners)
        // A corner vertex has degree = number of adjacent vertices
        // For a rectangular box, corners have degree 3
        long long oddVertices = 8;
        
        // For Eulerian path, we need to duplicate (oddVertices - 2) / 2 edges
        long long extraEdges = (oddVertices - 2) / 2;
        
        long long pathLength = totalEdges + extraEdges;
        
        cout << "Case #" << caseNum << ": " << pathLength << "\n";
        caseNum++;
    }
    
    return 0;
}
