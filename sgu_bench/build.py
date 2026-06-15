"""Build a problem's test data: gen.py -> .in, reference solution -> .ans.

A problem package looks like:
    problems/pNNN/
        config.json   id, limits, checker, seeds, reference path
        gen.py        prints one test on stdout given a seed argv
        checker.py    only for checker == "custom"
        tests/        built .in/.ans pairs (output of this module)

The reference solution is the repo's accepted submission, so its outputs
are trusted. When config lists a second oracle (the editorial's Python
solution), both are run on every generated test and a mismatch fails the
build — a wrong generator that violates constraints usually breaks the
two implementations differently, so agreement is a cheap correctness
signal for the test data itself.
"""

import json
import subprocess
import sys
import tempfile
from pathlib import Path

from .judge import check_output, compile_submission
from .sandbox import RunStatus, run_limited

BUILD_TIME_LIMIT = 30.0  # generous: build correctness, not speed, matters
BUILD_MEMORY_KB = 1 << 21


def build_tests(problem_dir: Path, repo_root: Path, verbose: bool = True) -> int:
    config = json.loads((problem_dir / "config.json").read_text())
    tests_dir = problem_dir / "tests"
    tests_dir.mkdir(exist_ok=True)

    workdir = Path(tempfile.mkdtemp(prefix=f"sgu_build_{config['id']}_"))
    reference = repo_root / config["reference"]
    ref_cmd = compile_submission(reference, workdir)
    if ref_cmd is None:
        raise RuntimeError(f"reference failed to compile, see {workdir}")

    oracle_cmd = None
    if config.get("second_oracle"):
        oracle_dir = Path(tempfile.mkdtemp(prefix="sgu_oracle_"))
        oracle_cmd = compile_submission(
            repo_root / config["second_oracle"], oracle_dir
        )

    built = 0
    for seed in config["seeds"]:
        name = f"{seed:03d}"
        in_file = tests_dir / f"{name}.in"
        ans_file = tests_dir / f"{name}.ans"

        gen = subprocess.run(
            [sys.executable, str(problem_dir / "gen.py"), str(seed)],
            capture_output=True,
            text=True,
            timeout=120,
        )
        if gen.returncode != 0:
            raise RuntimeError(f"gen.py failed on seed {seed}: {gen.stderr}")
        in_file.write_text(gen.stdout)

        run = run_limited(
            ref_cmd, in_file, ans_file, BUILD_TIME_LIMIT, BUILD_MEMORY_KB
        )
        if run.status is not RunStatus.OK:
            raise RuntimeError(
                f"reference {run.status.value} on seed {seed} ({in_file})"
            )

        if oracle_cmd is not None:
            oracle_out = workdir / f"{name}.oracle"
            run2 = run_limited(
                oracle_cmd, in_file, oracle_out, BUILD_TIME_LIMIT, BUILD_MEMORY_KB
            )
            if run2.status is not RunStatus.OK:
                raise RuntimeError(
                    f"second oracle {run2.status.value} on seed {seed}"
                )
            ok, detail = check_output(
                config["checker"], problem_dir, in_file, oracle_out, ans_file
            )
            if not ok:
                raise RuntimeError(
                    f"oracle disagreement on seed {seed}: {detail}"
                )

        built += 1
        if verbose:
            size = in_file.stat().st_size
            print(
                f"  {name}.in ({size} B) -> .ans  "
                f"ref {run.wall_time:.2f}s"
                + ("  [oracle ok]" if oracle_cmd else "")
            )

    return built
