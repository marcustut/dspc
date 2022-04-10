# DSPC

## User Manual

### NOT CUDA
1. Click on the `main.exe` to run this parallelisation program (except `CUDA`).

2. Usage:
    ```
    main <TYPE> <TECHNIQUE> <DATASET> [NUMBER_OF_ROWS]
    ```
    Field | Description
    --- | ---
    TYPE | Type of application to open, specify either _GUI_ or _CLI_
    TECHNIQUE | Type of technique to perform the algorithm, specify one of *Serial*, *OpenMP*, *Pthread*, *CppStdLib*
    DATASET | Relative path to dataset (.csv file). The default `mock.csv` is provided in this repo.
    NUM_OF_ROWS | Number of rows in the data to be loaded, default `160000`.

3. Enter `2` values of _X_ value to predict salary in 4 d.p.

### CUDA
1. Click on the `cuda.exe` to run CUDA parallelisation program.

2. Usage:
   ```
    CUDA <NUM_OF_ROWS>
    ```
    Field | Description
    --- | ---
    NUM_OF_ROWS | Number of rows in the data to be loaded, `CANNOT` exceed 110000

3. Enter `2` values of _X_ value to predict salary in 4 d.p.