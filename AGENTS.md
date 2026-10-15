# Repository Guidelines

## Project Structure & Module Organization

This repository is an offline Chinese embedded-software interview guide with 15 chapters and 120 questions.

- `index.html`: browser entry point; `开始学习.cmd` opens it on Windows.
- `assets/app.js`: rendering, navigation, and progress persistence; `engine.js`: validation and filtering; `data.js`: question bank and downloadable exercise sources; `style.css`: responsive and print styles.
- `01_分章讲义/`: standalone Markdown chapters.
- `02_面试准备/`: revision plans, project templates, exercise instructions, and official references.
- `03_代码练习/`: eight independent C99 programs; each includes `main` and assertion tests.
- Root `.txt` files document usage and verification; `文件校验_SHA256.csv` records file sizes and hashes.

## Build, Test, and Development Commands

No dependency installation or build step is required. Open `index.html` directly in modern Chrome or Edge.

From the repository root:

```sh
# Compile and run one exercise; repeat for each changed C file.
cc -std=c99 -Wall -Wextra -Werror 03_代码练习/01_ring.c -o ../ring_test
../ring_test

# Check JavaScript syntax when Node.js is available.
node --check assets/app.js
node --check assets/engine.js
node --check assets/data.js
```

Compile exercises separately because each defines `main`. Keep executables outside the repository.

## Coding Style & Naming Conventions

Use UTF-8 and preserve Chinese instructional prose. Follow surrounding formatting: JavaScript/CSS are compact; C implementation blocks generally use four-space indentation. No formatter or linter configuration is supplied. Use descriptive `snake_case` C functions and existing JavaScript naming patterns. Preserve numbered filenames and stable question IDs such as `c-01`, which identify saved progress.

## Testing Guidelines

C tests use standard `assert`; keep assertions enabled and cover boundaries, wraparound, invalid input, and failure semantics. No standalone test suite or coverage threshold is included. For UI changes, check offline loading, search/filter combinations, notes, bookmarks, progress export/import, storage conflicts, mobile layout, and printing. Report actual checks and environment; host tests do not establish MCU correctness.

## Content Consistency

Keep `assets/data.js`, chapter Markdown, reference indexes, and exercise instructions consistent. When changing C exercises, update their embedded download sources in `data.js`. Refresh existing checksum entries for changed listed files. Preserve offline operation and progress-import validation.

## Commit & Pull Request Guidelines

The repository began as an imported project, so there is no established commit history. Use concise imperative subjects, optionally prefixed with `docs:`, `fix:`, or `feat:`. PRs should describe affected chapters/features, link relevant issues, state validation performed, and include screenshots for visual changes.
