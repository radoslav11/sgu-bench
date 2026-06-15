#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long N, C, P, T;
    cin >> N >> C >> P >> T;
    
    vector<long long> A(N+1);
    for(int i = 1; i <= N; i++){
        cin >> A[i];
    }
    
    long long maxPeople = 0;
    
    // Try all possible maximum floors for visits
    for(int maxFloor = 0; maxFloor <= N; maxFloor++){
        long long timePerTrip = (maxFloor == 0) ? 0 : 2LL * maxFloor * P;
        
        if(timePerTrip > T) break;
        
        long long timeLeft = T;
        long long peopleDelivered = 0;
        vector<long long> remaining = A;
        
        while(maxFloor > 0 && timeLeft >= timePerTrip){
            long long capacity = C;
            long long peoplePicked = 0;
            
            for(int floor = 1; floor <= maxFloor && capacity > 0; floor++){
                long long canTake = min(capacity, remaining[floor]);
                peoplePicked += canTake;
                remaining[floor] -= canTake;
                capacity -= canTake;
            }
            
            if(peoplePicked == 0) break;
            
            peopleDelivered += peoplePicked;
            timeLeft -= timePerTrip;
        }
        
        maxPeople = max(maxPeople, peopleDelivered);
    }
    
    cout << maxPeople << endl;
    
    return 0;
}
