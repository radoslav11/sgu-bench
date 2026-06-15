#include<bits/stdc++.h>
using namespace std;

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int N, N1, N2;
  cin >> N >> N1 >> N2;
  
  double x1, y1, x2, y2;
  cin >> x1 >> y1 >> x2 >> y2;
  
  // Convert to 0-indexed
  N1--; N2--;
  
  // The angle step for each island
  double angleStep = 2.0 * M_PI / N;
  
  // Angle between N1 and N2 going clockwise
  int posDiff = (N2 - N1 + N) % N;
  double angleDiff = posDiff * angleStep;
  
  // We need to find center and initial angle
  // Midpoint of the chord
  double midX = (x1 + x2) / 2.0;
  double midY = (y1 + y2) / 2.0;
  
  // Vector from point 1 to point 2
  double dx = x2 - x1;
  double dy = y2 - y1;
  double dist = sqrt(dx*dx + dy*dy);
  
  // The center lies on the perpendicular bisector of the chord
  // Distance from midpoint to center
  double chordDist = dist / 2.0;
  double radiusSquared = chordDist * chordDist / (sin(angleDiff/2.0) * sin(angleDiff/2.0));
  double radius = sqrt(radiusSquared);
  
  // Perpendicular direction to chord (normalized)
  double perpX = -dy / dist;
  double perpY = dx / dist;
  
  // Distance from midpoint to center along perpendicular
  double centerDist = sqrt(radius * radius - chordDist * chordDist);
  
  // Two possible centers - we need to determine which one
  // Try both and see which gives clockwise ordering
  vector<pair<double,double>> centers;
  centers.push_back({midX + perpX * centerDist, midY + perpY * centerDist});
  centers.push_back({midX - perpX * centerDist, midY - perpY * centerDist});
  
  for(auto& center : centers){
    double cx = center.first;
    double cy = center.second;
    
    // Calculate angle of N1 from center
    double angle1 = atan2(y1 - cy, x1 - cx);
    
    // Generate all coordinates
    vector<pair<double,double>> islands(N);
    
    for(int i = 0; i < N; i++){
      double angle = angle1 - i * angleStep; // clockwise means decreasing angle
      double x = cx + radius * cos(angle);
      double y = cy + radius * sin(angle);
      islands[i] = {x, y};
    }
    
    // Verify with N2
    double dist_check = sqrt((islands[N2].first - x2)*(islands[N2].first - x2) + 
                             (islands[N2].second - y2)*(islands[N2].second - y2));
    
    if(dist_check < 1e-6){
      // This is the correct center
      cout << fixed << setprecision(6);
      for(int i = 0; i < N; i++){
        cout << islands[i].first << " " << islands[i].second << "\n";
      }
      return 0;
    }
  }
  
  return 0;
}
