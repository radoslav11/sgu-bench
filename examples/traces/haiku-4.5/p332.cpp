#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    
    vector<pair<double, double>> vertices(n);
    for(int i = 0; i < n; i++){
        cin >> vertices[i].first >> vertices[i].second;
    }
    
    // Calculate area using shoelace formula
    double area = 0;
    for(int i = 0; i < n; i++){
        int j = (i + 1) % n;
        area += vertices[i].first * vertices[j].second;
        area -= vertices[j].first * vertices[i].second;
    }
    area = abs(area) / 2.0;
    
    // Calculate perimeter
    double perimeter = 0;
    for(int i = 0; i < n; i++){
        int j = (i + 1) % n;
        double dx = vertices[j].first - vertices[i].first;
        double dy = vertices[j].second - vertices[i].second;
        perimeter += sqrt(dx * dx + dy * dy);
    }
    
    // Radius = Area / semi-perimeter = 2 * Area / Perimeter
    double radius = 2.0 * area / perimeter;
    
    cout << fixed << setprecision(10) << radius << endl;
    
    return 0;
}
