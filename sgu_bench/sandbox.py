"""Resource-limited execution of a single process.

POSIX-only. Limits are enforced with setrlimit in the child:
  - RLIMIT_CPU caps CPU seconds (SIGXCPU -> TLE),
  - RLIMIT_AS / RLIMIT_DATA cap memory (allocation failures -> MLE-ish RE),
  - RLIMIT_FSIZE caps output size,
  - a wall-clock timeout catches sleep/deadlock.

Memory enforcement is solid on Linux; on macOS RLIMIT_AS is only loosely
honoured, so MLE detection there is best-effort. Peak RSS is sampled via
os.wait4 rusage, which lets us report MLE even when the OS chose to kill
rather than fail an allocation.
"""

import math
import os
import resource
import signal
import subprocess
import time
from dataclasses import dataclass
from enum import Enum
from pathlib import Path
from typing import Optional, Sequence


class RunStatus(Enum):
    OK = "OK"
    TLE = "TLE"
    MLE = "MLE"
    RE = "RE"


@dataclass
class RunResult:
    status: RunStatus
    wall_time: float
    cpu_time: float
    peak_rss_kb: int
    exit_code: Optional[int]  # None when killed by the wall-clock timeout


def run_limited(
    cmd: Sequence[str],
    stdin_path: Path,
    stdout_path: Path,
    time_limit: float,
    memory_kb: int,
    cwd: Optional[Path] = None,
) -> RunResult:
    cpu_limit = math.ceil(time_limit) + 1
    mem_bytes = memory_kb * 1024
    output_cap = 256 * 1024 * 1024

    def apply_limits():
        resource.setrlimit(resource.RLIMIT_CPU, (cpu_limit, cpu_limit + 1))
        resource.setrlimit(resource.RLIMIT_FSIZE, (output_cap, output_cap))
        for rlim in (resource.RLIMIT_AS, resource.RLIMIT_DATA):
            try:
                resource.setrlimit(rlim, (mem_bytes, mem_bytes))
            except (ValueError, OSError):
                pass
        os.setsid()

    start = time.monotonic()
    with open(stdin_path, "rb") as fin, open(stdout_path, "wb") as fout:
        proc = subprocess.Popen(
            cmd,
            stdin=fin,
            stdout=fout,
            stderr=subprocess.DEVNULL,
            preexec_fn=apply_limits,
            cwd=cwd,
        )
        timed_out = False
        try:
            _, status, rusage = _wait4_with_timeout(
                proc, deadline=start + time_limit * 3 + 2
            )
        except TimeoutError:
            timed_out = True
            os.killpg(proc.pid, signal.SIGKILL)
            _, status, rusage = os.wait4(proc.pid, 0)

    wall = time.monotonic() - start
    cpu = rusage.ru_utime + rusage.ru_stime
    # ru_maxrss is bytes on macOS, kilobytes on Linux.
    rss_kb = rusage.ru_maxrss // (1024 if os.uname().sysname == "Darwin" else 1)

    if timed_out or cpu > time_limit or _killed_by(status, signal.SIGXCPU):
        return RunResult(RunStatus.TLE, wall, cpu, rss_kb, None)

    # Negative exit_code means death by that signal number.
    # macOS honours RLIMIT_AS/DATA only loosely, so over-allocation may
    # not fail inside the child; the post-hoc peak-RSS check catches it.
    exit_code = os.waitstatus_to_exitcode(status)
    if exit_code == 0:
        if rss_kb > memory_kb:
            return RunResult(RunStatus.MLE, wall, cpu, rss_kb, 0)
        return RunResult(RunStatus.OK, wall, cpu, rss_kb, 0)

    blown_memory = rss_kb > memory_kb or _killed_by(status, signal.SIGKILL)
    return RunResult(
        RunStatus.MLE if blown_memory else RunStatus.RE,
        wall,
        cpu,
        rss_kb,
        exit_code,
    )


def _wait4_with_timeout(proc, deadline):
    while True:
        pid, status, rusage = os.wait4(proc.pid, os.WNOHANG)
        if pid != 0:
            proc.returncode = 0  # silence Popen.__del__'s resource warning
            return pid, status, rusage
        if time.monotonic() > deadline:
            raise TimeoutError
        time.sleep(0.005)


def _killed_by(status, sig) -> bool:
    return os.WIFSIGNALED(status) and os.WTERMSIG(status) == sig
