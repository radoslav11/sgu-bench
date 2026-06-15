#include<bits/stdc++.h>
using namespace std;

int main(){
  int N, x;
  cin >> N >> x;
  
  vector<set<int>> friends(N + 1);
  
  // Read friend lists
  for(int i = 1; i <= N; i++){
    int d;
    cin >> d;
    for(int j = 0; j < d; j++){
      int f;
      cin >> f;
      friends[i].insert(f);
    }
  }
  
  // Find friends of friends for x
  set<int> friendsOfFriends;
  
  // For each friend of x
  for(int friend_of_x : friends[x]){
    // For each friend of that friend
    for(int friend_of_friend : friends[friend_of_x]){
      // If not x, not a direct friend of x, and not already added
      if(friend_of_friend != x && friends[x].count(friend_of_friend) == 0){
        friendsOfFriends.insert(friend_of_friend);
      }
    }
  }
  
  cout << friendsOfFriends.size() << "\n";
  for(int fof : friendsOfFriends){
    cout << fof << "\n";
  }
  
  return 0;
}
