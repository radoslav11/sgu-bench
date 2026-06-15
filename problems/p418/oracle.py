import sys


# Token kinds.
WORD = "word"      # Sequence of letters, converted to lowercase.
STRING = "string"  # Pascal string literal without surrounding apostrophes.
PUNCT = "punct"    # Single punctuation character.


def tokenize(src):
    """
    Convert the whole Pascal source into tokens.

    Each token is represented as a pair:
        (kind, value)
    """
    tokens = []             # Result token list.
    i = 0                   # Current source index.
    n = len(src)            # Source length.

    while i < n:
        c = src[i]          # Current character.

        # Whitespace is irrelevant.
        if c.isspace():
            i += 1
            continue

        # A word is a maximal sequence of letters.
        if c.isalpha():
            word = []

            while i < n and src[i].isalpha():
                word.append(src[i].lower())
                i += 1

            tokens.append((WORD, "".join(word)))
            continue

        # Pascal string literal.
        if c == "'":
            i += 1          # Skip opening apostrophe.
            content = []    # Literal contents.

            while i < n:
                if src[i] == "'":
                    # Pascal escapes an apostrophe as two apostrophes.
                    if i + 1 < n and src[i + 1] == "'":
                        content.append("'")
                        i += 2
                    else:
                        i += 1      # Closing apostrophe.
                        break
                else:
                    content.append(src[i])
                    i += 1

            tokens.append((STRING, "".join(content)))
            continue

        # Everything else is punctuation.
        tokens.append((PUNCT, c))
        i += 1

    return tokens


class ParserExtractor:
    """
    Parses the restricted Pascal parser and extracts grammar productions.
    """

    def __init__(self, tokens):
        self.tokens = tokens    # Full token list.
        self.pos = 0            # Current token pointer.

    def at_word(self, word):
        """
        Return True if the current token is the given lowercase word.
        """
        return (
            self.pos < len(self.tokens)
            and self.tokens[self.pos][0] == WORD
            and self.tokens[self.pos][1] == word
        )

    def current_value(self):
        """
        Return current token value.
        """
        return self.tokens[self.pos][1]

    def parse_segments(self):
        """
        Parse a list of segments until the matching 'end'.

        Segment representation:
        - unconditional call:
              ("call", routine_name)
        - conditional:
              ("if", branches, else_error)

          where branches is a list of:
              (terminal_character, inner_segments)
        """
        segments = []

        while not self.at_word("end"):
            if self.at_word("if"):
                segments.append(self.parse_conditional())
            else:
                # Unconditional call: Name ;
                name = self.current_value()
                self.pos += 1      # Consume name.
                self.pos += 1      # Consume semicolon.
                segments.append(("call", name))

        return segments

    def parse_conditional(self):
        """
        Parse an If / Else If / Else Error conditional chain.
        """
        branches = []              # Successful branches.
        else_error = False         # Whether the chain ends with Else Error.

        self.pos += 1              # Consume initial "if".

        while True:
            self.pos += 1          # Consume "peek".
            self.pos += 1          # Consume "=".

            ch = self.current_value()[0]  # Character literal content.
            self.pos += 1          # Consume string literal.

            self.pos += 1          # Consume "then".
            self.pos += 1          # Consume "begin".
            self.pos += 1          # Consume "skip".
            self.pos += 1          # Consume ";".

            inner = self.parse_segments() # Parse nested branch body.

            self.pos += 1          # Consume "end".

            branches.append((ch, inner))

            if self.at_word("else"):
                self.pos += 1      # Consume "else".

                if self.at_word("if"):
                    self.pos += 1  # Consume "if".
                    continue       # Parse next branch.

                # Otherwise this is Else Error ;
                self.pos += 1      # Consume "error".
                self.pos += 1      # Consume ";".
                else_error = True
                break

            # No Else, so the End is followed by semicolon.
            self.pos += 1          # Consume ";".
            break

        return ("if", branches, else_error)

    def find_routines(self):
        """
        Scan the token list and parse every real parsing routine definition.

        A real definition has the token pattern:
            procedure name ; begin

        Forward declarations have:
            procedure name ; forward

        Internal procedures Skip and Error are ignored.
        """
        routines = []
        self.pos = 0

        while self.pos < len(self.tokens):
            is_definition = (
                self.tokens[self.pos][0] == WORD
                and self.tokens[self.pos][1] == "procedure"
                and self.pos + 3 < len(self.tokens)
                and self.tokens[self.pos + 1][0] == WORD
                and self.tokens[self.pos + 3][0] == WORD
                and self.tokens[self.pos + 3][1] == "begin"
                and self.tokens[self.pos + 1][1] not in ("skip", "error")
            )

            if is_definition:
                name = self.tokens[self.pos + 1][1]
                self.pos += 4       # Consume procedure name ; begin.

                body = self.parse_segments()

                self.pos += 1       # Consume end.
                self.pos += 1       # Consume semicolon.

                routines.append((name, body))
            else:
                self.pos += 1

        return routines


def expand_segment(segment):
    """
    Expand one segment into a list of possible productions.

    A production is represented as a list of symbols:
    - terminal:    ("terminal", character)
    - nonterminal: ("nonterminal", name)
    """
    kind = segment[0]

    # Unconditional call contributes one nonterminal.
    if kind == "call":
        name = segment[1]
        return [[("nonterminal", name)]]

    # Conditional segment.
    _, branches, else_error = segment
    result = []

    for ch, inner_segments in branches:
        terminal_symbol = ("terminal", ch)

        # Branch contributes the consumed character followed by its body.
        for inner_prod in expand_segments(inner_segments):
            result.append([terminal_symbol] + inner_prod)

    # If there is no Else Error, doing nothing is also successful.
    if not else_error:
        result.append([])

    return result


def expand_segments(segments):
    """
    Expand a sequence of segments.

    Since segments are executed sequentially, alternatives are combined
    by Cartesian product and concatenation.
    """
    result = [[]]                  # One empty prefix initially.

    for segment in segments:
        alternatives = expand_segment(segment)
        new_result = []

        for prefix in result:
            for suffix in alternatives:
                new_result.append(prefix + suffix)

        result = new_result

    return result


def render_production(prod):
    """
    Convert a production into required BNF text.

    Adjacent terminal characters are merged into one quoted literal.
    """
    out = []
    i = 0

    while i < len(prod):
        typ, value = prod[i]

        if typ == "terminal":
            chars = []

            while i < len(prod) and prod[i][0] == "terminal":
                chars.append(prod[i][1])
                i += 1

            out.append("'" + "".join(chars) + "'")
        else:
            out.append("<" + value + ">")
            i += 1

    return "".join(out)


def solve():
    src = sys.stdin.read()             # Read entire Pascal program.
    tokens = tokenize(src)             # Tokenize it.

    extractor = ParserExtractor(tokens)
    routines = extractor.find_routines()

    lines = []

    for name, body in routines:
        alternatives = []

        for prod in expand_segments(body):
            alternatives.append(render_production(prod))

        alternatives.sort()

        line = "<" + name + ">::=" + "|".join(alternatives)
        lines.append(line)

    lines.sort()

    sys.stdout.write("\n".join(lines) + "\n")


if __name__ == "__main__":
    solve()
