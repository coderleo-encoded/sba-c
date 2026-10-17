# Inventory Management System (C)

A menu-driven, terminal-based inventory manager written in C. It tracks customer invoices and supplier records with full create, search, modify, and view (CRUD) support, stored in plain text files.

Originally a school project for a small electrical and mechanical sales business, then expanded by me to 1,776 lines as a self-directed way to learn C: structs, pointers, file I/O, string handling, and input validation.

## Features

- **Two record types:** customer invoices and supplier info
- **Create:** bulk entry of multiple records in one session
- **Search:** look up any record by its primary key
- **Modify:** edit a single field of a record; changes are applied via a temp-file swap (write a new file, delete the original, rename)
- **View:** print the whole table in a formatted layout
- **Login system:**
  - First-run password setup (8+ characters, no spaces, entered twice)
  - Boot counter saved to disk
  - Backup-code recovery
  - Forced password reset after 25 failed attempts
- **Input validation:**
  - Bounded `fgets` reads with input-buffer cleanup
  - ID format checks (`INV-XXXX`, `SUP-XXXX`)
  - Printable-ASCII filtering and text-only checks for names
  - Numeric checks for quantity and price
  - Duplicate primary-key detection
- **Cross-platform:** `#ifdef _WIN32` switches between Windows (`windows.h`, `Sleep`, `cls`) and Linux/macOS (`unistd.h`, `sleep`, `clear`)
- **Custom exit codes** for error handling:

| Code | Meaning |
|------|---------|
| 0 | No errors |
| -1 | File error |
| -2 | Record size limit exceeded |
| -3 | Duplicate record / empty file |
| -4 | Inventory file could not be modified |

## Build and Run

Requires a C compiler such as GCC.

```bash
gcc -Wall -Wextra SBA.c -o inventory
./inventory        # Linux / macOS / WSL
inventory.exe      # Windows
```

On first launch you will be asked for a backup code, then to create a password.

## Files Created at Runtime

| File | Purpose |
|------|---------|
| `invoice.txt` | Customer invoice records |
| `supplier_info.txt` | Supplier records |
| `password.txt` | Stored password |
| `boot_count.txt` | Number of times the system has booted |

## Record Formats

- **Invoice ID:** `INV-XXXX` (4 digits)
- **Supplier ID:** `SUP-XXXX` (4 digits)
- **Date:** `dd/mm/yy`
- **Prices** are in JMD

## Limitations

This is an educational project, not production software.

- Passwords are stored in **plain text**
- Backup codes are **hardcoded** in the source
- Records are stored in a formatted text table, not a database
- Primary-key search uses substring matching

## What I Learned

- Manual string and buffer handling in C (`fgets`, `strtok`, `snprintf`, `memset`)
- Passing structs by pointer across many functions
- File I/O modes (`r+`, `a+`, `w`) and safe file replacement
- Writing portable code with the preprocessor
- Designing validation so bad input never reaches the data files

## Possible Improvements

- Hash stored passwords instead of saving them as plain text
- Move to a binary or SQLite storage format
- Split the single source file into modules (`io`, `validation`, `auth`, `inventory`)
- Add automated tests and a Makefile

## Author

Leonardo Harrison | [GitHub](https://github.com/coderleo-encoded)
