#!/bin/bash
#bash run.sh [path] || ./run.sh [path] daca are permisiuni
# A se specifica [path] daca fisierul de input nu se afla la ../inputGenerator/users.csv
# Scripturile in python nu functioneaza pe inputul primit, ruleaza asta
# 
f="users.csv"
x="input.in"
############################################################
if [ $# -eq 0 ]
  	then
    	cp ../inputGenerator/users.csv $f
	else
		cp $1 $f
fi
cat $f | tr "," " " | tail -n +2 > $x
############################################################
	python heatmap.py $x & python scatter.py $x && fg
	wait
	rm $f
	rm $x