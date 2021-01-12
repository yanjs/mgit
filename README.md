# mgit

A simple git-like version control system that only has essential functions.

## Functions Implemented So Far

| Mgit command                  | Equivalent git command          |
| ----------------------------- | ------------------------------- |
| `mgit init`                   | `git init`                      |
| `mgit hash-object [files...]` | `git hash-object -w [files...]` |
| `mgit cat-file [files...]`    | `git cat-file [files...]`       |

## Functions to Be Refactored
| Mgit command                  | Equivalent git command          |
| ----------------------------- | ------------------------------- |
| `mgit write-tree`             | `git write-tree`                |

## Next Function to Be Implemented

`mgit read-tree`
