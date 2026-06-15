#include<bits/stdc++.h>
using namespace std;

int main(){
    int n, x0, y0;
    cin >> n >> x0 >> y0;
    
    vector<pair<int,int>> w(n);
    int minX = x0, maxX = x0, minY = y0, maxY = y0;
    
    for(int i = 0; i < n; i++){
        cin >> w[i].first >> w[i].second;
        minX = min(minX, w[i].first);
        maxX = max(maxX, w[i].first);
        minY = min(minY, w[i].second);
        maxY = max(maxY, w[i].second);
    }
    
    // Create a rectangle path
    long long width = maxX - minX;
    long long height = maxY - minY;
    long long area = width * height;
    
    string path;
    // Go around the bounding box
    for(int i = 0; i < width; i++) path += 'E';
    for(int i = 0; i < height; i++) path += 'N';
    for(int i = 0; i < width; i++) path += 'W';
    for(int i = 0; i < height; i++) path += 'S';
    
    cout << area << "\n" << path << "\n";
    
    return 0;
}
