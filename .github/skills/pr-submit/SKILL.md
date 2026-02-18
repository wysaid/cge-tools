---
name: pr-submit
description: Create or update a GitHub Pull Request after completing code changes for cge-tools
---

## When to use

- Code changes are complete, tested, and ready for review
- Need to create a new PR or update an existing one with new commits

## Constraints

- **ALWAYS** set `PAGER=cat` before calling `gh` to avoid pagination issues
- **ALWAYS** run `./tasks.sh --release --rebuild` before submitting to verify build
- **NEVER** commit directly to `master`

## Procedure

1. **Verify Changes**:
   - Review all changes for completeness and correctness
   - Ensure no debug code, temporary files, or unintended changes
   - Check that code follows `.clang-format` style (run `./format-all.sh`)
   - Verify no new compiler warnings

2. **Build Verification**:
   ```bash
   # Clean rebuild to ensure everything compiles
   ./tasks.sh --release --rebuild
   
   # Optional: Test examples
   cd examples/build && ./simple_filter
   ```

3. **Clean Workspace**:
   - Remove temporary files and debug artifacts
   - Ensure `.gitignore` is respected
   - Check `git status` for unintended changes

4. **Branch Management**:
   - If on `master`, create a feature branch:
     ```bash
     git checkout -b feat/descriptive-name
     # or fix/bug-description
     # or refactor/improvement
     # or docs/update-section
     ```
   - Ensure branch name doesn't conflict with existing branches

5. **Commit Changes**:
   - Stage changes: `git add <files>`
   - Commit with conventional format:
     ```bash
     git commit -m "feat: add vignette filter support"
     # or
     git commit -m "fix: correct brightness calculation
     
     - Fixed off-by-one error in brightness shader
     - Added test case for edge values
     - Updated documentation"
     ```
   - Push to remote: `git push origin <branch-name>`

6. **Create or Update PR**:
   - Set pagination: `export PAGER=cat`
   - Check for existing PR on current branch:
     ```bash
     gh pr view --json number,title,body
     ```
   
   - **If PR exists**:
     - Fetch current title and description
     - Note the new commits
     - Update PR with comment about new changes:
       ```bash
       gh pr comment --body "Updated with new commits:
       - [commit message 1]
       - [commit message 2]"
       ```
   
   - **If PR doesn't exist**:
     - Create draft title from commit messages
     - Create comprehensive description including:
       * What changed (summary)
       * Why (motivation/problem being solved)
       * How (technical approach)
       * Testing performed
       * Related issues (if any)
     - Create PR:
       ```bash
       gh pr create --title "feat: add vignette filter" \
         --body "..." \
         --base master
       ```

## PR Title Format

Follow conventional commit format:
- `feat: add new feature`
- `fix: correct bug in component`
- `refactor: improve code structure`
- `perf: optimize texture processing`
- `docs: update build instructions`
- `style: format code with clang-format`
- `test: add filter tests`
- `chore: update dependencies`

## PR Description Template

```markdown
## Summary
Brief description of changes (1-2 sentences)

## Motivation
Why is this change needed? What problem does it solve?

## Changes
- Specific change 1
- Specific change 2
- Specific change 3

## Testing
- [ ] Compiled successfully (Debug and Release)
- [ ] Ran `./format-all.sh`
- [ ] No new compiler warnings
- [ ] Tested with example programs
- [ ] Verified on [platform(s)]

## Related Issues
Fixes #123 (if applicable)

## Screenshots/Video
(if applicable for visual changes)
```

## Output

When reporting back to the user, include:
- Branch name and commit count
- PR URL (if created/updated)
- Build verification results
- Key changes summary
- Any issues encountered

## Example Commands

```bash
# Check if PR exists
export PAGER=cat
gh pr view

# Create new PR
gh pr create --title "feat: add gaussian blur filter" \
  --body "Implements configurable gaussian blur with sigma parameter..." \
  --base master

# Update PR with comment
gh pr comment 42 --body "Updated with performance optimizations"

# Check PR status
gh pr status

# View PR details
gh pr view 42
```

## Common Issues

**Issue**: `gh` not installed
**Solution**: Install GitHub CLI: `brew install gh` (macOS) or see https://cli.github.com/

**Issue**: Not authenticated with GitHub
**Solution**: Run `gh auth login`

**Issue**: PR creation fails due to conflicts
**Solution**: Rebase on latest master: `git fetch origin && git rebase origin/master`

## Best Practices

- Keep PRs focused on a single feature/fix
- Write clear, descriptive commit messages
- Test thoroughly before submitting
- Update documentation if API changes
- Respond to review comments promptly
- Keep PR description up-to-date if scope changes
