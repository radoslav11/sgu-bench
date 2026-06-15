#include<iostream>
#include<string>
using namespace std;

int main(){
    int balls[7]; // 0:red, 1:yellow, 2:green, 3:brown, 4:blue, 5:pink, 6:black
    for(int i = 0; i < 7; i++){
        cin >> balls[i];
    }
    
    string lastPotted;
    cin >> lastPotted;
    
    int values[7] = {1, 2, 3, 4, 5, 6, 7};
    int score = 0;
    
    if(balls[0] > 0){
        // Maximum with reds: alternate red(1) with black(7)
        score += balls[0] * (1 + 7);
        
        // Add remaining colours (all except black since we used it with reds)
        for(int i = 1; i < 6; i++){
            score += balls[i] * values[i];
        }
    }
    else {
        // No reds - pot colours in order
        for(int i = 1; i < 7; i++){
            score += balls[i] * values[i];
        }
    }
    
    cout << score << endl;
    
    return 0;
}
