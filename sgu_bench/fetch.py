"""Download and unpack the benchmark data (auto, on first use).

The full problems/ tree (metadata + ~1.1 GB of tests, ~310 MB gzipped) is not
in git; it ships as a single GitHub Release tarball and is unpacked into the
data root the first time the benchmark is used. Override the source with
``$SGU_BENCH_TESTS_URL``.
"""

import os
import sys
import tarfile
import tempfile
import urllib.request
from pathlib import Path

from .paths import data_root, tests_present

DEFAULT_TESTS_URL = (
    "https://github.com/radoslav11/sgu-bench/releases/download/"
    "tests-v1/sgu-bench-tests-v1.tar.gz"
)


def tests_url() -> str:
    return os.environ.get("SGU_BENCH_TESTS_URL", DEFAULT_TESTS_URL)


def _progress(done, total):
    mb = done / 1048576
    if total and total > 0:
        sys.stderr.write(f"\r  downloading data: {done * 100 // total}% ({mb:.0f} MB)")
    else:
        sys.stderr.write(f"\r  downloading data: {mb:.0f} MB")
    sys.stderr.flush()


def fetch_tests(force: bool = False, url: str | None = None) -> Path:
    """Download the data tarball and extract it into the data root.

    No-op (returns immediately) if the data is already present and not forced.
    """
    root = data_root()
    if tests_present() and not force:
        return root / "problems"

    url = url or tests_url()
    root.mkdir(parents=True, exist_ok=True)
    print(
        f"sgu-bench: downloading benchmark data (~310 MB, one-time) into {root}",
        file=sys.stderr,
    )
    print(f"  source: {url}", file=sys.stderr)
    tmp_path = Path(tempfile.mkstemp(suffix=".tar.gz")[1])
    try:
        urllib.request.urlretrieve(
            url, tmp_path, reporthook=lambda b, bs, t: _progress(b * bs, t)
        )
        sys.stderr.write("\n")
        print("  extracting...", file=sys.stderr)
        with tarfile.open(tmp_path, "r:gz") as tar:
            tar.extractall(root)
    finally:
        tmp_path.unlink(missing_ok=True)

    if not tests_present():
        raise RuntimeError(
            "data did not unpack as expected; the tarball must contain "
            "problems/pNNN/tests/*.in (and the problem metadata)."
        )
    print(f"  ready: {root / 'problems'}", file=sys.stderr)
    return root / "problems"


def ensure_data():
    """Make sure the benchmark data is available, downloading if needed."""
    if not tests_present():
        fetch_tests()
