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
- `q6/` — Merge two sorted numeric files, drop duplicates
- `q7/` — Quadratic equation roots using `case`

### Lab 3 — C programs (stdio and UNIX utilities)

- `q1/` — Simple `grep`: print lines that contain a given word
- `q2/` — Simple `more`: page output every 20 lines
- `q3/` — `printf` conversion specifiers (`%d`, `%x`, `%f`, `%s`, …)
- `q4/` — Character-by-character file copy with `getc` / `putc`
- `q5/` — Grant others read permission on your `.c` files
- `q6/` — Copy initial / middle / last parts of a file with `lseek`

### Lab 4 — File metadata and links

- `q1/` — Print the inode number of a file (`stat`)
- `q2/` — Print the complete `stat` structure of a file
- `q3/` — Create a hard link (`link`) then `unlink` it
- `q4/` — Create a soft link (`symlink`) then `unlink` it
- `q5/` — Inode number of every file in a directory
- `q6/` — Full `stat` structure of every file in a directory

### Lab 5 — Process control (`fork`, `wait`, `exec`)

- `q1/` — Parent blocks with `wait()` until the child finishes
- `q2/` — Child loads Q1's binary with `execl`
- `q3/` — Print PID, parent PID, and child PID in both processes
- `q4/` — Zombie (defunct) child; parent sleeps without `wait`
- `q5/` — Orphan child; print parent PID before and after adoption
- `q6/` — `wait(&status)` and print the child's exit code

### Lab 6 — Multithreaded programming (Pthreads)

- `q1/` — Fibonacci sequence in a child thread; parent waits, then prints
- `q2/` — Sum of 0..n in a child thread; result returned to main
- `q3/` — Primes from a start number to an end number
- `q4/` — Even-sum and odd-sum threads over an array; parent joins both
- `q5/` — Matrix multiplication (one thread per result row)
- `q6/` — Row sums and column sums in two threads

## How to run

### Shell labs (Lab 1 and Lab 2)

```bash
cd lab2/q5
chmod +x sample.sh
echo 5 | ./sample.sh
```

### C labs (Lab 3–Lab 6)

```bash
cd lab6/q1
gcc -pthread sample.c -o sample
./sample 8
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
| 5 | Q1 | `gcc sample.c -o sample.out && ./sample.out` |
| 5 | Q2 | Compile Q1 first, then `./sample.out ../q1/sample.out` |
| 5 | Q4 | `./sample.out &` then `ps -l` to see the zombie |
| 6 | Q1 | `gcc -pthread sample.c -o sample && ./sample 8` |
| 6 | Q2 | `./sample 10` |
| 6 | Q3 | `./sample 10 40` |
| 6 | Q4 | `./sample` |
| 6 | Q5 | `./sample` |
| 6 | Q6 | `./sample` |

Lab 3 Q2 waits for Enter every 20 lines. To run it non-interactively:

```bash
printf '\n\n' | ./sample longfile.txt
```

## Notes

- Extra question folders (`lab2/q6`–`q7`, `q5`/`q6` in labs 3–6) are gitignored. They stay on this machine and on the download site.
- Compiled binaries (`sample`, `*.o`, `a.out`) are gitignored. Rebuild them with `gcc` as shown above.
- Some scripts and programs read from stdin (`read` in shell, `getchar` in C). Pipe input or type it when prompted.
