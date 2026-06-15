import sys


def is_letter(ch):
    return (b'a'[0] <= ch <= b'z'[0]) or (b'A'[0] <= ch <= b'Z'[0])


def is_symbol(ch):
    # Letters, digits, underscore or hyphen (ASCII only, as in the grammar)
    return is_letter(ch) or (b'0'[0] <= ch <= b'9'[0]) or ch in (b'_'[0], b'-'[0])


def is_valid_prefix(pref):
    if not pref:
        return False
    dot = b'.'[0]
    if pref[0] == dot or pref[-1] == dot:
        return False
    last_dot = False
    for ch in pref:
        if ch == dot:
            if last_dot:
                return False
            last_dot = True
        else:
            if not is_symbol(ch):
                return False
            last_dot = False
    return True


def is_valid_domain(dom):
    return len(dom) in (2, 3) and all(is_letter(ch) for ch in dom)


def is_valid_suffix(suf):
    i = suf.rfind(b'.')
    if i < 0:
        return False
    return is_valid_prefix(suf[:i]) and is_valid_domain(suf[i + 1:])


def is_valid_email(email):
    i = email.find(b'@')
    if i < 0:
        return False
    return is_valid_prefix(email[:i]) and is_valid_suffix(email[i + 1:])


def main():
    data = sys.stdin.buffer.read().split(b'\n')
    n = int(data[0])
    out = []
    for email in data[1:n + 1]:
        email = email.rstrip(b'\r')
        out.append("YES" if is_valid_email(email) else "NO")
    print("\n".join(out))


if __name__ == "__main__":
    main()
