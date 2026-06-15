dp_prev = copy of dp before group g
for j from 0 to K:
  for diff from 0 to DSIZE-1:
    best_val = -1, best_t = -1
    for t from 0 to min(c[g], j):
      prev_diff = diff - t * D[g]
      if prev_diff < 0 or prev_diff >= DSIZE: continue
      if dp_prev[j-t][prev_diff] < 0: continue
      val = dp_prev[j-t][prev_diff] + gain[g][t]
      if val > best_val or (val == best_val and ...):
        best_val = val
        best_t = t
    dp_curr[j][diff] = best_val
    taken[g][j][diff] = best_t
