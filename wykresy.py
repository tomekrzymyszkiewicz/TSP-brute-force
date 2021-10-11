import numpy as np
import pandas as pd
import matplotlib as plt
from matplotlib import pyplot as pllt




def main():    
    data = pd.read_csv('wyniki.csv')
    # print(data)
    data = np.array(data)
    # print(data)

    single_time = np.array([data[i][3]/data[i][4] for i in range(len(data))])
    data = np.column_stack((data,single_time))
    for i in range(len(data)):
        data[i][1] = "".join(data[i][1].split())
    # print(data)
    n = np.array([len(data[i][1])-1 for i in range(len(data))])
    # print(n)
    data = np.column_stack((data,n))
    print(data)
    x = [data[i][6] for i in range(len(data))]
    y = [data[i][5] for i in range(len(data))]
    pllt.plot(x,y)
    pllt.show()
    pass



if __name__=="__main__":
    main()