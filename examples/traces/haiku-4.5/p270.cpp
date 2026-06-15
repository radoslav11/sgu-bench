#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<pair<int, int>> swaps(M);
    for(int i = 0; i < M; i++){
        cin >> swaps[i].first >> swaps[i].second;
    }
    
    set<int> possiblePositions;
    
    vector<int> indices(M);
    for(int i = 0; i < M; i++){
        indices[i] = i;
    }
    
    do {
        int ballPos = 1;
        for(int i = 0; i < M; i++){
            int swapIdx = indices[i];
            int posA = swaps[swapIdx].first;
            int posB = swaps[swapIdx].second;
            
            if(ballPos == posA){
                ballPos = posB;
            } else if(ballPos == posB){
                ballPos = posA;
            }
        }
        possiblePositions.insert(ballPos);
    } while(next_permutation(indices.begin(), indices.end()));
    
    bool first = true;
    for(int pos : possiblePositions){
        if(!first) cout << " ";
        cout << pos;
        first = false;
    }
    cout << "\n";
    
    return 0;
}
