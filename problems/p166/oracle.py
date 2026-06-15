import sys

def solve() -> None:
    # Read the entire command line including spaces, without the trailing newline.
    commands = sys.stdin.readline()
    if commands.endswith("\n"):
        commands = commands[:-1]

    # Editor state: list of lines and cursor (row, col).
    lines = [""]          # start with one empty line
    row, col = 0, 0

    def pad_line(r: int, c: int) -> None:
        """
        Ensure lines[r] has length at least c by padding with spaces.
        (Cursor column c is allowed to be beyond end-of-line.)
        """
        s = lines[r]
        if c > len(s):
            lines[r] = s + (" " * (c - len(s)))

    for ch in commands:
        if ch == "Q":
            break

        # Insert lowercase letters or spaces
        if ("a" <= ch <= "z") or ch == " ":
            pad_line(row, col)
            # Insert at position col: s[:col] + ch + s[col:]
            s = lines[row]
            lines[row] = s[:col] + ch + s[col:]
            col += 1

        elif ch == "L":
            if col > 0:
                col -= 1

        elif ch == "R":
            col += 1

        elif ch == "U":
            if row > 0:
                row -= 1

        elif ch == "D":
            if row + 1 < len(lines):
                row += 1

        elif ch == "N":
            # Clamp col to end-of-line before splitting (matches the C++ code).
            if col > len(lines[row]):
                col = len(lines[row])

            s = lines[row]
            head = s[:col]
            tail = s[col:]
            lines[row] = head
            lines.insert(row + 1, tail)
            row += 1
            col = 0

        elif ch == "E":
            col = len(lines[row])

        elif ch == "H":
            col = 0

        elif ch == "X":
            s = lines[row]
            if col < len(s):
                # Delete character at cursor.
                lines[row] = s[:col] + s[col + 1:]
            elif row + 1 < len(lines):
                # Beyond end-of-line: pad with spaces up to col, then merge next line.
                pad_line(row, col)
                lines[row] = lines[row] + lines[row + 1]
                del lines[row + 1]
            # Else: beyond end of last line => do nothing.

        elif ch == "B":
            if col > 0:
                s = lines[row]
                # If cursor is within the real text, delete the character before cursor.
                if col <= len(s):
                    lines[row] = s[:col - 1] + s[col:]
                # Cursor always moves left by one if col > 0.
                col -= 1
            elif row > 0:
                # Merge with previous line.
                prev_len = len(lines[row - 1])
                lines[row - 1] += lines[row]
                del lines[row]
                row -= 1
                col = prev_len
            # Else at (0,0): do nothing.

    # Output: newline between lines, but not after the last one.
    sys.stdout.write("\n".join(lines))

if __name__ == "__main__":
    solve()
