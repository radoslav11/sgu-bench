#include<bits/stdc++.h>
using cept;

int main(){
    long long N, H;
    cin >> N >> H;
    
    map<long long, long long> occupied; // x -> y position of topmost pumpkin
    vector<long long> result;
    
    for(long long i = 1; i <= N; i++){
        long long x = 0, y = N;
        
        // Simulate falling
        while(y > 0){
            // Try to go down
            if(occupied.find(x) == occupied.end() || occupied[x] < y - 1){
                y--;
            }
            // Try to go diagonally left
            else if((occupied.find(x-1) == occupied.end() || occupied[x-1] < y - 1) &&
                    (occupied.find(x-1) == occupied.end() || occupied[x-1] < y)){
                x--;
                y--;
            }
            // Try to go diagonally right
            else if((occupied.find(x+1) == occupied.end() || occupied[x+1] < y - 1) &&
                    (occupied.find(x+1) == occupied.end() || occupied[x+1] < y)){
                x++;
                y--;
            }
            else{
                break;
            }
        }
        
        if(y == H){
            result.push_back(i);
        }
        occupied[x] = y;
    }
    
    for(int i = 0; i < result.size(); i++){
        if(i > 0) cout << " ";
        cout << result[i];
    }
    cout << "\n";
    
    return 0;
}
