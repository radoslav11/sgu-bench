parse(pos, c):
  if s[pos] == other: return pos+1
  if s[pos] == c: return parse(pos+1, other)  // but this recurses into f(other), not consuming the full f(c)!
