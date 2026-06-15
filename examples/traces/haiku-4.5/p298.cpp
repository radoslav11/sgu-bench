#include<bits/stdc++.h>
using namespace std;

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int N, M;
    cin >> N >> M;
    
    vector<tuple<int,int,int>> constraints;
    for(int i = 0; i < M; i++){
        int x, y, c;
        cin >> x >> y >> c;
        constraints.push_back({x, y, c});
    }
    
    // Try different starting values for A[1]
    for(int start = -10000; start <= 10000; start++){
        vector<int> A(N+1, INT_MIN);
        vector<int> minVal(N+1, -10000);
        vector<int> maxVal(N+1, 10000);
        
        A[1] = start;
        queue<int> q;
        q.push(1);
        vector<bool> inQueue(N+1, false);
        inQueue[1] = true;
        
        bool valid = true;
        
        while(!q.empty() && valid){
            int u = q.front();
            q.pop();
            inQueue[u] = false;
            
            if(A[u] < -10000 || A[u] > 10000){
                valid = false;
                break;
            }
            
            for(auto& [x, y, c] : constraints){
                // A[x] >= A[y] + c
                if(u == y && A[y] != INT_MIN){
                    int newMin = A[y] + c;
                    if(A[x] == INT_MIN){
                        A[x] = newMin;
                        if(A[x] > 10000) A[x] = 10000;
                        if(A[x] < -10000){
                            valid = false;
                            break;
                        }
                        if(!inQueue[x]){
                            q.push(x);
                            inQueue[x] = true;
                        }
                    } else if(A[x] < newMin){
                        A[x] = newMin;
                        if(A[x] > 10000){
                            valid = false;
                            break;
                        }
                        if(!inQueue[x]){
                            q.push(x);
                            inQueue[x] = true;
                        }
                    }
                }
                if(u == x && A[x] != INT_MIN){
                    int newMax = A[x] - c;
                    if(A[y] == INT_MIN){
                        A[y] = newMax;
                        if(A[y] < -10000) A[y] = -10000;
                        if(A[y] > 10000){
                            valid = false;
                            break;
                        }
                        if(!inQueue[y]){
                            q.push(y);
                            inQueue[y] = true;
                        }
                    } else if(A[y] > newMax){
                        A[y] = newMax;
                        if(A[y] < -10000){
                            valid = false;
                            break;
                        }
                        if(!inQueue[y]){
                            q.push(y);
                            inQueue[y] = true;
                        }
                    }
                }
            }
        }
        
        if(valid && A[N] != INT_MIN){
            bool allSatisfied = true;
            for(auto& [x, y, c] : constraints){
                if(A[x] < A[y] + c){
                    allSatisfied = false;
                    break;
                }
            }
            if(allSatisfied){
                for(int i = 1; i <= N; i++){
                    cout << A[i];
                    if(i < N) cout << " ";
                }
                cout << "\n";
                return 0;
            }
        }
    }
    
    cout << "-1\n";
    return 0;
}
