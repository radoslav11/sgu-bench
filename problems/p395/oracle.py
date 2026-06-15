import sys
from bisect import bisect_left


def main():
    # Read all input lines.
    data = sys.stdin.read().strip().splitlines()

    # Number of existing records.
    n = int(data[0])

    # rec_type[i]:
    #   0 means '+'
    #   1 means '-'
    #   2 means '='
    rec_type = [0] * n

    # rec_arg[i]:
    #   for '+/-': compressed name id
    #   for '=': required count
    rec_arg = [0] * n

    # Mapping from name string to integer id.
    name_id = {}

    # names[id] = original name string.
    names = []

    # Parse all records.
    for i in range(n):
        parts = data[i + 1].split()
        op = parts[0]

        if op == "=":
            # Equality record.
            rec_type[i] = 2
            rec_arg[i] = int(parts[1])
        else:
            # Enter or leave record.
            nm = parts[1]

            # Assign id to name if first time seen.
            if nm not in name_id:
                name_id[nm] = len(names)
                names.append(nm)

            rec_type[i] = 0 if op == "+" else 1
            rec_arg[i] = name_id[nm]

    # Number of distinct original names.
    d = len(names)

    # occ[x] stores all positions where name x appears in '+/-' records.
    occ = [[] for _ in range(d)]

    for i in range(n):
        if rec_type[i] != 2:
            occ[rec_arg[i]].append(i)

    # last_at[x] stores last occurrence of name x.
    last_at = [-1] * d

    for x in range(d):
        if occ[x]:
            last_at[x] = occ[x][-1]

    def next_occ(x, pos):
        """
        Return first occurrence position of name x at or after pos.
        If it does not exist, return a large value.
        """
        arr = occ[x]
        j = bisect_left(arr, pos)
        if j == len(arr):
            return 10**9
        return arr[j]

    # Original names should not be used as fresh generated names.
    used_original = set(names)

    # Base-26 counter for generating fresh names:
    # [0] -> 'a', [1] -> 'b', ..., [25] -> 'z', [0,0] -> 'aa', etc.
    digits = [0]

    def next_fresh():
        """
        Generate a lowercase name that does not occur in the original log.
        The generator itself never goes backwards, so generated names are unique.
        """
        nonlocal digits

        while True:
            # Convert digits to a string.
            s = "".join(chr(ord("a") + v) for v in digits)

            # Increment base-26 counter.
            p = len(digits) - 1

            # Carry through trailing 25s.
            while p >= 0 and digits[p] == 25:
                digits[p] = 0
                p -= 1

            if p < 0:
                # z -> aa, zz -> aaa, etc.
                digits = [0] * (len(digits) + 1)
            else:
                digits[p] += 1

            # Skip names already present in original input.
            if s not in used_original:
                return s

    # present[x] is True if original name x is currently inside as a named guest.
    present = [False] * d

    # Anonymous guests currently inside.
    # We keep their names so we can output valid leave records.
    anon = []

    # Total number of guests currently inside.
    cnt = 0

    # Output polished log.
    out = []

    def open_named(x):
        """Output '+ name' and mark original name x as inside."""
        nonlocal cnt
        out.append("+ " + names[x])
        present[x] = True
        cnt += 1

    def close_named(x):
        """Output '- name' and mark original name x as outside."""
        nonlocal cnt
        out.append("- " + names[x])
        present[x] = False
        cnt -= 1

    def open_anon(nm):
        """Output '+ nm' and remember this anonymous guest."""
        nonlocal cnt
        out.append("+ " + nm)
        anon.append(nm)
        cnt += 1

    def close_anon():
        """Close the most recently opened anonymous guest."""
        nonlocal cnt
        nm = anon.pop()
        out.append("- " + nm)
        cnt -= 1

    # Process original records in order.
    for i in range(n):
        t = rec_type[i]
        a = rec_arg[i]

        if t == 0:
            # Original record: '+ name'

            if present[a]:
                # Cannot enter twice, so insert '- name' first.
                close_named(a)

            if last_at[a] == i:
                # If this name never appears again, we do not need to track it by id.
                open_anon(names[a])
            else:
                # Otherwise keep it as a named active visitor.
                open_named(a)

        elif t == 1:
            # Original record: '- name'

            if present[a]:
                # Valid leave.
                close_named(a)
            else:
                # Person is not inside, so insert a matching enter immediately before.
                out.append("+ " + names[a])
                out.append("- " + names[a])
                # cnt does not change overall.

        else:
            # Original record: '= k'
            k = a

            if cnt < k:
                # Need to insert enter records.
                need = k - cnt

                candidates = []

                for x in range(d):
                    if not present[x]:
                        nx = next_occ(x, i)

                        # Best candidates are absent people whose next event is '- x'.
                        if nx != 10**9 and rec_type[nx] == 1:
                            candidates.append((nx, x))

                # Prefer the earliest future '- x'.
                candidates.sort()

                done = 0

                # Open useful candidates first.
                for _, x in candidates:
                    if done == need:
                        break
                    open_named(x)
                    done += 1

                # Fill remaining slots with fresh anonymous people.
                while done < need:
                    open_anon(next_fresh())
                    done += 1

            elif cnt > k:
                # Need to insert leave records.
                need = cnt - k

                liabilities = []
                useful = []

                for x in range(d):
                    if present[x]:
                        nx = next_occ(x, i)

                        if nx != 10**9 and rec_type[nx] == 0:
                            # If next event is '+ x', x must be closed before it anyway.
                            liabilities.append((nx, x))
                        else:
                            # Usually next event is '- x', so keeping x is useful.
                            useful.append((nx, x))

                # Close liabilities with earliest future '+ x' first.
                liabilities.sort()

                # If forced to close useful people, close those with latest future '- x' first.
                useful.sort(reverse=True)

                rem = need

                # First close liabilities.
                for _, x in liabilities:
                    if rem == 0:
                        break
                    close_named(x)
                    rem -= 1

                # Then close anonymous people.
                while rem > 0 and anon:
                    close_anon()
                    rem -= 1

                # Finally close useful named people if still necessary.
                for _, x in useful:
                    if rem == 0:
                        break
                    close_named(x)
                    rem -= 1

            # Now the count is exactly k, so output original equality record.
            out.append("= " + str(k))

    # Print final log length and records.
    print(len(out))
    print("\n".join(out))


if __name__ == "__main__":
    main()
