# ASS Labs — Linux Shell and C Programs

Lab work for Advanced System Software: Linux shell commands, shell scripts, and C programs that reimplement simple UNIX utilities.

Each lab has a `ques.txt` with the questions. Every question lives in its own folder (`q1/`, `q2/`, …) with:

- `sample.sh` or `sample.c` — the solution
- any extra input files needed for that question

## Labs

### Lab 1 — Linux shell commands and scripts

- `q1/` — Common Linux commands (`who`, `ls`, `grep`, `chmod`, …)
- `q2/` — List all `.c` files in a given directory
- `q3/` — Print the current date/time and number of logged-in users
- `q4/` — Print lines in a file that contain the word `manipal`
- `q5/` — Print a range of lines from a file

### Lab 2 — Shell scripts

- `q1/` — Check that a string has at least 10 characters (`case` and `expr`)
- `q2/` — Delete files in directory `d2` whose names also exist in `d1`
- `q3/` — Rename argument files to uppercase if the new name is free
- `q4/` — Formatted listing: permissions, size, name, mtime, atime
- `q5/` — Factorial of `n`

### Lab 3 — C programs (stdio and UNIX utilities)

- `q1/` — Simple `grep`: print lines that contain a given word
- `q2/` — Simple `more`: page output every 20 lines
- `q3/` — `printf` conversion specifiers (`%d`, `%x`, `%f`, `%s`, …)
- `q4/` — Character-by-character file copy with `getc` / `putc`

### Lab 4 — File metadata and links

- `q1/` — Print the inode number of a file (`stat`)
- `q2/` — Print the complete `stat` structure of a file
- `q3/` — Create a hard link (`link`) then `unlink` it
- `q4/` — Create a soft link (`symlink`) then `unlink` it

## How to run

### Shell labs (Lab 1 and Lab 2)

```bash
cd lab2/q5
chmod +x sample.sh
echo 5 | ./sample.sh
```

### C labs (Lab 3 and Lab 4)

```bash
cd lab4/q1
gcc sample.c -o sample
./sample notes.txt
```

| Lab | Question | Example run |
| --- | --- | --- |
| 3 | Q1 | `./sample manipal data.txt` |
| 3 | Q2 | `./sample longfile.txt` |
| 3 | Q3 | `./sample` |
| 3 | Q4 | `./sample source.txt dest.txt` |
| 4 | Q1 | `./sample notes.txt` |
| 4 | Q2 | `./sample notes.txt` |
| 4 | Q3 | `./sample original.txt` |
| 4 | Q4 | `./sample original.txt` |

Lab 3 Q2 waits for Enter every 20 lines. To run it non-interactively:

```bash
printf '\n\n' | ./sample longfile.txt
```

## Notes

- Compiled binaries (`sample`, `*.o`, `a.out`) are gitignored. Rebuild them with `gcc` as shown above.
- Some scripts and programs read from stdin (`read` in shell, `getchar` in C). Pipe input or type it when prompted.
