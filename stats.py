import subprocess

import matplotlib.pyplot as plt
import numpy as np


def compile(file, flags=[]):
    compile = ["gcc"] + flags + [file]
    subprocess.run(compile, stdout=subprocess.PIPE, text=True)


if __name__ == "__main__":
    compile("main_sym.c")

    execute = ["./a.out", "1000"]
    result = subprocess.run(execute, stdout=subprocess.PIPE, text=True)

    raw = np.array(list(map(lambda x: float(x), result.stdout.splitlines())))
    if len(raw) == 0:
        print("Seg fault!")
        exit()

    wc_sym = raw[:100]
    wc_t = raw[100:]

    counts, bins = np.histogram(wc_sym)
    plt.hist(bins[:-1], bins, weights=counts)
    plt.show()

    counts, bins = np.histogram(wc_t)
    plt.hist(bins[:-1], bins, weights=counts)
    plt.show()
