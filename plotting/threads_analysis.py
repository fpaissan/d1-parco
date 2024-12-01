import subprocess
import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import scienceplots
import seaborn as sns

plt.style.use(["science", "no-latex"])


if __name__ == "__main__":
    TASK = sys.argv[1]
    if TASK == "tra":
        STRATEGIES = ["OMP", "TILEDOMP"]
    else:
        STRATEGIES = ["OMP"]

    optimization_flag = "O0"  # Fixed to O0
    matrix_size_to_plot = 2**12  # Focus on matrix size 2^12
    thread_counts = [5, 15, 30, 40]  # Thread counts for TILEDOMP and OMP

    # Dictionary to store results
    grouped_data = {
        strategy: {threads: [] for threads in thread_counts} for strategy in STRATEGIES
    }

    log_folder = Path(f"results/{TASK}_results_{optimization_flag}")

    # Read data
    for s in STRATEGIES:
        for t in thread_counts:
            fname = f"{s}_{matrix_size_to_plot}_t{t}.txt"
            try:
                with open(log_folder / fname, "r") as f:
                    logs = f.readlines()
                # Convert logs to a NumPy array and scale by 1e3
                raw = np.array([float(x.strip()) * 1e3 for x in logs])
                if len(raw) > 0:
                    grouped_data[s][t].append(
                        np.mean(raw)
                    )  # Average for the given thread count
            except FileNotFoundError:
                print(f"Log file {fname} not found in {log_folder}.")

    # Aggregate data
    latency_data = {
        strategy: {
            threads: (
                np.mean(grouped_data[strategy][threads])
                if grouped_data[strategy][threads]
                else 0
            )
            for threads in thread_counts
        }
        for strategy in STRATEGIES
    }

    # Plot the results
    plt.figure(figsize=(10, 6))
    for strategy in STRATEGIES:
        latencies = [latency_data[strategy][threads] for threads in thread_counts]
        plt.plot(thread_counts, latencies, marker="o", label=strategy)

    # Configure the plot
    plt.xlabel("Number of Threads", fontsize=12)
    plt.ylabel(
        f"Average Latency (ms) for Matrix Size {matrix_size_to_plot}", fontsize=12
    )
    plt.title(
        f"Performance Trends for {TASK} ({optimization_flag}, Matrix Size = {matrix_size_to_plot})",
        fontsize=14,
    )
    plt.legend(title="Strategies", fontsize=10)
    plt.grid(True, linestyle="--", linewidth=0.5)
    plt.tight_layout()

    # Show the plot
    plt.show()
