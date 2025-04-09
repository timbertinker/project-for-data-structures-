#!/bin/python
# python heatmap.py fisier_de_input;
# Versiune python: 2.X 
# Atentie: nu merge fara modulele matplotlib si numpy
# Foloseste run.sh
import numpy as np
import matplotlib.pyplot as plt
import sys
############################################################
with open(sys.argv[1],"r",0) as in_ptr:
	xy=[]
	xyz= [[float(v) for v in line.split()]for line in in_ptr]
	z,x,y = zip(*xyz)
	in_ptr.close()
############################################################
	heatmap, xedges, yedges = np.histogram2d(x,y,bins=100) # Modifica bins=x pentru rezolutie
	extent = [xedges[0],xedges[-1],yedges[0],yedges[-1]]
############################################################
	plt.clf()
	plt.imshow(heatmap,extent=extent)
	plt.title('Numarul de clienti: %f'%int(len(x)))
	plt.show()
