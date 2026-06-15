#include<bits/stdc++.h>
using namespace std;

int n;
vector<int> coins;
int weighing_count = 0;

int calculate_weighings(int cnt) {
    if (cnt <= 1) return 0;
    if (cnt <= 3) return 1;
    int w = 0;
    int temp = cnt;
    while (temp > 1) {
        temp = (temp + 2) / 3;  // ceil(temp/3)
        w++;
    }
    return w;
}

void solve(vector<int> group, int indent) {
    string ind(indent, ' ');
    
    if (group.size() == 1) {
        cout << ind << "fake " << group[0] << "\n";
        return;
    }
    
    int third = (group.size() + 2) / 3;
    
    vector<int> g1(group.begin(), group.begin() + third);
    vector<int> g2(group.begin() + third, group.begin() + 2*third);
    vector<int> g3(group.begin() + 2*third, group.end());
    
    // Output weighing
    cout << ind << "weigh ";
    for (int i = 0; i < g1.size(); i++) {
        if (i > 0) cout << "+";
        cout << g1[i];
    }
    cout << " vs ";
    for (int i = 0; i < g2.size(); i++) {
        if (i > 0) cout << "+";
        cout << g2[i];
    }
    cout << "\n";
    
    // Case: g1 < g2 (fake in g1 or g2 is heavier)
    cout << ind << "case <:\n";
    solve(g1, indent + 2);
    cout << ind << "end\n";
    
    // Case: g1 = g2 (fake in g3)
    cout << ind << "case =:\n";
    solve(g3, indent + 2);
    cout << ind << "end\n";
    
    // Case: g1 > g2 (fake in g2 or g1 is lighter)
    cout << ind << "case >:\n";
    solve(g2, indent + 2);
    cout << ind << "end\n";
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    cin >> n;
    
    int weighings = calculate_weighings(n);
    cout << "need " << weighings << " weighings\n";
    
    vector<int> all_coins;
    for (int i = 1; i <= n; i++) {
        all_coins.push_back(i);
    }
    
    solve(all_coins, 0);
    
    return 0;
}
