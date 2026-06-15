#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
using namespace std;

int main() {
    int n;
    cin >> n;
    vector<pair<double, double>> cities;
    double total_weight = 0.0;
    for (int i = 0; i < n; i++) {
        double x, p;
        cin >> x >> p;
        cities.push_back({x, p});
        total_weight += p;
    }
    sort(cities.begin(), cities.end());
    double cumulative = 0.0;
    for (int i = 0; i < n; i++) {
        cumulative += cities[i].second;
        if (cumulative * 2 >= total_weight) {
            cout << fixed << setprecision(5) << cities[i].first << endl;
            return 0;
        }
    }
    // In case no city satisfies (should not happen for n>0), output last
    cout << fixed << setprecision(5) << cities[n-1].first << endl;
    return 0;
}
