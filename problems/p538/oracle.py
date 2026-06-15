import sys
# Independent rule from examples: a round bracket is a REAL paren (NOT an
# emoticon) iff it cleanly pairs with the nearest bracket on the relevant
# side AND only letters/spaces lie between. Otherwise it is an emoticon.
# Answer = total brackets - real-paren brackets.
s = sys.stdin.readline().rstrip("\n")
n = len(s)
bracket_pos = [i for i,c in enumerate(s) if c in "()"]
emot = 0
for idx, i in enumerate(bracket_pos):
    if s[i] == '(':
        # nearest bracket to the right
        if idx+1 < len(bracket_pos):
            j = bracket_pos[idx+1]
            between = s[i+1:j]
            if s[j] == ')' and all(ch==' ' or ch.isalpha() for ch in between):
                continue  # real paren
        emot += 1
    else:  # ')'
        if idx-1 >= 0:
            j = bracket_pos[idx-1]
            between = s[j+1:i]
            if s[j] == '(' and all(ch==' ' or ch.isalpha() for ch in between):
                continue
        emot += 1
print(emot)
