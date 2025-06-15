import matplotlib.pyplot as plt
import numpy as np
import random
from kdtree import RelaxedKDTree
from typing import List, Union
import colorsys

# Experiment


def make_query(k: int, s: int) -> List[Union[float, str]]:
    point = [random.random() for _ in range(k)]
    return point[:s] + ['?'] * (k - s)



def run_experiment(k, n, s, q):
    """
    Runs an experiment to evaluate the average and standard deviation of node visits in a RelaxedKDTree during partial match queries.
    Parameters:
        k (int): The dimensionality of the data points.
        n (int): The number of data points to generate and insert into the tree.
        s (int): The number of specified (non-wildcard) dimensions in each query.
        q (int, optional): The number of queries to run. Defaults to 1000.
    Returns:
        tuple: A tuple containing the mean and standard deviation of the number of node visits per query.
    """
    tree = RelaxedKDTree([[random.random() for _ in range(k)] for _ in range(n)])
    costs = []
    for _ in range(q):
        query = make_query(k, s)
        visits, _ = tree.partial_match(query)
        costs.append(visits)
    return np.mean(costs) #, np.std(costs)


# Experiment parameters
ks = [2, 4, 6, 8]
ns = [10, 100, 1_000, 10_000, 100_000]
q = 1000

results = {}
for k in ks:
    for s in range(k-1, 0, -2):
        x_vals, y_vals = [], []
        for n in ns:
            avg_cost = run_experiment(k, n, s, q=1_000)
            print(f'{k=}, {s=}, {n=}')
            x_vals.append(n)
            y_vals.append(avg_cost)
        results[(k, s)] = (x_vals, y_vals)

# Plotting
plt.figure(figsize=(10, 6))
for (k, s), (x, y) in results.items():
    plt.plot(x, y, marker='o', label=f'k={k}, s={s}', color=colorsys.hsv_to_rgb(k/max(ks), 1, 1)+((s+1)/(k+1),))

plt.xlabel("n")
plt.ylabel("avg cost")
plt.loglog()
plt.title("Average Cost of Partial Match Queries vs Tree Size")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("plot.png")
plt.show()