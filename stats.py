import subprocess
import sys
from pathlib import Path

import matplotlib.pyplot as plt
import numpy as np
import scienceplots
import seaborn as sns

plt.style.use(["science", "no-latex"])

if __name__ == "__main__":
    OPT = sys.argv[1]

    STRATEGIES = ["SEQ", "IMP", "OMP"]
    LOGS = []
    for s in STRATEGIES:
        for pow in range(4, 13):
            LOGS.append(f"{s}_{2**pow}.txt")

        results = {}
        for fname in LOGS:
            log_folder = Path(f"symm_results_{OPT}")
            with open(log_folder / fname, "r") as f:
                logs = f.readlines()

            raw = np.array(list(map(lambda x: float(x), logs)))
            if len(raw) == 0:
                print("Log is empty.")
                exit()

            results[fname] = raw

        powers_of_two = list(range(4, 13))  # Corresponding to 2^4 to 2^12
        data_by_power = {2**pow: [] for pow in powers_of_two}

        for fname, raw_data in results.items():
            # Extract power of two from the filename
            _, size_with_txt = fname.split("_")
            size = int(size_with_txt.split(".")[0])  # Extract 2^power
            data_by_power[size].extend(raw_data)

        # Prepare data for plotting
        violin_data = []
        violin_labels = []
        means = []  # To store mean values for annotations
        maxv = []

        for pow in powers_of_two:
            size = 2**pow
            values = data_by_power[size]
            violin_data.extend(values)
            violin_labels.extend([f"2^{pow}"] * len(values))
            maxv.append(np.max(values))
            means.append(np.mean(values))  # Compute mean for each group

        # Create the violin plot
        # plt.figure(figsize=(6, 5))
        sns.violinplot(
            x=violin_labels,
            y=violin_data,
            inner="quartile",
            scale="width",
            palette="muted",
        )

        # Adjust y-axis limit to ensure all annotations fit
        y_max = max(maxv)  # Highest data point
        annotation_padding = 0.3 * y_max  # 10% padding above the highest point

        # Add annotations above the violins
        # Add annotations just above the max value of each violin
        for i, (mean, max_val) in enumerate(zip(means, maxv)):
            plt.text(
                i if i != 12 else 10,
                max_val + 0.10 * max_val,  # Place slightly above the max value
                f"{mean * 1e3:.2f}",
                ha="center",
                va="bottom",
                fontsize=8,
                color="black",
            )

        # Add labels and title
        plt.xlabel("Power of Two (2^x)")
        plt.ylabel("Time [s]")
        plt.ylim(0, y_max + annotation_padding)  # Extend y-axis range
        # plt.semilogy()
        plt.title(f"Wall-clock time distribution (-{OPT}, {s})")
        plt.xticks(rotation=45)  # Rotate labels for better readability
        plt.grid(axis="y", linestyle="--", alpha=0.7)

        # Show the plot
        plt.tight_layout()
        plt.savefig(f"plots/{OPT}_{s}.pdf")
        plt.cla()
        plt.clf()
        # plt.show()
