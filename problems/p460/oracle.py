import sys

def pluralize(word):
    # Rule 1: ends with 'y'
    if word.endswith('y'):
        return word[:-1] + 'ies'
    # Rule 2: ends with 'ch', or 'o', 'x', 's'
    if word.endswith('ch') or word[-1] in {'o', 'x', 's'}:
        return word + 'es'
    # Rule 3: ends with 'fe' or 'f'
    if word.endswith('fe'):
        # Drop 'fe', add 'ves'
        return word[:-2] + 'ves'
    if word.endswith('f'):
        # Drop 'f', add 'ves'
        return word[:-1] + 'ves'
    # Rule 4: default
    return word + 's'

def main():
    data = sys.stdin.read().split()
    n = int(data[0])
    words = data[1:]
    for w in words[:n]:
        print(pluralize(w))

if __name__ == "__main__":
    main()
