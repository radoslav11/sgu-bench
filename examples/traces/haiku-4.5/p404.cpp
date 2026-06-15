#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    int N, M;
    cin >> N >> M;
    
    vector<string> phrases(M);
    for (int i = 0; i < M; i++) {
        cin >> phrases[i];
    }
    
    int resultIndex = (N - 1) % M;
    cout << phrases[resultIndex] << endl;
    
    return 0;
}
