# Hash cracker
Program implemented in C language, based on a distributed processing openMPI library, used to crack short hashed text containing letters of the Latin alphabet.

## Running application
Download and install [openMPI library](https://www.open-mpi.org/).

### Default hash
To run the program with default settings in the project directory type:
```
make run
```

Settings:
- NODES - number of parallel working process (def: `13`)
- HASH - encrypted text (def: `"aa7uckMQPpN46"`)
- LETTER_NUM - searched word length (def: `5`)
- EXEC - name of the exec file (def: `cracker`)

### Own hash
To decrypt your own text count message letters and generate hash with salt = `aa`. To do it you can use encryption program.
```
          make encrypt TEXT=<YOUR TEXT>
example:  make encrypt TEXT="github"  
```

Enter generated result as an argument (HASH) of the decrypt program
```
          make NODES=<PROCESS NUM> HASH=<GENERATED HASH> LETTER_NUM=<TEXT LENGTH> run
example:  make NODES=10 HASH="aaADy47La.an6" LETTER_NUM=6 run
```