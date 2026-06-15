#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

map<ll,ll> U, L;
ll usum, lsum;

ll ec(ll x1, ll y1, ll x2, ll y2) { return x1*y2 - x2*y1; }

// Returns ec(a, b) for map entries
ll ec(map<ll,ll>::iterator a, map<ll,ll>::iterator b) {
    return ec(a->first, a->second, b->first, b->second);
}

// Remove edge between a and b from sum, add edge between a and c
void replace_right(ll& sum, map<ll,ll>::iterator a, map<ll,ll>::iterator b, map<ll,ll>::iterator c) {
    sum -= ec(a, b);
    sum -= ec(b, c);
    sum += ec(a, c);
}

bool upper_bad(map<ll,ll>::iterator a, map<ll,ll>::iterator b, map<ll,ll>::iterator c) {
    // b is bad for upper hull if cross(a->b, a->c) <= 0 (b is below or on line a-c)
    // (b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y) >= 0 means b is not strictly above
    return (b->first - a->first) * (c->second - a->second) >= 
           (c->first - a->first) * (b->second - a->second);
}

bool lower_bad(map<ll,ll>::iterator a, map<ll,ll>::iterator b, map<ll,ll>::iterator c) {
    return (b->first - a->first) * (c->second - a->second) <= 
           (c->first - a->first) * (b->second - a->second);
}

void add_to_upper(ll x, ll y) {
    // Check if (x,y) is already dominated
    // Find neighbors in upper hull
    auto it = U.lower_bound(x);
    
    if (it != U.end() && it->first == x) {
        if (it->second >= y) return; // dominated
        // Replace
        // Remove edge contributions involving it
        if (it != U.begin() && next(it) != U.end()) {
            auto pv = prev(it), nx = next(it);
            usum -= ec(pv, it);
            usum -= ec(it, nx);
            usum += ec(pv, nx); // temporarily
        } else if (it != U.begin()) {
            usum -= ec(prev(it), it);
        } else if (next(it) != U.end()) {
            usum -= ec(it, next(it));
        }
        it->second = y;
        if (it != U.begin() && next(it) != U.end()) {
            auto pv = prev(it), nx = next(it);
            usum -= ec(pv, nx); // remove temp
            usum += ec(pv, it);
            usum += ec(it, nx);
        } else if (it != U.begin()) {
            usum += ec(prev(it), it);
        } else if (next(it) != U.end()) {
            usum += ec(it, next(it));
        }
    } else {
        // Insert new point
        // Check if it's inside the hull
        // it points to the first element >= x, so prev(it) is the left neighbor
        if (it != U.end() && it != U.begin()) {
            auto left = prev(it), right = it;
            // Check if (x,y) is below the line from left to right
            if (!upper_bad(left, /* fake */ ..., right)) ... // Can't do this without inserting
        }
        U[x] = y;
        it = U.find(x);
    }
    // ... rest of fixup
}
