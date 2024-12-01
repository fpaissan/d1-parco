""" 
Author: Francesco Paissan
"""

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import scienceplots

plt.style.use(["science", "no-latex"])


if __name__ == "__main__":
    TASK = sys.argv[1]
    optimization_flag = "O0"  # Example optimization flag
    STRATEGIES = ["SEQ", "IMP", "OMP", "TILED", "TILEDOMP"]
    matrix_sizes = [2**pow for pow in range(4, 13)]  # Sizes from 2^4 to 2^12

    # Dictionary to store results
    grouped_data = {
        strategy: {size: [] for size in matrix_sizes} for strategy in STRATEGIES
    }
    log_folder = Path(f"results/{TASK}_results_{optimization_flag}")

    # Read data
    for s in STRATEGIES:
        for size in matrix_sizes:
            if not "OMP" in s:
                THREADS = [1]
            else:
                THREADS = [5, 15, 30, 40]

            for t in THREADS:
                fname = f"{s}_{size}_t{t}.txt"
                try:
                    with open(log_folder / fname, "r") as f:
                        logs = f.readlines()
                    # Convert logs to a NumPy array and scale by 1e3
                    raw = np.array([float(x.strip()) * 1e3 for x in logs])
                    if len(raw) > 0:
                        grouped_data[s][size].append(
                            np.mean(raw)
                        )  # Average for the given size
                except FileNotFoundError:
                    print(f"Log file {fname} not found in {log_folder}.")

    # Compute GB/s for each matrix size and strategy
    bandwidth_data = {strategy: [] for strategy in STRATEGIES}
    for s in STRATEGIES:
        for size in matrix_sizes:
            if grouped_data[s][size]:
                avg_time_ms = np.mean(grouped_data[s][size])  # Average time in ms
                avg_time_s = avg_time_ms / 1000  # Convert to seconds
                data_moved_bytes = 8 * size**2  # Data moved in bytes
                bandwidth_gbps = data_moved_bytes / avg_time_s / 1e9  # Convert to GB/s
                bandwidth_data[s].append(bandwidth_gbps)

    # Plot the results
    # plt.figure(figsize=(12, 7))
    colors = plt.cm.viridis(
        np.linspace(0, 1, len(STRATEGIES))
    )  # Unique colors for each strategy

    for idx, strategy in enumerate(STRATEGIES):
        plt.plot(
            matrix_sizes,
            bandwidth_data[strategy],
            marker="o",
            label=strategy,
            color=colors[idx],
        )

    # Configure the plot
    plt.xlabel("Matrix Sizes (2^n)")
    plt.ylabel("Bandwidth (GB/s)")
    plt.legend(loc="upper left", bbox_to_anchor=(1.05, 1), fontsize=6)
    plt.xscale("log")  # Log scale for better visualization of large matrix sizes
    plt.grid(True, linestyle="--", linewidth=0.5)
    plt.tight_layout()

    # Show the plot
    plt.savefig("./appendices/plots/peak_performance.pdf")
