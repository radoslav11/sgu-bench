import sys


# Read the whole input as lines.
lines = sys.stdin.read().splitlines()

# The first line contains N and M.
n, m = map(int, lines[0].split())

# The next N lines contain the grid.
grid = [lines[i + 1].strip() for i in range(n)]

# The remaining lines contain the B++ program.
# All whitespace in the program must be ignored.
program_text = "".join(lines[n + 1:])

# Remove every whitespace character from the program source.
source = "".join(ch for ch in program_text if not ch.isspace())

# Find the robot's starting position.
start_r = start_c = -1
for i in range(n):
    for j in range(m):
        if grid[i][j] == "R":
            start_r, start_c = i, j


# Function-name to integer-ID mapping.
func_id_of = {}

# Function bodies indexed by function ID.
# Each body is a list of parsed operators.
func_body = []

# Function formal parameters indexed by function ID.
func_params = []


def get_or_create_func(name):
    """
    Return the integer ID of a function.
    If this is the first time we see this function name, create storage for it.
    """
    if name in func_id_of:
        return func_id_of[name]

    fid = len(func_id_of)
    func_id_of[name] = fid
    func_body.append([])
    func_params.append([])
    return fid


# Current parser position in source.
pos = 0


def parse_arg_letters():
    """
    Parse an argument list.

    The current character must be '('.
    Examples:
        ()      -> []
        (X)     -> ['X']
        (X,Y)   -> ['X', 'Y']
        (L,C)   -> ['L', 'C'] for call arguments
    """
    global pos

    # Skip '('.
    pos += 1

    args = []

    # Read until ')'.
    while source[pos] != ")":
        # Commas are separators and ignored.
        if source[pos] != ",":
            args.append(source[pos])
        pos += 1

    # Skip ')'.
    pos += 1

    return args


def parse_block():
    """
    Parse a function body block.

    The current character must be '{'.

    Operators can be:
    - primitive commands L/R/C
    - uppercase argument references
    - lowercase function calls
    """
    global pos

    # Skip '{'.
    pos += 1

    ops = []

    # Parse until matching '}'.
    while source[pos] != "}":
        ch = source[pos]

        # Direct primitive command.
        if ch in "LRC":
            ops.append(("cmd", ch))
            pos += 1

        # Lowercase character means a function call.
        elif "a" <= ch <= "z":
            # Function name.
            name = ch
            pos += 1

            # Get called function ID.
            fid = get_or_create_func(name)

            # Parse actual arguments.
            args = parse_arg_letters()

            # Store call operator.
            ops.append(("call", fid, args))

        # Otherwise it is an uppercase argument reference.
        else:
            ops.append(("arg", ch))
            pos += 1

    # Skip '}'.
    pos += 1

    return ops


def parse_function_def():
    """
    Parse one full function definition:

        name(args){body}
    """
    global pos

    # Function name is one lowercase character.
    name = source[pos]
    pos += 1

    # Get this function's ID.
    fid = get_or_create_func(name)

    # Parse formal parameters.
    params = parse_arg_letters()

    # Parse body.
    body = parse_block()

    # Store parsed definition.
    func_params[fid] = params
    func_body[fid] = body


# Parse all function definitions in the source.
while pos < len(source):
    parse_function_def()


# Direction vectors: 0=up, 1=right, 2=down, 3=left.
dr = [-1, 0, 1, 0]
dc = [0, 1, 0, -1]

# Global total primitive-command count.
total_commands = 0

# Set of already reported starred cells.
visited = set()

# First-visit order of starred cells.
visit_order = []


def register_star(r, c):
    """
    If cell (r, c) is a starred cell not previously visited,
    record it in the answer.
    """
    if grid[r][c] == "*" and (r, c) not in visited:
        visited.add((r, c))
        visit_order.append((r, c))


def apply_command(cmd, r, c, d, star_log):
    """
    Execute one primitive robot command.

    Returns the new robot state:
        r, c, d

    star_log receives all starred cells stepped on during this execution.
    """
    global total_commands

    if cmd == "L":
        # Turn left.
        d = (d + 3) % 4

    elif cmd == "R":
        # Turn right.
        d = (d + 1) % 4

    else:
        # Command C: move forward.
        nr = r + dr[d]
        nc = c + dc[d]

        # Ignore movement if it leaves the grid.
        if 0 <= nr < n and 0 <= nc < m:
            r, c = nr, nc

            # If a star is entered, log it and register first visit.
            if grid[r][c] == "*":
                star_log.append((r, c))
                register_star(r, c)

    # Every primitive command counts, including blocked movement.
    total_commands += 1

    return r, c, d


# Memoization dictionary.
# Key:
#   (function id, argument bindings tuple, start row, start col, start direction)
#
# Value:
#   (end row, end col, end direction, command count, tuple of star steps)
cache = {}


def execute_function(fid, bindings, r, c, d):
    """
    Execute function fid with concrete argument bindings.

    bindings is a tuple/list containing actual command values for parameters,
    each being one of 'L', 'R', or 'C'.
    """
    global total_commands

    bindings = tuple(bindings)

    # Build cache key from function identity, arguments, and starting state.
    key = (fid, bindings, r, c, d)

    # If this exact call was executed before, reuse it.
    if key in cache:
        end_r, end_c, end_d, cmd_count, stars = cache[key]

        # Replay star visits in order.
        for sr, sc in stars:
            register_star(sr, sc)

        # Add skipped primitive commands to global counter.
        total_commands += cmd_count

        return end_r, end_c, end_d, cmd_count, stars

    # Execute body normally.
    params = func_params[fid]
    ops = func_body[fid]

    # Map parameter names to actual command values.
    env = dict(zip(params, bindings))

    # Local command counter at call start.
    before_commands = total_commands

    # All star steps made during this function call.
    stars = []

    # Execute each operator in the function body.
    for op in ops:
        kind = op[0]

        if kind == "cmd":
            # Direct command L/R/C.
            cmd = op[1]
            r, c, d = apply_command(cmd, r, c, d, stars)

        elif kind == "arg":
            # Argument reference, resolved to L/R/C.
            arg_name = op[1]
            cmd = env[arg_name]
            r, c, d = apply_command(cmd, r, c, d, stars)

        else:
            # Function call.
            _, child_fid, call_args = op

            # Resolve actual arguments for child function.
            child_bindings = []

            for a in call_args:
                if a in "LRC":
                    # Literal command argument.
                    child_bindings.append(a)
                else:
                    # Caller argument; substitute its actual value.
                    child_bindings.append(env[a])

            # Execute child function from current robot state.
            child_r, child_c, child_d, child_count, child_stars = execute_function(
                child_fid,
                child_bindings,
                r,
                c,
                d,
            )

            # Update current robot state.
            r, c, d = child_r, child_c, child_d

            # Append all child star steps to this call's log.
            stars.extend(child_stars)

    # Number of primitive commands executed by this function call.
    cmd_count = total_commands - before_commands

    # Store immutable star list in cache.
    result = (r, c, d, cmd_count, tuple(stars))
    cache[key] = result

    return result


# Entry point is m().
main_id = get_or_create_func("m")

# Execute from starting position, initially facing up.
execute_function(main_id, (), start_r, start_c, 0)

# Output result.
out = [str(len(visit_order))]

for r, c in visit_order:
    # Convert from zero-based to one-based coordinates.
    out.append(f"{r + 1} {c + 1}")

print("\n".join(out))
