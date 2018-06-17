# WSU Vancouver - CS360 - File Binary Search
## Systems Programming - Assignment 2 - Dictionary Search
## C / Search Algorithm
### Abstract
When compiled, the program searches the dictionary file *(in the default case, webster)* and returns 'yes' if the dictionary file contains the entry.  Otherwise returns no.

### Requirements
*GNU C and GNU make*

### Executing

```
        make
        ./ok <dictionary_word>
```

*to clean project when done:*

```
        make clean
```

### Programming notes
The program has a DEBUG flag which can be set to on or off:
```
#define DEBUG 1
```

The dictionary file for the program can be modified by changing the following define:
```
#define DICTFILE "webster"
```
*Note: dictionary file is not a complete dictionary of words*
