import sys, bisect

def normalize(pos, N):
    """Wrap pos onto [1..N] circularly."""
    if pos <= 0:
        return pos + N
    if pos > N:
        return pos - N
    return pos

def get_intervals(head, size, N):
    """Return list of 1 or 2 intervals for a block of length size from head."""
    if head + size - 1 <= N:
        return [(head, head + size - 1)]
    else:
        rem = size - (N - head + 1)
        return [(head, N), (1, rem)]

def format_intervals(intervals):
    """Format intervals as 'a-b,c-d,...' or single numbers."""
    intervals.sort()
    parts = []
    for l, r in intervals:
        parts.append(f"{l}" if l == r else f"{l}-{r}")
    return ",".join(parts)

def main():
    data = sys.stdin.read().split()
    it = iter(data)
    N, Q = map(int, (next(it), next(it)))
    slots = next(it).strip()

    # Two sorted lists for clusters:
    #  by size: list of (size, head, id)
    #  by pos: list of (head, id)
    clusters_by_size = []
    clusters_by_pos = []

    # Maps and counters
    next_cid = 0
    head_to_id = {}
    info = {}  # cid -> (head, size)
    alloc = {} # query idx -> intervals

    def add_cluster(head, size):
        nonlocal next_cid
        cid = next_cid
        next_cid += 1
        tup_s = (size, head, cid)
        tup_p = (head, cid)
        # insert in sorted order
        bisect.insort(clusters_by_size, tup_s)
        bisect.insort(clusters_by_pos, tup_p)
        head_to_id[head] = cid
        info[cid] = (head, size)

    def remove_cluster(cid):
        head, size = info[cid]
        # remove by binary search
        i = bisect.bisect_left(clusters_by_size, (size, head, cid))
        clusters_by_size.pop(i)
        j = bisect.bisect_left(clusters_by_pos, (head, cid))
        clusters_by_pos.pop(j)
        head_to_id.pop(head)
        info.pop(cid)

    # Build initial clusters by scanning the circle
    # Find an occupied slot to start
    occ = next((i+1 for i,ch in enumerate(slots) if ch=='X'), None)
    if occ is None:
        add_cluster(1, N)
    else:
        i = occ
        while True:
            if slots[i-1]=='.':
                h=i; length=0
                while slots[i-1]=='.':
                    length+=1
                    i = normalize(i+1, N)
                add_cluster(h, length)
            else:
                i = normalize(i+1, N)
            if i==occ:
                break

    out = []
    for q in range(1, Q+1):
        typ = next(it)
        if typ=='PARK':
            S = int(next(it))
            # find first cluster size>=S
            i = bisect.bisect_left(clusters_by_size, (S, -1, -1))
            if i==len(clusters_by_size):
                out.append("NO ROOM")
            else:
                size, head, cid = clusters_by_size[i]
                # record head/size before format_intervals sorts iv in place
                iv = get_intervals(head, S, N)
                alloc[q] = (head, S)
                out.append(format_intervals(iv))
                # remove old cluster and maybe add remainder
                remove_cluster(cid)
                if size>S:
                    add_cluster(normalize(head+S, N), size-S)

        else:  # LEAVE
            qi = int(next(it))
            head, size = alloc.pop(qi)
            if not clusters_by_pos:
                add_cluster(head, size)
            else:
                # find neighbor by head
                endpos = normalize(head+size, N)
                j = bisect.bisect_left(clusters_by_pos, (endpos, -1))
                if j==len(clusters_by_pos):
                    j=0
                # predecessor in circular list
                k = j-1 if j>0 else len(clusters_by_pos)-1
                left_id  = clusters_by_pos[k][1]
                right_id = clusters_by_pos[j][1]

                lhead, lsize = info[left_id]
                rhead, rsize = info[right_id]

                ml = normalize(lhead+lsize, N)==head
                mr = normalize(head+size, N)==rhead

                if ml:
                    head = lhead
                    size += lsize
                    remove_cluster(left_id)
                if mr and right_id in info:
                    size += rsize
                    remove_cluster(right_id)
                if size==N:
                    head=1
                add_cluster(head, size)

    sys.stdout.write("\n".join(out) + "\n")

if __name__=='__main__':
    main()
