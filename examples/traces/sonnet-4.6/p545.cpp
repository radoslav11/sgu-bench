While on circle A:
  a = current angle
  dir = direction of movement
  For each other alive circle B:
    Find angles on circle A where ball is on boundary of B (i.e., at intersection of A and B)
    Find the first such angle in direction dir from a
  a_next = min of (a_end, intersections with other circles) in direction dir
  Move arc to a_next
  If hit circle B: now on both A and B = at equilibrium (since eq is intersection), stop.
  Else: continue (reached x=eq.x or reached eq itself)
