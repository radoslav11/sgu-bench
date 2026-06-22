# sgu-bench

An offline, **verifiable** benchmark of **452 ACM SGU** competitive-programming
problems for evaluating LLMs and coding agents. You write one function —
`statement -> source code` — and sgu-bench compiles it, runs it in a
resource-limited sandbox over hidden tests, and returns a real verdict
(AC / WA / TLE / MLE / RE / CE) per problem.

Unlike text-similarity or LLM-as-judge evals, grading here is deterministic
and exact: every problem's expected outputs come from a reference solution
that is **accepted on the real judge**, and the tests include mandatory
maximal-size cases so time/memory limits actually bite.

The problem statements and reference solutions come from the
[acm-sgu](https://github.com/radoslav11/acm-sgu) dataset.

## Install

```bash
pip install sgu-bench          # add [openai] or [anthropic] for the examples
```

That's it. The benchmark data (~310 MB) downloads automatically the first time
you use it, into `~/.cache/sgu-bench` (override with `$SGU_BENCH_HOME`). You can
also pre-fetch explicitly with `python -m sgu_bench fetch`.

You need a C++ compiler (`g++` or `clang++`) on PATH. The sandbox is POSIX-only
(Linux/macOS).

## Connect your inference

```python
from sgu_bench import Benchmark

bench = Benchmark()

def solver(statement: str) -> str:      # <- your model / agent
    return my_model_generate_cpp(statement)

report = bench.evaluate(solver, problems="all")   # or ["p100", "p172", ...]
print(report)                                     # solved N/452 + verdict breakdown
```

That's the whole integration surface. `solver` is any callable returning C++
(or pass `language="py"`). For agentic, feedback-driven solving (retry on the
verdict), drive the loop yourself with `bench.evaluate_source(pid, source)` —
see [`examples/agent_solver.py`](examples/agent_solver.py).

Ready-made adapters: [`examples/openai_solver.py`](examples/openai_solver.py),
[`examples/anthropic_solver.py`](examples/anthropic_solver.py). Sample runs are
in [`examples/traces/`](examples/traces/).

## CLI

```bash
python -m sgu_bench fetch              # download/unpack the hidden tests
python -m sgu_bench list               # problem count + whether tests are present
python -m sgu_bench run p172 my.cpp    # grade one solution file
```

## How grading works

- Each `problems/pNNN/` has `config.json` (limits, checker kind, seeds), the
  `statement.txt`, the reference solution, a `gen.py`, and (for "output any
  valid answer" problems) a `checker.py`. The hidden `tests/` are fetched.
- **Checkers:** `diff` (token-exact), `float:<eps>` (numeric tolerance), or
  `custom` (a per-problem special judge for problems with multiple valid
  answers — ~157 of them).
- **Sandbox:** each run is capped by the problem's time/memory limits
  (`rlimit` + peak-RSS check). Old SGU time limits target 2002 hardware, so
  the harness scales them up; Python submissions get extra head-room.

## Rebuilding tests (maintainers)

Tests are generated, not scraped, so they can be rebuilt from the bundled
generators + reference solutions:

```bash
python -m sgu_bench build p172   # gen.py -> .in, reference -> .ans (+ oracle cross-check)
```

## Caveats

- macOS rounds peak RSS in 16 KB pages, so problems with very tight (4 MB)
  limits can read slightly high; the judge adds a small memory head-room.
- The benchmark only contains inputs/outputs, not the original judge's secret
  tests; strength comes from constraint-maximal generated cases.

## License

MIT.
