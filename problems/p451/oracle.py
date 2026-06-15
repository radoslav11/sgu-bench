from collections import defaultdict, deque


def expand_relation(rel):
    if rel in ("father", "mother", "son", "daughter", "husband", "wife"):
        return [[(rel, 0)]]

    if rel == "brother":
        return [[("father", 1), ("son", 2)],
                [("mother", 1), ("son", 2)]]

    if rel == "sister":
        return [[("father", 1), ("daughter", 2)],
                [("mother", 1), ("daughter", 2)]]

    if rel == "grandfather":
        return [[("father", 0), ("father", 0)],
                [("mother", 0), ("father", 0)]]

    if rel == "grandmother":
        return [[("father", 0), ("mother", 0)],
                [("mother", 0), ("mother", 0)]]

    if rel == "grandson":
        return [[("son", 0), ("son", 0)],
                [("daughter", 0), ("son", 0)]]

    if rel == "granddaughter":
        return [[("son", 0), ("daughter", 0)],
                [("daughter", 0), ("daughter", 0)]]

    if rel == "uncle":
        return [[("father", 0), ("father", 1), ("son", 2)],
                [("father", 0), ("mother", 1), ("son", 2)],
                [("mother", 0), ("father", 1), ("son", 2)],
                [("mother", 0), ("mother", 1), ("son", 2)]]

    if rel == "aunt":
        return [[("father", 0), ("father", 1), ("daughter", 2)],
                [("father", 0), ("mother", 1), ("daughter", 2)],
                [("mother", 0), ("father", 1), ("daughter", 2)],
                [("mother", 0), ("mother", 1), ("daughter", 2)]]

    if rel == "nephew":
        return [[("father", 1), ("son", 2), ("son", 0)],
                [("mother", 1), ("son", 2), ("son", 0)],
                [("father", 1), ("daughter", 2), ("son", 0)],
                [("mother", 1), ("daughter", 2), ("son", 0)]]

    if rel == "niece":
        return [[("father", 1), ("son", 2), ("daughter", 0)],
                [("mother", 1), ("son", 2), ("daughter", 0)],
                [("father", 1), ("daughter", 2), ("daughter", 0)],
                [("mother", 1), ("daughter", 2), ("daughter", 0)]]

    return []


def solve(line):
    pos = line.find(" is A")
    if pos == -1:
        return ""

    line = line[pos + 5:]

    if line == "" or line == " is A":
        return "0 0"

    relations = []
    for word in line.split():
        if word.endswith("'s"):
            word = word[:-2]
        if word and word not in ("'", "s"):
            relations.append(word)

    fa = {}
    mo = {}
    sp = {}
    ch = defaultdict(set)
    gender = defaultdict(int)
    extra_children = defaultdict(set)

    nid = 1
    cur = {(0, -1)}

    def apply_step(cur_set, rel, flag):
        nonlocal nid
        nxt = set()
        for p, excl in cur_set:

            def emit(target):
                if flag == 2 and target == excl:
                    return
                new_excl = -1 if flag == 2 else excl
                if flag == 1:
                    new_excl = p
                nxt.add((target, new_excl))

            if rel == "father":
                if p not in fa:
                    if p in mo and mo[p] in sp:
                        fa[p] = sp[mo[p]]
                        ch[fa[p]].add(p)
                    else:
                        fid = nid
                        nid += 1
                        fa[p] = fid
                        gender[fid] = 1
                        ch[fid].add(p)
                        if p in mo:
                            sp[fid] = mo[p]
                            sp[mo[p]] = fid
                emit(fa[p])

            elif rel == "mother":
                if p not in mo:
                    if p in fa and fa[p] in sp:
                        mo[p] = sp[fa[p]]
                        ch[mo[p]].add(p)
                    else:
                        mid = nid
                        nid += 1
                        mo[p] = mid
                        gender[mid] = 2
                        ch[mid].add(p)
                        if p in fa:
                            sp[mid] = fa[p]
                            sp[fa[p]] = mid
                emit(mo[p])

            elif rel in ("son", "daughter"):
                want = 1 if rel == "son" else 2
                for c in ch[p]:
                    if gender[c] == want or gender[c] == 0:
                        emit(c)
                if want not in extra_children[p]:
                    cid = nid
                    nid += 1
                    gender[cid] = want
                    ch[p].add(cid)
                    if gender[p] == 2:
                        mo[cid] = p
                        if p not in sp:
                            sid = nid
                            nid += 1
                            sp[p] = sid
                            sp[sid] = p
                            gender[sid] = 1
                            fa[cid] = sid
                            ch[sid].add(cid)
                        else:
                            fa[cid] = sp[p]
                            ch[sp[p]].add(cid)
                    else:
                        fa[cid] = p
                        if gender[p] == 0:
                            gender[p] = 1
                        if p not in sp:
                            sid = nid
                            nid += 1
                            sp[p] = sid
                            sp[sid] = p
                            gender[sid] = 2
                            mo[cid] = sid
                            ch[sid].add(cid)
                        else:
                            mo[cid] = sp[p]
                            ch[sp[p]].add(cid)
                    extra_children[p].add(want)
                    emit(cid)

            elif rel == "husband":
                if p not in sp:
                    hid = nid
                    nid += 1
                    sp[p] = hid
                    sp[hid] = p
                    gender[hid] = 1
                    gender[p] = 2
                emit(sp[p])

            elif rel == "wife":
                if p not in sp:
                    wid = nid
                    nid += 1
                    sp[p] = wid
                    sp[wid] = p
                    gender[wid] = 2
                    gender[p] = 1
                emit(sp[p])

        return nxt

    for rel in relations:
        exps = expand_relation(rel)
        all_results = set()
        for exp in exps:
            tmp = cur
            for step_rel, step_flag in exp:
                tmp = apply_step(tmp, step_rel, step_flag)
            all_results |= tmp
        cur = all_results

    adj = defaultdict(list)

    def add(u, v, w):
        adj[u].append((v, w))
        adj[v].append((u, w))

    for c, f in fa.items():
        add(c, f, 1)
    for c, m in mo.items():
        add(c, m, 1)

    seen = set()
    for a, b in sp.items():
        e = (a, b) if a < b else (b, a)
        if e in seen:
            continue
        seen.add(e)
        add(a, b, 0)

    dist = {0: 0}
    dq = deque([0])
    while dq:
        u = dq.popleft()
        du = dist[u]
        for v, w in adj[u]:
            nd = du + w
            if v not in dist or nd < dist[v]:
                dist[v] = nd
                if w == 0:
                    dq.appendleft(v)
                else:
                    dq.append(v)

    lo = 10**18
    hi = -(10**18)
    for p, _excl in cur:
        if p in dist:
            lo = min(lo, dist[p])
            hi = max(hi, dist[p])

    if lo == 10**18:
        return "0 0"
    return f"{hi} {lo}"


if __name__ == "__main__":
    import sys
    line = sys.stdin.readline().rstrip("\n")
    print(solve(line))
