# Introduction to Parallel Computing - Homework 1

The project is described in this [report](parco_d1.pdf). The appendix to the report is uploaded [here](./appendices).

To reproduce the results, you can follow these steps:

### Run the kernels on your system
For matrix transposition, you can use the following command:
```bash
./scripts/bulk_experiments_tra.sh
```
This will loop through all the compilation flags, matrix sizes, and strategies. For OpenMP-based kernels, it will loop through different number of threads in {5, 15, 30, 40}.

Similarly, for the symmetry checking kernel, you can use
```bash
./scripts/bulk_experiments_sym.sh
```

These scripts will save the output in the results folder. To aggregate the measurements, you can use the plotting scripts (see below).

### Analyze the results

To reproduce the plots presented in the report (and in the appendix), you can use the scripts in the `plotting` folder. If the scripts complains about files not existing, it's likely because you did not create the folder where to store the plots (you can do so via `mkdir -p {LOCATION}`. This is intended as a safety measure to avoid overriding results during the development of the project.

To visualize the violin plots, use:

```bash
python plotting/stats.py {TASK} {OPT_LEVEL}
```
where TASK can be `symm` or `tra` and OPT_LEVEL ranges from `O0` to `O3`.

To visualize the impact of the number of threads, use:

```bash
python plotting/threads_analysis.py {TASK}
```

To visualize the impact of the optimization flags, use:

```bash
python plotting/trends.py {TASK}
```

To visualize the transfer rate of different kernels, you can use:

```bash
python plotting/peak_performance.py {TASK}
```

For additional information, contact me at francescopaissan [at] gmail [dot] com.
