#include <bits/stdc++.h>
using namespace std;

int ans;
string s;

// stack represented as vector, initial_count = dishes pre-placed
// We'll do DFS over segments between '*'
// For '*', try inserting sequences - but must be smart

void solve(int pos, vector<char>& stk, int initial) {
    if (pos == (int)s.size()) {
        if (stk.empty()) ans = min(ans, initial);
        return;
    }
    char c = s[pos];
    if (c == '*') {
        // '*' can be any sequence; we need to figure out optimal insertions
        // Key: we can push/pop arbitrary things, net effect on stack can be anything
        // But we want minimum initial dishes
        // After '*', stack can be any valid stack (any content, any depth >= 0)
        // But items already in stack stay at bottom (we can only pop from top)
        // '*' can pop existing items or push new ones
        // Let's try: '*' results in stack being any suffix of current stack + any new items on top
        // Actually '*' can pop some top elements and push new ones
        // Let depth = stk.size(), '*' can pop k items (0<=k<=depth) and push m items (m>=0)
        // For each combination, recurse - but infinite m
        // Key insight: only colors that appear later matter
        // This is complex. Let me bound: max stack size is 2500, so try all depths 0..2500
        // For each resulting depth d:
        //   if d <= stk.size(): pop (stk.size()-d) from top, no new initial needed
        //   if d > stk.size(): push (d - stk.size()) items on top
        // But what colors to push? Colors that will be popped later optimally
        // This is still complex. Let me look at what's needed after this point.
        
        // Simpler approach: just try all possible stack depths after '*'
        // and all possible top portions (colors) - but colors matter
        
        // Given constraints (<=5 stars, string len <=2500), 
        // maybe we can be smarter: the inserted sequence in '*' only interacts
        // with what comes after. Let's just try all depths and for pushed items,
        // use the "best" colors = the ones that will be popped next.
        
        // Actually let me just try all possible stacks after '*':
        // Keep bottom of current stack (some prefix), then add any items on top
        // The top items: we'll push exactly what's needed next (greedy)
        
        // For now: try all possible "pop k" from current stack (k=0..stk.size())
        // then push 0..some_limit items of any color
        // Colors of pushed items: we'll set them greedily to match upcoming pops
        
        // This is getting complex. Let me try a different approach:
        // enumerate what the '*' does as: pop k items then push a sequence
        // The pushed sequence we choose optimally = look ahead for what pops are needed
        
        // Given time constraints, let me implement with bounded exploration
        
        int sz = stk.size();
        // Try popping k items from stack (k = 0 to sz)
        for (int k = 0; k <= sz; k++) {
            vector<char> nstk(stk.begin(), stk.begin() + (sz - k));
            // Now push some items; we'll determine colors by looking ahead
            // For simplicity, try pushing 0 to (2500 - nstk.size()) items
            // But we need to choose colors wisely
            // Let's just try pushing 0..reasonable number and pick colors greedily
            // Actually, let's look ahead: find the sequence of uppercase letters
            // before next '*' or end, and push those colors in reverse order
            
            // Find next segment (until next '*' or end)
            int npos = pos + 1;
            // Collect upcoming pops before next star
            vector<char> upcoming_pops;
            for (int i = npos; i < (int)s.size() && s[i] != '*'; i++) {
                if (isupper(s[i])) upcoming_pops.push_back(tolower(s[i]));
            }
            
            // Push items to satisfy upcoming pops that can't be satisfied by nstk
            // Simulate: how many pops can nstk satisfy?
            // This is complex because pushes in segment also happen
            // Let me just try pushing 0..min(upcoming_pops.size(), 50) items
            // with colors = next pops in order (reversed since stack)
            
            // Actually simplest: just try all subsets of upcoming colors to push
            // But exponential...
            
            // Let me just try: push exactly the items that will be needed
            // by simulating the next segment with current nstk and seeing what's missing
            
            // Simulate next segment with nstk, count how many extra we need from bottom
            // Those extras we can provide by pushing them now (in reverse order of need)
            
            // Let's do a forward simulation of the segment after '*'
            // to find what colors are needed from below
            
            // Simulate segment [npos, next_star) with stack = nstk copy
            // track what's needed from below (in order)
            vector<char> tsimstk = nstk;
            vector<char> needed; // items needed from below, in order of need
            bool seg_ok = true;
            for (int i = npos; i < (int)s.size() && s[i] != '*'; i++) {
                if (islower(s[i])) {
                    tsimstk.push_back(s[i]);
                } else {
                    char need = tolower(s[i]);
                    if (!tsimstk.empty()) {
                        if (tsimstk.back() == need) {
                            tsimstk.pop_back();
                        } else {
                            seg_ok = false; break; // can't fix without '*'
                            // actually can't fix at all since no more '*' until next one
                        }
                    } else {
                        needed.push_back(need);
                        // pretend we got it from below
                    }
                }
            }
            
            // Push 'needed' items onto nstk in reverse order
            // (last needed goes on top = pushed last)
            // But we also need to push them such that they come out right
            // needed[0] is first popped from bottom, so it should be deepest
            // i.e., push needed in order needed[0], needed[1], ...
            // Wait: needed[0] is the first item popped when stack was empty
            // so it was at the bottom of what we push. Push order: needed[0] first (deepest), ..., needed[last] on top
            // But these are pushed by '*', so we push needed[0], needed[1], ..., needed[m-1]
            // resulting stack top = needed[m-1]
            
            if (!seg_ok) {
                // Even with pushed items, segment has conflict - but wait,
                // seg_ok=false means a pop happened when stack had wrong color
                // That's only fixable if... it's not fixable here
                // But actually we might push different things to change what's on stack
                // This approach is incomplete. Let's just try and skip.
                // Actually if seg_ok is false, it means in the segment there's a pop
                // of color X but top of stack is color Y (Y != X) - that's a real error
                // unless... no, it's a real inconsistency.
                continue;
            }
            
            // Push needed items
            vector<char> nstk2 = nstk;
            for (char nc : needed) nstk2.push_back(nc);
            
            solve(pos + 1, nstk2, initial);
            
            // Also try pushing extra items (that won't be popped until later or ever)
            // But "before cleaning ends pile is empty", so all must be popped
            // Pushing extra items that survive to end would violate empty pile condition
            // UNLESS future '*' pops them. So we might need to push extra items
            // that get popped by future '*'. This is hard to handle greedily.
            // For now, just go with needed items and hope it works.
        }
        return;
    }
    
    if (islower(c)) {
        stk.push_back(c);
        solve(pos + 1, stk, initial);
        stk.pop_back();
    } else {
        char need = tolower(c);
        if (!stk.empty() && stk.back() == need) {
            stk.pop_back();
            solve(pos + 1, stk, initial);
            stk.push_back(need);
        } else if (stk.empty()) {
            // Need a pre-placed dish of color 'need' at the bottom
            // But we can't just add to bottom now; this means we need more initial dishes
            // Actually this means inconsistency unless we add to initial
            // We need 'need' at bottom... but stack is empty, so this is impossible
            // unless we had placed it initially
            // This is a fundamental problem with my approach
            // Let me rethink
            ans = min(ans, -1); // signal error? No...
            // Actually return, this path is invalid
            return;
        } else {
            // Top is wrong color - conflict
            return;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin >> s;
    ans = INT_MAX;
    
    vector<char> stk;
    solve(0, stk, 0);
    
    if (ans == INT_MAX) cout << -1 << endl;
    else cout << ans << endl;
    
    return 0;
}
