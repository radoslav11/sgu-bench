#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, Q;
    cin >> N >> Q;
    
    list<int> circle;
    for(int i = 1; i <= N; i++){
        circle.push_back(i);
    }
    
    auto it = circle.begin();
    bool clockwise = true;
    int lastErased = -1;
    
    while(!circle.empty()){
        int count = Q;
        while(count > 1){
            if(clockwise){
                ++it;
                if(it == circle.end()) it = circle.begin();
            } else {
                if(it == circle.begin()) it = circle.end();
                --it;
            }
            count--;
        }
        
        lastErased = *it;
        auto nextIt = it;
        ++nextIt;
        if(nextIt == circle.end()) nextIt = circle.begin();
        
        circle.erase(it);
        
        if(circle.empty()) break;
        
        it = nextIt;
        clockwise = (*it % 2 == 1);
    }
    
    cout << lastErased << endl;
    return 0;
}
