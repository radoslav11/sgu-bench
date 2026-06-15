#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int n;
    cin >> n;
    cin.ignore();
    
    vector<pair<string, int>> stars;
    for(int i = 0; i < n; i++){
        string line;
        getline(cin, line);
        istringstream iss(line);
        string name;
        double x, y, z;
        iss >> name >> x >> y >> z;
        stars.push_back({name, i});
    }
    
    sort(stars.begin(), stars.end());
    
    cout << n << "\n";
    for(auto& s : stars) {
        cout << s.first << "\n";
    }
    
    return 0;
}
