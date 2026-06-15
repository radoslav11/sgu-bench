"""Where the benchmark data lives.

Resolution order for the data root (the dir that contains ``problems/``):
  1. ``$SGU_BENCH_DATA`` if set (maintainers point this at a repo checkout).
  2. the repo root, when running from a source checkout (problems/ present).
  3. otherwise a per-user cache dir (``~/.cache/sgu-bench``), where the data
     is auto-downloaded on first use.

The full problems/ tree (metadata + the ~300 MB of tests) is distributed as
a single GitHub Release tarball and fetched automatically; see fetch.py.
"""

import os
from pathlib import Path

_PACKAGE_DIR = Path(__file__).resolve().parent


def cache_dir() -> Path:
    base = (
        os.environ.get("SGU_BENCH_HOME")
        or os.environ.get("XDG_CACHE_HOME")
        or str(Path.home() / ".cache")
    )
    return Path(base).expanduser() / "sgu-bench"


def _is_checkout(root: Path) -> bool:
    return (root / "problems" / "p100" / "config.json").exists()


def data_root() -> Path:
    env = os.environ.get("SGU_BENCH_DATA")
    if env:
        return Path(env).expanduser().resolve()
    repo = _PACKAGE_DIR.parent
    if _is_checkout(repo):
        return repo
    return cache_dir()


def problems_dir() -> Path:
    return data_root() / "problems"


def metadata_present() -> bool:
    return (problems_dir() / "p100" / "config.json").exists()


def tests_present() -> bool:
    """True once the test data has been unpacked."""
    sample = problems_dir() / "p100" / "tests"
    return sample.is_dir() and any(sample.glob("*.in"))
