"""Checker for p176: usage checker.py <in> <out> <ans>; exit 0 = accept.

The minimal production speed (or "Impossible") is deterministic and
must match the reference. The flow assignment is a certificate: M
values with 0 <= f_i <= Z_i, f_i = Z_i on every mandatory pipe, flow
conserved at internal nodes, net outflow of node 1 equal to net inflow
of node N equal to the claimed minimal speed. Any such assignment is
accepted; it is never compared to the reference's flows.
"""

import sys


def reject(message):
    print(message)
    sys.exit(1)


def main():
    in_tokens = open(sys.argv[1]).read().split()
    out_tokens = open(sys.argv[2]).read().split()
    ans_tokens = open(sys.argv[3]).read().split()

    it = iter(in_tokens)
    n = int(next(it))
    m = int(next(it))
    pipes = []
    for _ in range(m):
        u = int(next(it))
        v = int(next(it))
        z = int(next(it))
        c = int(next(it))
        pipes.append((u, v, z, c))

    if not out_tokens:
        reject("empty output")

    ref_impossible = ans_tokens[0].lower() == "impossible"
    if out_tokens[0].lower() == "impossible":
        if not ref_impossible:
            reject("claimed Impossible but a solution exists")
        if len(out_tokens) > 1:
            reject("trailing output after Impossible")
        sys.exit(0)
    if ref_impossible:
        reject("reference says Impossible but submission claims a flow")

    try:
        speed = int(out_tokens[0])
    except ValueError:
        reject(f"first token must be an integer or Impossible, "
               f"got '{out_tokens[0]}'")
    best = int(ans_tokens[0])
    if speed != best:
        reject(f"claimed speed {speed}, minimal speed is {best}")

    if len(out_tokens) != 1 + m:
        reject(f"expected {m} flow values, got {len(out_tokens) - 1}")
    try:
        flows = [int(t) for t in out_tokens[1:]]
    except ValueError:
        reject("non-integer flow value")

    net = [0] * (n + 1)
    for (u, v, z, c), f in zip(pipes, flows):
        if f < 0 or f > z:
            reject(f"flow {f} outside [0, {z}] on pipe {u}->{v}")
        if c == 1 and f != z:
            reject(f"mandatory pipe {u}->{v} not full: {f} < {z}")
        net[u] += f
        net[v] -= f

    if net[1] != speed:
        reject(f"node 1 produces {net[1]}, claimed {speed}")
    if -net[n] != speed:
        reject(f"node {n} consumes {-net[n]}, claimed {speed}")
    for v in range(2, n):
        if net[v] != 0:
            reject(f"flow not conserved at node {v} (net {net[v]})")

    sys.exit(0)


if __name__ == "__main__":
    main()
