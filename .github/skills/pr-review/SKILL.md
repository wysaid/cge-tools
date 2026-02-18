---
name: pr-review
description: Review a GitHub Pull Request for cge-tools, providing constructive feedback and suggestions
---

## When to use

- Asked to review code changes in a pull request
- Need to provide feedback on implementation, style, or correctness
- Evaluating if a PR is ready to merge

## Constraints

- **ALWAYS** set `PAGER=cat` before calling `gh` to avoid pagination issues
- Focus on **constructive** feedback, not criticism
- Check **compatibility** with existing API contracts
- Verify **build system** changes don't break anything

## Review Checklist

### 1. Code Quality

**Correctness**:
- [ ] Logic is sound and handles edge cases
- [ ] No obvious bugs or incorrect assumptions
- [ ] Error handling is appropriate
- [ ] Resource cleanup is correct (GL resources, memory)

**Style**:
- [ ] Follows `.clang-format` configuration
- [ ] Uses consistent naming (CGE prefix, camelCase methods, m_ members)
- [ ] Code is in `namespace CGE {}` where appropriate
- [ ] Includes are organized (system, Qt, CGE)

**Performance**:
- [ ] No unnecessary texture uploads/downloads
- [ ] Efficient GL state management
- [ ] No performance regressions
- [ ] Memory usage is reasonable

### 2. API Compatibility

**Public API** (headers in `include/cge/`):
- [ ] No breaking changes to existing methods
- [ ] New methods follow existing patterns
- [ ] Backward compatibility maintained
- [ ] Default parameters for new optional features

**Filter Interface**:
- [ ] Inherits from `CGEImageFilterInterface`
- [ ] Implements required methods correctly
- [ ] Provides intensity/parameter adjustment
- [ ] Cleans up resources properly

### 3. OpenGL / Graphics

- [ ] ES 2.0 compatible (no ES 3.0+ features)
- [ ] Shaders use `CGE_SHADER_STRING()` macros
- [ ] Proper precision qualifiers (`highp`, `mediump`)
- [ ] GL errors are checked (at least in Debug)
- [ ] Resources released before context destruction
- [ ] Thread safety (all GL calls on GL thread)

### 4. Qt Integration

- [ ] Qt version checks where needed (`QT_VERSION >= QT_VERSION_CHECK(6,0,0)`)
- [ ] No deprecated Qt5 APIs
- [ ] Qt objects have proper ownership
- [ ] UI operations on main thread

### 5. Build System

- [ ] CMake changes are correct
- [ ] No hardcoded paths
- [ ] Source files auto-discovered (no manual listing needed)
- [ ] Cross-platform compatibility
- [ ] Doesn't break existing build configurations

### 6. Testing

- [ ] PR author confirmed testing
- [ ] Examples still work
- [ ] No new compiler warnings
- [ ] Builds on multiple platforms (ideally)

### 7. Documentation

- [ ] Public APIs are documented
- [ ] Complex logic has explanatory comments
- [ ] README/docs updated if needed
- [ ] Commit messages follow conventions

## Review Process

### 1. Fetch PR Information

```bash
export PAGER=cat

# View PR details
gh pr view <PR_NUMBER> --json number,title,body,author,state,files

# Get file changes
gh pr diff <PR_NUMBER>

# Check PR checks/CI status
gh pr checks <PR_NUMBER>
```

### 2. Analyze Changes

For each file changed:
- Understand the purpose and motivation
- Check for correctness and style
- Look for potential issues
- Consider edge cases and error conditions

### 3. Provide Feedback

**Structure of feedback**:

```markdown
## Overall Assessment
[High-level summary: good/needs work/excellent]

## Strengths
- What's done well
- Good practices observed

## Issues

### Critical (Must Fix)
- [ ] Issue 1: explanation and why it's critical
- [ ] Issue 2: ...

### Important (Should Fix)
- [ ] Issue 3: explanation and impact
- [ ] Issue 4: ...

### Minor (Nice to Have)
- [ ] Issue 5: suggestion for improvement
- [ ] Issue 6: ...

## Suggestions
- Improvement idea 1
- Alternative approach 2

## Questions
- Clarification needed on X
- Why was Y approach chosen over Z?

## Recommendation
[Approve / Request Changes / Comment]
```

