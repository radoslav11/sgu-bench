"""CLI: python -m sgu_bench <command>

    fetch                  download + unpack the hidden test data
    list                   list problems and whether tests are present
    run pNNN sol.cpp       grade a solution file against one problem
    build pNNN             (maintainer) regenerate a problem's tests locally
"""

import sys
from pathlib import Path

from . import paths
from .benchmark import Benchmark
from .fetch import fetch_tests


def main():
    if len(sys.argv) < 2:
        print(__doc__)
        return 1
    cmd = sys.argv[1]

    if cmd == "fetch":
        fetch_tests(force="--force" in sys.argv)
        return 0

    if cmd == "list":
        bench = Benchmark()
        present = paths.tests_present()
        print(f"{len(bench.problem_ids())} problems; "
              f"tests {'present' if present else 'NOT fetched (run: fetch)'}")
        return 0

    if cmd == "run":
        pid, sol = sys.argv[2], Path(sys.argv[3])
        bench = Benchmark()
        lang = "py" if sol.suffix == ".py" else "cpp"
        result = bench.evaluate_source(pid, sol.read_text(), lang)
        for tv in result.tests:
            detail = f"  {tv.detail}" if tv.detail else ""
            print(f"  {tv.test_name}: {tv.verdict} ({tv.wall_time:.2f}s){detail}")
        if result.verdict == "CE":
            print(result.compile_log)
        print(f"{pid}: {result.verdict}")
        return 0 if result.passed else 1

    if cmd == "build":
        from .build import build_tests

        pid = sys.argv[2]
        n = build_tests(paths.problems_dir() / pid, paths.data_root())
        print(f"built {n} tests for {pid}")
        return 0

    print(__doc__)
    return 1


if __name__ == "__main__":
    sys.exit(main())
