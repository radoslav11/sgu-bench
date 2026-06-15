import sys
import bisect

def mod(x: int, m: int) -> int:
    """Mathematical modulo in [0, m)."""
    return (x % m + m) % m

def solve() -> None:
    it = iter(sys.stdin.read().strip().split())
    n = int(next(it))
    L = int(next(it))

    p = [0] * n
    c = [0] * n
    d = [0] * n
    for i in range(n):
        p[i] = int(next(it))
        c[i] = int(next(it))
        d[i] = int(next(it))

    T = int(next(it))

    order = list(range(n))
    order.sort(key=lambda i: p[i])

    sp = [p[i] for i in order]
    sc = [c[i] for i in order]
    sd = [d[i] for i in order]

    cw_info = []
    for i in range(n):
        if sd[i] == 1:
            cw_final_pos = mod(sp[i] + T, L)
            cw_info.append((cw_final_pos, i))
    cw_info.sort()

    cw_final_positions = [x for x, _ in cw_info]

    shift = 0
    for i in range(n):
        if sd[i] == 1:
            shift += (sp[i] + T) // L
        else:
            if sp[i] == 0:
                shift -= (L + T - 1) // L
            else:
                shift -= (L - sp[i] + T - 1) // L

    shift %= n

    slot_pos = [0.0] * n
    slot_dir = [0] * n
    slot_color = [0] * n

    for i in range(n):
        slot_pos[i] = float(mod(sp[i] + T * sd[i], L))
        slot_dir[i] = sd[i]

        if sd[i] == 1:
            slot_color[i] = sc[i]
        else:
            if not cw_info:
                slot_color[i] = sc[i]
            else:
                q = sp[i]
                ccw_final = int(slot_pos[i])

                j = bisect.bisect_left(cw_final_positions, ccw_final + 1)
                if j == len(cw_info):
                    j = 0
                _, cw_idx = cw_info[j]
                p0 = sp[cw_idx]

                if q >= p0:
                    t0 = (q - p0) // 2
                else:
                    t0 = (q + L - p0) // 2

                if t0 <= T:
                    slot_color[i] = sc[cw_idx]
                else:
                    slot_color[i] = sc[i]

    slot_order = list(range(n))
    slot_order.sort(key=lambda i: slot_pos[i])

    final_pos = [0.0] * n
    final_dir = [0] * n
    final_color = [0] * n

    for i in range(n):
        slot = slot_order[(i + shift) % n]
        final_pos[i] = slot_pos[slot]
        final_dir[i] = slot_dir[slot]
        final_color[i] = slot_color[slot]

    inv_order = [0] * n
    for sorted_i, orig_i in enumerate(order):
        inv_order[orig_i] = sorted_i

    out_lines = []
    for orig_i in range(n):
        idx = inv_order[orig_i]
        out_lines.append(f"{final_pos[idx]:.3f} {final_color[idx]} {final_dir[idx]}")
    sys.stdout.write("\n".join(out_lines))

if __name__ == "__main__":
    solve()