### 4. Submit Review

```bash
# Add review comment
gh pr review <PR_NUMBER> --comment --body "..."

# Request changes
gh pr review <PR_NUMBER> --request-changes --body "..."

# Approve
gh pr review <PR_NUMBER> --approve --body "LGTM! ..."

# Add inline comment (if needed)
gh pr review <PR_NUMBER> --comment --body "..." \
  --body-file review.md
```

## Review Categories

### 🔴 Critical Issues (Block Merge)

- Breaking API changes without migration path
- Memory leaks or resource leaks
- Crashes or undefined behavior
- Security vulnerabilities
- Major performance regressions
- Breaks existing functionality

### 🟡 Important Issues (Should Fix)

- Code style violations
- Missing error handling
- Incomplete implementations
- Poor performance (but not regression)
- Missing documentation for public APIs
- Inconsistent with project patterns

### 🟢 Minor Issues (Optional)

- Nitpicks on naming or formatting
- Suggestions for better readability
- Future improvements
- Alternative approaches to consider

## Examples

### Good Feedback

✅ **Specific and actionable**:
```
In `src/filters/cgeVignetteFilter.cpp:45`, the vignette radius isn't 
clamped. Consider adding:

    m_radius = std::max(0.0f, std::min(1.0f, radius));

This prevents undefined behavior when users pass invalid values.
```

✅ **Explains the why**:
```
The shader uses `highp` precision for all calculations, which may 
cause performance issues on older mobile GPUs. Consider using 
`mediump` for intermediate calculations and `highp` only for 
final color output, matching the pattern in other filters.
```

✅ **Offers alternatives**:
```
Instead of loading the LUT texture on every frame, consider 
caching it similar to how `CGELookupFilter` handles it. This 
would improve performance significantly.
```

### Bad Feedback

❌ **Vague**:
```
This code looks wrong.
```

❌ **Not constructive**:
```
Why did you write it this way? This is terrible.
```

❌ **Nitpicking without value**:
```
I personally prefer tabs over spaces.
```

## Common Review Points

### Filters

- Shader correctness and precision
- Parameter validation and clamping
- Resource cleanup in destructor
- Intensity blending implementation
- Texture coordinate handling

### Core Library

- Thread safety in multi-threaded scenarios
- Proper use of Qt signal/slot mechanisms
- OpenGL state management
- Memory management and ownership

### Build System

- CMake best practices
- Cross-platform compatibility
- No hardcoded paths
- Proper dependency handling

### Documentation

- Public API documentation
- Complex algorithm explanations
- Updated README if needed
- Changelog updates for releases

## Output Format

When summarizing the review for the user:

```
## PR Review Summary

**PR #XX**: [Title]
**Author**: [Author]
**Status**: [Open/Closed/Merged]

### Assessment: [Approve / Changes Needed / Comments]

**Strengths**:
- Point 1
- Point 2

**Issues Found**:
- 🔴 Critical: Issue 1
- 🟡 Important: Issue 2
- 🟢 Minor: Issue 3

**Recommendation**: [Detailed recommendation]

**Review submitted**: [Yes/No - if submitted via gh]
```

## Best Practices

- **Be kind**: Remember there's a person behind the code
- **Be specific**: Point to exact lines and explain clearly
- **Be helpful**: Suggest solutions, not just problems
- **Be balanced**: Acknowledge good work alongside issues
- **Be timely**: Review promptly to keep momentum
- **Be thorough**: Check all aspects, not just obvious issues

## Related

- See `.github/CONTRIBUTING.md` for contribution guidelines
- See `.github/instructions/code-conventions.instructions.md` for style rules
- See `.github/copilot-instructions.md` for project conventions
