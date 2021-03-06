# Blocksworld

## Compiling
To compile blocksworld simply type make into the command line. This will auto-generate the proper compiler commands:
```
>> make
g++ -std=c++11 blocksworld.cpp
```
This will generate an a.out file.

## Running
To run the blocksworld program use ./a.out plus any arguments
```
./a.out <problem_to_solve> -H [H0|H1|H2] -MAX_ITERS <max_iterations> -MOVES
```

## Arguments
| Argument | Description | Example | Required |
| ---------- | ---------- | ---------- | ---------- |
| <problem_to_solve> | The location of the file to be inputed into the program. Problems must be in the same design as specified in the problem statement | probA03.pwb | Yes |
| -H | The desired heuristic function | H0 | No -- default:H2 |
| -MAX_ITERS | The maximum iterations desired for the algorithm before quitting | 1000 | No -- default:100000 |
| -MOVES | Prints out the solution path | No arguments associated with | No -- deafault:No printing |

## Notes
- H0 is for BFS
- Testing has shown program will work with multiple number of stacks
- Best heuristic successfully finds goal routes for all provided test cases
- Best heuristic will overestimate few configurations by a small number of moves
- There will be variation in performance based on operating system as shown by similar, but differing test results on local machine and TAMU Compute servers. This is documented in detail in RESULTS.txt