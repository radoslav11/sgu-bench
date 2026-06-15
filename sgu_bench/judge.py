"""Compile a submission, run it over a problem's tests, and grade it.

Checker selection lives in the problem's config.json:
  "checker": "diff"          token-by-token comparison (whitespace-insensitive)
  "checker": "float:1e-6"    numeric comparison with absolute/relative eps
  "checker": "custom"        problems/pNNN/checker.py <in> <out> <ans>,
                             exit 0 = accept; used for "output any of them"
"""

import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional

from .sandbox import RunStatus, run_limited

PYTHON_TIME_SCALE = 3.0  # CPython gets extra headroom over the C++ limit
PYTHON_MEMORY_FLOOR_KB = 262144  # CPython baseline RSS alone is ~13 MB

# Absorb host RSS-measurement variance at tight limits: macOS samples peak
# RSS in 16 KB pages and rounds up harder than Linux's 4 KB, so a solution
# sitting right at a 4 MB limit can read a few hundred KB over and false-MLE.
# A small absolute pad fixes that without weakening detection of real
# blowups, which overshoot by tens to hundreds of MB.
MEMORY_HEADROOM_KB = 8192


@dataclass
class TestVerdict:
    test_name: str
    verdict: str  # AC / WA / TLE / MLE / RE
    wall_time: float = 0.0
    peak_rss_kb: int = 0
    detail: str = ""


@dataclass
class Evaluation:
    problem_id: str
    verdict: str  # overall: AC, or the first failing test's verdict / CE
    tests: List[TestVerdict] = field(default_factory=list)
    compile_log: str = ""

    @property
    def passed(self) -> bool:
        return self.verdict == "AC"


def compile_submission(source: Path, workdir: Path) -> Optional[List[str]]:
    """Return the run command, or None on compile error (log in workdir)."""
    if source.suffix == ".py":
        return [sys.executable, str(source)]

    binary = workdir / "submission"
    compiler = shutil.which("g++") or shutil.which("clang++")
    if compiler is None:
        raise RuntimeError("no C++ compiler found (need g++ or clang++)")

    result = subprocess.run(
        [compiler, "-O2", "-std=c++20", "-o", str(binary), str(source)],
        capture_output=True,
        text=True,
        timeout=60,
    )
    (workdir / "compile.log").write_text(result.stderr)
    return [str(binary)] if result.returncode == 0 else None


def evaluate(
    problem_dir: Path, source: Path, stop_on_failure: bool = True
) -> Evaluation:
    import json

    config = json.loads((problem_dir / "config.json").read_text())
    problem_id = config["id"]
    time_limit = config["time_limit"]
    memory_kb = config["memory_kb"]
    if source.suffix == ".py":
        time_limit *= PYTHON_TIME_SCALE
        memory_kb = max(memory_kb, PYTHON_MEMORY_FLOOR_KB)
    memory_kb += MEMORY_HEADROOM_KB

    workdir = Path(tempfile.mkdtemp(prefix=f"sgu_{problem_id}_"))
    cmd = compile_submission(source, workdir)
    if cmd is None:
        log = (workdir / "compile.log").read_text()
        return Evaluation(problem_id, "CE", compile_log=log)

    evaluation = Evaluation(problem_id, "AC")
    for in_file in sorted((problem_dir / "tests").glob("*.in")):
        if in_file.name.startswith("."):
            continue
        ans_file = in_file.with_suffix(".ans")
        out_file = workdir / (in_file.stem + ".out")
        run = run_limited(cmd, in_file, out_file, time_limit, memory_kb)

        if run.status is not RunStatus.OK:
            tv = TestVerdict(
                in_file.stem, run.status.value, run.wall_time, run.peak_rss_kb
            )
        else:
            ok, detail = check_output(
                config["checker"], problem_dir, in_file, out_file, ans_file
            )
            tv = TestVerdict(
                in_file.stem,
                "AC" if ok else "WA",
                run.wall_time,
                run.peak_rss_kb,
                detail,
            )

        evaluation.tests.append(tv)
        if tv.verdict != "AC":
            evaluation.verdict = tv.verdict
            if stop_on_failure:
                break

    return evaluation


def check_output(
    checker: str, problem_dir: Path, in_file: Path, out_file: Path, ans_file: Path
):
    if checker == "diff":
        out_tokens = out_file.read_text(errors="replace").split()
        ans_tokens = ans_file.read_text(errors="replace").split()
        if out_tokens == ans_tokens:
            return True, ""
        return False, _first_diff(out_tokens, ans_tokens)

    if checker.startswith("float:"):
        eps = float(checker.split(":", 1)[1])
        return _check_float(out_file, ans_file, eps)

    if checker == "custom":
        result = subprocess.run(
            [
                sys.executable,
                str(problem_dir / "checker.py"),
                str(in_file),
                str(out_file),
                str(ans_file),
            ],
            capture_output=True,
            text=True,
            timeout=60,
        )
        return result.returncode == 0, result.stdout.strip()

    raise ValueError(f"unknown checker: {checker}")


def _check_float(out_file: Path, ans_file: Path, eps: float):
    out_tokens = out_file.read_text(errors="replace").split()
    ans_tokens = ans_file.read_text(errors="replace").split()
    if len(out_tokens) != len(ans_tokens):
        return False, f"token count {len(out_tokens)} != {len(ans_tokens)}"

    for i, (got, exp) in enumerate(zip(out_tokens, ans_tokens)):
        try:
            g, e = float(got), float(exp)
        except ValueError:
            if got != exp:
                return False, f"token {i}: '{got}' != '{exp}'"
            continue

        if abs(g - e) > eps * max(1.0, abs(e)):
            return False, f"token {i}: {got} vs {exp} (eps {eps})"

    return True, ""


def _first_diff(out_tokens, ans_tokens) -> str:
    for i, (got, exp) in enumerate(zip(out_tokens, ans_tokens)):
        if got != exp:
            return f"token {i}: got '{got}', expected '{exp}'"
    return f"token count: got {len(out_tokens)}, expected {len(ans_tokens)}"
