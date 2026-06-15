import sys
from itertools import combinations


def serialize(pairs):
    pairs_sorted = sorted(pairs)
    return ",".join(f"{k}={v}" for k, v in pairs_sorted) + (
        "," if pairs_sorted else ""
    )


def main():
    data = sys.stdin.buffer.read().split()
    idx = 0
    n = int(data[idx])
    idx += 1
    all_props = []
    for _ in range(n):
        k = int(data[idx])
        idx += 1
        props = []
        for _ in range(k):
            tok = data[idx].decode()
            idx += 1
            key, val = tok.split("=", 1)
            props.append((key, val))
        all_props.append(props)

    cnt = {}
    for props in all_props:
        k = len(props)
        for sz in range(k + 1):
            for comb in combinations(props, sz):
                key = serialize(comb)
                cnt[key] = cnt.get(key, 0) + 1

    m = int(data[idx])
    idx += 1
    out = []
    for _ in range(m):
        l = int(data[idx])
        idx += 1
        qprops = []
        for _ in range(l):
            tok = data[idx].decode()
            idx += 1
            key, val = tok.split("=", 1)
            qprops.append((key, val))
        key = serialize(qprops)
        out.append(str(cnt.get(key, 0)))

    sys.stdout.write("\n".join(out) + "\n")


if __name__ == "__main__":
    main()
