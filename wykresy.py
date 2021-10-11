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
    data = np.column_stack((data,single_time))
    # for i in range(len(data)):
        # data[i][1] = "".join(data[i][1].split())
    n = np.array([data[i][1].count(' ') for i in range(len(data))])
    # n = np.array([len(data[i][1])-1 for i in range(len(data))])
    data = np.column_stack((data,n))
    # print(data)
    x = [data[i][6] for i in range(len(data))]
    y = [data[i][5] for i in range(len(data))]
    pllt.plot(x,y,label='TSP brute force',linewidth=4.0,marker='.')
    # x_e = np.linspace(min([data[i][6] for i in range(len(data))]),max([data[i][6] for i in range(len(data))]),max([data[i][6] for i in range(len(data))])-min([data[i][6] for i in range(len(data))]))
    # x_e = n
    # x_e = np.arange(min([data[i][6] for i in range(len(data))]),max([data[i][6] for i in range(len(data))]))
    # y_e = np.array([math.factorial(i) for i in x_e])
    # y_e = np.array([i/param for i in y_e])
    # pllt.plot(x_e,y_e,label='n!')
    x_e = np.linspace(min([data[i][6] for i in range(len(data))]),max([data[i][6] for i in range(len(data))]),100)
    param = max(y)/max(gamma(x_e))
    pllt.plot(x_e,gamma(x_e)*param,label='n!',linestyle='--',linewidth=3.0)
    pllt.xlabel('Liczba wierzchołków w grafie')
    pllt.ylabel('Czas wykonywania algorytmu[s]')
    pllt.legend()
    pllt.show()
    pass



if __name__=="__main__":
    main()