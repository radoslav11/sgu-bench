#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int P, M, C, K, R, V;
    cin >> P >> M >> C >> K >> R >> V;
    
    int patties_from_flour = P / K;
    int patties_from_milk = M / R;
    int patties_from_cabbage = C / V;
    
    int result = min({patties_from_flour, patties_from_milk, patties_from_cabbage});
    
    cout << result << endl;
    
    return 0;
}
