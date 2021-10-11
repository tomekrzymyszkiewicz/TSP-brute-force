import numpy as np
import pandas as pd
import matplotlib as plt
from matplotlib import pyplot as pllt
import math
from scipy.special import gamma


def main():
    data = pd.read_csv('wyniki.csv')
    data = np.array(data)
    single_time = np.array([data[i][3]/data[i][4] for i in range(len(data))])
    data = np.column_stack((data, single_time))
    n = np.array([data[i][1].count(' ') for i in range(len(data))])
    data = np.column_stack((data, n))
    x = [data[i][6] for i in range(len(data))]
    y = [data[i][5] for i in range(len(data))]
    pllt.plot(x, y, label='TSP brute force',
              linewidth=3.0, marker='.', markersize=15)
    x_e = np.linspace(min([data[i][6] for i in range(len(data))]), max(
        [data[i][6] for i in range(len(data))]), 100)
    param = max(y)/max(gamma(x_e))
    pllt.plot(x_e, gamma(x_e)*param, label='n!', linestyle='--', linewidth=2.0)
    pllt.xlabel('Liczba wierzchołków w grafie')
    pllt.ylabel('Czas wykonywania algorytmu[s]')
    pllt.legend()
    pllt.show()
    pass


if __name__ == "__main__":
    main()
