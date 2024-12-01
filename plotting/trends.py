""" 
Author: Francesco Paissan
"""

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import scienceplots
import seaborn as sns

plt.style.use(["science", "no-latex"])


if __name__ == "__main__":
    TASK = sys.argv[1]

    if TASK == "symm":
        STRATEGIES = ["SEQ", "IMP", "OMP"]
    else:
        STRATEGIES = ["SEQ", "IMP", "OMP", "TILED", "TILEDOMP"]

    optimization_flags = ["O0", "O1", "O2", "O3"]

    for matrix_size_to_plot in [2**i for i in range(4, 13)]:
        grouped_data = {
            strategy: {
                flag: {2**pow: [] for pow in range(4, 13)}
                for flag in optimization_flags
            }
            for strategy in STRATEGIES
        }

        # Loop through each optimization flag
        for OPT in optimization_flags:
            log_folder = Path(f"results/{TASK}_results_{OPT}")

            for s in STRATEGIES:
                for pow in range(4, 13):
                    if not "OMP" in s:
                        THREADS = [1]
                    else:
                        THREADS = [5, 15, 30, 40]

                    for t in THREADS:
                        fname = f"{s}_{2**pow}_t{t}.txt"
                        try:
                            with open(log_folder / fname, "r") as f:
                                logs = f.readlines()
                            # Convert logs to a NumPy array and scale by 1e3
                            raw = np.array([float(x.strip()) * 1e3 for x in logs])
                            if len(raw) > 0:
                                grouped_data[s][OPT][2**pow].append(
                                    np.mean(raw)
                                )  # Store average for each size
                        except FileNotFoundError:
                            print(f"Log file {fname} not found in {log_folder}.")

        # Extract data for matrix size 2^12
        latency_data = {
            strategy: {
                flag: (
                    np.mean(grouped_data[strategy][flag][matrix_size_to_plot])
                    if grouped_data[strategy][flag][matrix_size_to_plot]
                    else 0
                )
                for flag in optimization_flags
            }
            for strategy in STRATEGIES
        }

        # Plot the results
        # plt.figure(figsize=(10, 6))
        for strategy in STRATEGIES:
            latencies = [latency_data[strategy][flag] for flag in optimization_flags]
            plt.plot(optimization_flags, latencies, marker="o", label=strategy)

        # Configure the plot
        plt.xlabel("Optimization Flags")
        plt.ylabel(f"Average Latency [ms]")
        plt.title(f"Matrix Size = {matrix_size_to_plot}")
        plt.legend(fontsize=7)
        plt.grid(True, linestyle="--", linewidth=0.5)
        plt.tight_layout()

        # Show the plot
        plt.savefig(f"plots/trends/opt_task-{TASK}_s{matrix_size_to_plot}.pdf")
        plt.cla()
        plt.clf()
