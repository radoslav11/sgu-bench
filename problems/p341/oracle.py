import sys

# Gate type constants
G_INPUT, G_NOT, G_DFF, G_AND, G_NAND, G_OR, G_NOR = range(7)

class Junction:
    __slots__ = ("type", "inputs")
    def __init__(self):
        self.type = -1       # will be set once we parse its definition
        self.inputs = []     # list of upstream junction indices

def trim(s: str) -> str:
    return s.strip()

def op_type(op: str) -> int:
    if op == "NOT":  return G_NOT
    if op == "DFF":  return G_DFF
    if op == "AND":  return G_AND
    if op == "NAND": return G_NAND
    if op == "OR":   return G_OR
    if op == "NOR":  return G_NOR
    return -1

def solve() -> None:
    # Circuit storage
    junctions = []            # id -> Junction
    name_id = {}              # name -> id
    input_order = []          # ids in INPUT(...) order
    output_order = []         # ids in OUTPUT(...) order
    dff_ids = []              # ids of DFF junctions
    tick_inputs = []          # list of tick bitstrings

    def get_id(name: str) -> int:
        """Return existing integer id for junction name, or create a new one."""
        if name in name_id:
            return name_id[name]
        idx = len(junctions)
        name_id[name] = idx
        junctions.append(Junction())
        return idx

    # -------- Parse input --------
    reading_values = False
    for raw in sys.stdin:
        if raw.startswith("#"):
            continue
        t = trim(raw)
        if not t:
            continue

        if not reading_values:
            if t == "INPUT VALUES":
                reading_values = True
                continue

            if t.startswith("INPUT("):
                l = t.find("(")
                r = t.find(")")
                name = trim(t[l+1:r])
                idx = get_id(name)
                junctions[idx].type = G_INPUT
                input_order.append(idx)

            elif t.startswith("OUTPUT("):
                l = t.find("(")
                r = t.find(")")
                name = trim(t[l+1:r])
                idx = get_id(name)
                output_order.append(idx)

            else:
                # Parse: lhs = OP(arg1, arg2, ...)
                eq = t.find("=")
                lhs = trim(t[:eq])
                rhs = trim(t[eq+1:])

                l = rhs.find("(")
                r = rhs.find(")")
                op = trim(rhs[:l])
                args = rhs[l+1:r]

                u = get_id(lhs)
                junctions[u].type = op_type(op)
                if junctions[u].type == G_DFF:
                    dff_ids.append(u)

                # Split args by comma and record their ids
                for part in args.split(","):
                    tok = trim(part)
                    if tok:
                        junctions[u].inputs.append(get_id(tok))
        else:
            tick_inputs.append(t)

    # -------- Simulation --------
    n = len(junctions)
    d = len(dff_ids)

    # Map junction id -> position in dff_out array (or -1 if not a DFF)
    dff_pos = [-1] * n
    for i, u in enumerate(dff_ids):
        dff_pos[u] = i

    # DFF outputs (state) before first tick are all 0
    dff_out = [0] * d

    # Memoization for each tick using stamp technique
    val = [0] * n
    seen = [0] * n
    stamp = 0

    sys.setrecursionlimit(1000000)

    def eval_node(u: int) -> int:
        """Compute junction u's value for current tick, memoized by stamp."""
        nonlocal stamp
        if seen[u] == stamp:
            return val[u]
        seen[u] = stamp

        j = junctions[u]
        t = j.type

        if t == G_INPUT:
            # value already set at tick start
            return val[u]

        if t == G_DFF:
            val[u] = dff_out[dff_pos[u]]
            return val[u]

        if t == G_NOT:
            val[u] = 1 - eval_node(j.inputs[0])
            return val[u]

        if t == G_AND:
            r = 1
            for v in j.inputs:
                r &= eval_node(v)
            val[u] = r
            return val[u]

        if t == G_NAND:
            r = 1
            for v in j.inputs:
                r &= eval_node(v)
            val[u] = 1 - r
            return val[u]

        if t == G_OR:
            r = 0
            for v in j.inputs:
                r |= eval_node(v)
            val[u] = r
            return val[u]

        if t == G_NOR:
            r = 0
            for v in j.inputs:
                r |= eval_node(v)
            val[u] = 1 - r
            return val[u]

        # Should not happen with correct input
        return 0

    out_lines = []
    for tick in tick_inputs:
        stamp += 1

        # Load primary inputs for this tick
        for i, u in enumerate(input_order):
            val[u] = ord(tick[i]) - ord("0")
            seen[u] = stamp

        # Produce outputs
        bits = []
        for u in output_order:
            bits.append("1" if eval_node(u) else "0")
        out_lines.append("".join(bits))

        # Compute and latch next DFF outputs
        next_dff = [0] * d
        for i, u in enumerate(dff_ids):
            # DFF has exactly one input
            next_dff[i] = eval_node(junctions[u].inputs[0])
        dff_out = next_dff

    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
