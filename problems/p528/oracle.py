import sys


def solve() -> None:
    # Read all input lines.
    data = sys.stdin.read().splitlines()

    # First line: maximum allowed valid-object length.
    m = int(data[0])

    # Second line: character sequence to process.
    c = data[1]

    # Frame types for the stack of open containers.
    LIST = 0      # Inside a list.
    DICT_K = 1    # Inside a dictionary, expecting a key or closing 'e'.
    DICT_V = 2    # Inside a dictionary, expecting a value.

    # Parser progress states.
    NONE = 0      # At a clean boundary.
    INT = 1       # Reading integer digits after 'i'.
    STR_LEN = 2   # Reading string length digits.
    STR_BODY = 3  # Reading string body characters.

    # Stack of open containers.
    frames = []

    # Current parser progress.
    progress = NONE

    # Integer parsing state.
    int_digits = 0
    int_zero = False

    # String parsing state.
    #
    # While reading STR_LEN, str_val is the parsed string length.
    # While reading STR_BODY, str_val is the number of body characters remaining.
    str_val = 0
    str_len_digits = 0
    str_len_zero = False

    # Whether the top-level object has started/completed.
    root_started = False
    root_completed = False

    # Sum of g_cost(frame) over all open frames.
    g_sum = 0

    # We never need string lengths larger than this cap for feasibility checks.
    # If a length exceeds m, then completion is certainly too long.
    CAP = m + 1

    def g_cost(frame: int) -> int:
        """
        Cost to complete this frame assuming the currently active item inside it
        has just completed.
        """
        if frame == DICT_K:
            # A key just finished; need shortest value "0:" and closing 'e'.
            return 3
        else:
            # List or dictionary after value can close with 'e'.
            return 1

    def f_cost(frame: int) -> int:
        """
        Cost to complete this frame from a clean boundary directly inside it.
        """
        if frame == DICT_V:
            # Must provide shortest value "0:" and then close with 'e'.
            return 3
        else:
            # List or dictionary expecting key can close with 'e'.
            return 1

    def min_remaining() -> int:
        """
        Return the minimum number of additional characters needed to finish the
        current prefix as a valid bencoded object.
        """
        nonlocal progress, root_started, root_completed
        nonlocal frames, g_sum
        nonlocal int_digits, str_val

        # Already complete: need nothing.
        if root_completed:
            return 0

        # At a clean boundary, not inside an integer/string.
        if progress == NONE:
            # If no root item has started, shortest possible object has length 2.
            if not root_started:
                return 2

            # If no frames are open, the root is effectively complete.
            if not frames:
                return 0

            # Complete the innermost frame using f_cost,
            # and all outer frames using their g_cost.
            top = frames[-1]
            return f_cost(top) + (g_sum - g_cost(top))

        # If inside an unfinished integer.
        if progress == INT:
            # Need one digit if no digits yet, then final 'e'.
            finish_cost = (1 if int_digits == 0 else 0) + 1

        # If reading string length.
        elif progress == STR_LEN:
            # Need ':' and then str_val body characters.
            finish_cost = 1 + str_val

        # If reading string body.
        else:
            # Need exactly str_val more raw characters.
            finish_cost = str_val

        # After this item finishes, all open containers still need closing.
        return finish_cost + g_sum

    def item_completed() -> None:
        """
        Called whenever a full item has just been parsed.
        Updates the parent container state, or marks root complete.
        """
        nonlocal root_completed, g_sum

        # If no container is open, this item is the root object.
        if not frames:
            root_completed = True
            return

        # Otherwise, update dictionary state if needed.
        top = frames[-1]

        if top == DICT_K:
            # A dictionary key completed; now it expects a value.
            g_sum -= g_cost(DICT_K)
            frames[-1] = DICT_V
            g_sum += g_cost(DICT_V)

        elif top == DICT_V:
            # A dictionary value completed; now it expects another key or 'e'.
            g_sum -= g_cost(DICT_V)
            frames[-1] = DICT_K
            g_sum += g_cost(DICT_K)

        # If top is LIST, nothing changes.

    def start_item(ch: str) -> bool:
        """
        Try to start a new bencoded item with character ch.
        Return True if successful, False otherwise.
        """
        nonlocal progress, int_digits, int_zero
        nonlocal str_val, str_len_digits, str_len_zero
        nonlocal g_sum

        if ch == "i":
            # Start integer.
            progress = INT
            int_digits = 0
            int_zero = False
            return True

        if ch == "l":
            # Start list.
            frames.append(LIST)
            g_sum += g_cost(LIST)
            return True

        if ch == "d":
            # Start dictionary, initially expecting a key.
            frames.append(DICT_K)
            g_sum += g_cost(DICT_K)
            return True

        if "0" <= ch <= "9":
            # Start string length.
            progress = STR_LEN
            str_val = ord(ch) - ord("0")
            str_len_digits = 1
            str_len_zero = (ch == "0")
            return True

        # No other character can begin an item.
        return False

    def process(ch: str) -> bool:
        """
        Process one input character.
        Return False if the prefix becomes syntactically invalid.
        """
        nonlocal progress
        nonlocal int_digits, int_zero
        nonlocal str_val, str_len_digits, str_len_zero
        nonlocal root_started, root_completed
        nonlocal g_sum

        # Extra characters after the root object are forbidden.
        if root_completed:
            return False

        # Currently reading integer.
        if progress == INT:
            if "0" <= ch <= "9":
                # Leading zero is illegal unless integer is exactly "0".
                if int_zero and int_digits >= 1:
                    return False

                if ch == "0" and int_digits == 0:
                    int_zero = True

                int_digits += 1
                return True

            if ch == "e":
                # Integer must contain at least one digit.
                if int_digits == 0:
                    return False

                progress = NONE
                item_completed()
                return True

            return False

        # Currently reading string length.
        if progress == STR_LEN:
            if "0" <= ch <= "9":
                # Leading zero in string length is illegal unless length is 0.
                if str_len_zero and str_len_digits >= 1:
                    return False

                # Accumulate length, but cap to avoid huge Python integers.
                if str_val <= CAP:
                    str_val = str_val * 10 + (ord(ch) - ord("0"))
                    if str_val > CAP:
                        str_val = CAP

                str_len_digits += 1
                return True

            if ch == ":":
                # Start reading string body.
                progress = STR_BODY

                # Empty string completes immediately.
                if str_val == 0:
                    progress = NONE
                    item_completed()

                return True

            return False

        # Currently reading string body.
        if progress == STR_BODY:
            # Any character is allowed in string body.
            str_val -= 1

            # String completes after exactly the required number of chars.
            if str_val == 0:
                progress = NONE
                item_completed()

            return True

        # progress == NONE: we are at a boundary between items.

        if not frames:
            # If root already started, a new item would be trailing garbage.
            if root_started:
                return False

            # Start root object.
            root_started = True
            return start_item(ch)

        # Inspect current innermost container.
        top = frames[-1]

        if top == LIST or top == DICT_K:
            # Lists and dictionaries expecting keys may close with 'e'.
            if ch == "e":
                g_sum -= g_cost(top)
                frames.pop()

                # The closed list/dictionary is itself one completed item.
                item_completed()
                return True

            # Otherwise, start an item inside the container.
            return start_item(ch)

        # top == DICT_V: dictionary expects a value.
        # It cannot close here.
        if ch == "e":
            return False

        # Start dictionary value.
        return start_item(ch)

    n = len(c)

    # Largest feasible prefix length found so far.
    j_max = 0

    # Scan input.
    for i, ch in enumerate(c):
        # Stop on syntax error.
        if not process(ch):
            break

        # Current prefix length.
        new_j = i + 1

        # Stop if even the shortest completion exceeds m.
        if new_j + min_remaining() > m:
            break

        # This prefix can still be completed within m.
        j_max = new_j

    # Valid iff entire input was consumed as a complete root object.
    if j_max == n and root_completed:
        print("ok")
    else:
        print(f"Error at position {j_max}!")


if __name__ == "__main__":
    solve()
