read n,p
vector<vector<ll>> a (int entries)
// modular det
modmat = a mod p
long double for sign: dmat
sign det integer via long double:
double_det_sign computed
// modular value:
mod = p
det_mod = 1
for col: find pivot row with nonzero entry mod p (>= col)
  if swap, det_mod = -det_mod (mod)
  det_mod = det_mod * pivot % mod
  for rows below: eliminate
result residue r = det_mod (in [0,p))
