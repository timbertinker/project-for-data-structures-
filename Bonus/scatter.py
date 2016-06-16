#!/bin/python
# python scatter.py fisier_de_input;
# Versiune python: 2.X 
# Atentie: nu merge fara modulul matplotlib
# Foloseste run.sh
import matplotlib.pyplot as plt
import sys
############################################################
xyz=[]
with open(sys.argv[1],"r",0) as in_ptr:
	xyz = [[float(v) for v in line.split()]for line in in_ptr]
z,x, y = zip(*xyz)
############################################################
plt.scatter(x, y)
plt.title('Numarul de clienti: %f'%int(len(x)))
plt.show()