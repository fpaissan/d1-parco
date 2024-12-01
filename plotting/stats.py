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
    OPT = sys.argv[2]

    if TASK == "symm":
        STRATEGIES = ["SEQ", "IMP", "OMP"]
    else:
        STRATEGIES = ["SEQ", "IMP", "OMP", "TILED", "TILEDOMP", "CO"]

    LOGS = []
    for s in STRATEGIES:
        for pow in range(4, 13):
            if not "OMP" in s:
                THREADS = [1]
            else:
                THREADS = [5, 15, 30, 40]

            for t in THREADS:
                LOGS.append(f"{s}_{2**pow}_t{t}.txt")

        results = {}
        for fname in LOGS:
            output_folder = Path(f"plots/{TASK}/")
            log_folder = Path(f"results/{TASK}_results_{OPT}")
            with open(log_folder / fname, "r") as f:
                logs = f.readlines()

            raw = np.array(list(map(lambda x: float(x) * 1e3, logs)))
            if len(raw) == 0:
                print("Log is empty.")
                exit()

            results[fname] = raw

    # Organize results by strategy and threads
    grouped_results = {}
    for fname, data in results.items():
        strategy, size, threads = fname.split("_")
        size = int(size)  # Power of two size
        threads = int(threads[1:-4])  # Extract thread count from t{threads}.txt

        if (strategy, threads) not in grouped_results:
            grouped_results[(strategy, threads)] = {}

        if size not in grouped_results[(strategy, threads)]:
            grouped_results[(strategy, threads)][size] = []

        grouped_results[(strategy, threads)][size].extend(data)

    # Create a plot for each configuration
    for (strategy, threads), data_by_size in grouped_results.items():
        powers_of_two = sorted(data_by_size.keys())
        violin_data = []
        violin_labels = []

        for size in powers_of_two:
            violin_data.extend(data_by_size[size])
            violin_labels.extend([f"2^{int(np.log2(size))}"] * len(data_by_size[size]))

        # Generate the plot
        # plt.figure(figsize=(12, 7))
        sns.violinplot(
            x=violin_labels,
            y=violin_data,
            inner="quartile",
            scale="width",
            palette="muted",
        )

        y_max = max(data_by_size[size])
        annotation_padding = 0.13 * y_max
        plt.ylim(0, y_max + annotation_padding)

        # Add annotations
        for i, size in enumerate(powers_of_two):
            max_val = max(data_by_size[size])
            mean_val = np.mean(data_by_size[size])
            plt.text(
                i,
                max_val + 0.01 * max_val,
                f"{mean_val:.2f}",
                ha="center",
                va="bottom",
                fontsize=8,
                color="black",
            )

        # Add labels and title
        plt.xlabel("Power of Two (2^x)")
        plt.ylabel("Time [ms]")
        plt.title(f"{strategy} - {OPT} - {threads} Threads")
        plt.xticks(rotation=45)  # Rotate labels for better readability
        plt.grid(axis="y", linestyle="--", alpha=0.7)

        # Show or save the plot
        plt.tight_layout()
        plt.savefig(output_folder / f"{OPT}_{strategy}_t{threads}.pdf")
        plt.cla()
        plt.clf()
