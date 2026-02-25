---
name: pr-review
description: Address review comments and CI failures for the current branch's PR
---

## Constraints

- Prepend `GH_PAGER=` to every `gh` command (bash/zsh), or set `$env:GH_PAGER=""` in PowerShell — never modify global config
- Key commands: `gh pr list --head <BRANCH>` · `gh pr view <PR> --comments` · `gh pr checks <PR>` · `gh run view <RUN_ID> --log-failed`
- Workflow-only fixes (YAML syntax, missing steps, bad paths) **cannot be verified locally** — must rely on remote re-run
- Code fixes **should be verified locally first** whenever the local environment matches the failing CI platform (see Local Verification below)

### Review Restraint Policy

Before acting on any review comment or suggestion, classify it by importance:

| Level | Criteria | Action |
|-------|----------|--------|
| **P1 — Must Fix** | Bug, security issue, broken behavior, API contract violation, CI failure | Fix immediately |
| **P2 — Should Fix** | Correctness risk, meaningful maintainability improvement, clear code smell with real impact | Fix with brief justification |
| **P3 — Optional** | Style preference, minor naming nitpick, debatable design choice, "could be cleaner" | **Do NOT auto-fix** — surface to user for decision |
| **P4 — Reject** | Contradicts project conventions, introduces unnecessary complexity, or is factually wrong | Reject with explanation |

**Rules:**
- **Do not implement P3 suggestions automatically.** Note them in the final summary and let the user decide.
- **Do not add comments to code** unless the comment explains non-obvious logic that is truly necessary. Never add comments just to acknowledge a review suggestion was applied.
- When in doubt about importance, prefer the lower severity (P3/P4) and defer to the user rather than making the change.

### Local Verification

Before pushing code fixes, verify locally when possible (`uname -s` / `uname -m`) to avoid unnecessary remote round-trips.

**CI platform map:**

| CI job | OS family | Arch |
|--------|-----------|------|
| macOS arm64 | Darwin | arm64 |
| macOS x86_64 | Darwin | x86_64 |
| Linux (Ubuntu / Fedora) | Linux | x86_64 |
| Windows x64 | Windows | x86_64 |

**When to verify locally:**
- OS matches + **build / configure / test** failure → **verify locally**
- OS matches + **packaging / deploy** failure (macdeployqt, linuxdeploy, windeployqt) → **build only** (packaging depends on CI tooling)
- OS mismatch or pure **workflow issue** → **skip**, rely on remote

**Local commands** (via `tasks.sh`):

| Failure stage | Command |
|---------------|---------|
| Configure | `bash -l tasks.sh --release --config` |
| Build | `bash -l tasks.sh --release --rebuild` |
| Test | `bash -l tasks.sh --release --rebuild && bash -l tasks.sh --release --test` |
| Format / lint | `bash -l tasks.sh --format` then check for diffs |

Local pass → proceed to push. Local fail → iterate until fixed. Inconclusive (e.g. toolchain mismatch) → note in summary, proceed.

## Procedure

1. **Locate PR** — get PR number for current branch
2. **Fix CI failures** — for each failing check:
   - Fetch logs: `gh run view <RUN_ID> --log-failed`
   - Identify the failure category and the CI platform (OS + arch)
   - **Workflow issue** (wrong config, missing step, bad path):
     1. Fix `.github/workflows/` directly
     2. Commit & push the workflow change
     3. Wait for the re-triggered run to complete: poll with `GH_PAGER= gh pr checks <PR>` (or `GH_PAGER= gh run watch <RUN_ID>`) until the affected check finishes
     4. If it **passes** → continue to the next failing check
     5. If it **still fails** → fetch new logs (`gh run view <NEW_RUN_ID> --log-failed`) and repeat from step i
   - **Code issue, non-breaking**:
     1. Fix source code directly
     2. **Attempt local verification** (see Local Verification above) — run the appropriate `tasks.sh` command if local OS matches the failing CI platform
     3. If local verification passes or is skipped (platform mismatch), proceed; otherwise iterate on the fix
   - **Code issue, breaking change required**: stop and report to developer for a decision
3. **Address review comments** — apply the Review Restraint Policy to each comment:
   - P1/P2: implement the fix
   - P3: record in summary, do **not** modify code, ask user
   - P4: record rejection reason in summary
4. **Local pre-check** — if local OS matches any failing CI platform, run a final `bash -l tasks.sh --release --rebuild && bash -l tasks.sh --release --test` to catch cross-fix interactions. Skip if no platform matches.
5. **Commit & push** — single commit covering all non-workflow fixes (workflow fixes are pushed incrementally during step 2)
6. **Final verification** — confirm every check is green: `GH_PAGER= gh pr checks <PR>`

## Output

- Per CI failure: root cause, resolution (or escalation reason), and whether local verification was performed
- After all fixes: summary of check statuses confirming all green
- **Review summary table** — produced at the end of every review session:

```
| # | Source | Issue | Priority | Action | Local verified | Reason if skipped |
|---|--------|-------|----------|--------|----------------|-------------------|
| 1 | Reviewer @xxx | rename foo() | P3 | Not fixed | — | Style preference — deferred to user |
| 2 | CI: macOS arm64 | missing include | P1 | Fixed | Yes (Darwin arm64) | — |
```

All P3/P4 items that were **not** fixed must appear in this table with a clear reason, so the user can make an informed decision.
