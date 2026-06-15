# Example traces

Raw model outputs from sgu-bench runs, kept so you can see exactly what was
generated and graded.

## Pilot

`gpt-5-mini/` holds the three pilot problems (p100, p123, p172) produced by
`examples/openai_solver.py` at low effort. Each `pNNN.md` is the full reply and
`pNNN.cpp` is the extracted program. All three were accepted.

## Paper runs

The remaining folders are the full runs behind the SGU-Editorial paper: seven
recent models over all 452 problems, one sample per problem, at the provider's
**low** reasoning effort. For each model, `pNNN.md` is the full reply,
`pNNN.cpp` is the extracted program that was compiled and run, and
`verdicts.json` holds the per-problem and per-test verdicts from the sandbox.
A missing `pNNN.cpp` means the reply had no extractable code block (the model
ran out of output budget before emitting a program).

Accepted rate over the 452 problems (low effort, pass@1):

| Model (low effort)     | AC%  | Accepted |
|------------------------|-----:|---------:|
| GPT-5.5                | 75.7 |  342/452 |
| Qwen 3.7 Plus          | 69.7 |  315/452 |
| Claude Opus 4.8        | 65.9 |  298/452 |
| Gemma 4                | 57.3 |  259/452 |
| Claude Sonnet 4.6      | 42.7 |  193/452 |
| Qwen3 Coder Next       | 36.1 |  163/452 |
| Claude Haiku 4.5       | 21.5 |   97/452 |

These are single-sample, low-effort numbers and are meant as a reference point,
not a definitive ranking. Re-run any of them with `examples/openai_solver.py` or
`examples/anthropic_solver.py`.
