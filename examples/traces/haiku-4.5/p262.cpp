#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M, K;
    cin >> N >> M >> K;
    
    vector<string> symbols[K];
    
    // Read all symbols
    for(int i = 0; i < K; i++){
        for(int j = 0; j < N; j++){
            string row;
            cin >> row;
            symbols[i].push_back(row);
        }
    }
    
    int totalPixels = N * M;
    int minPixels = totalPixels + 1;
    long long bestMask = 0;
    
    // Try all possible subsets of pixels
    for(long long mask = 1; mask < (1LL << totalPixels); mask++){
        // Check if this mask can distinguish all symbols
        set<vector<int>> signatures;
        bool valid = true;
        
        for(int s = 0; s < K; s++){
            vector<int> signature;
            for(int p = 0; p < totalPixels; p++){
                if(mask & (1LL << p)){
                    int row = p / M;
                    int col = p % M;
                    signature.push_back(symbols[s][row][col] - '0');
                }
            }
            if(signatures.count(signature)){
                valid = false;
                break;
            }
            signatures.insert(signature);
        }
        
        if(valid){
            int pixelCount = __builtin_popcountll(mask);
            if(pixelCount < minPixels){
                minPixels = pixelCount;
                bestMask = mask;
            }
        }
    }
    
    cout << minPixels << "\n";
    
    // Output the grid
    for(int row = 0; row < N; row++){
        for(int col = 0; col < M; col++){
            int pixelIndex = row * M + col;
            if(bestMask & (1LL << pixelIndex)){
                cout << "1";
            } else {
                cout << "0";
            }
        }
        cout << "\n";
    }
    
    return 0;
}
