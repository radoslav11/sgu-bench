#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll,ll> pll;

int n;
vector<pll> pts;
ll ans = 0;

// Divide and conquer: count friendly pairs where one point from [l,mid], one from [mid+1,r]
// pts sorted by x in [l,r]
// For cross pair (i in left, j in right):
// friendly iff no point k in [l,r] (k!=i,j) with x in [xi,xj] and y in [min(yi,yj),max(yi,yj)]
// = no left point k!=i in [l,mid] with xk>=xi and yk in [min(yi,yj),max(yi,yj)]
//   AND no right point k!=j in [mid+1,r] with xk<=xj and yk in range

// This is still complex due to the x-range constraints on left/right points
