#!bin/bash

gcc main.c -o main

# Varying Ki values from 1 to 4
./main -i 1.0 -m 1.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/ki-1.txt
./main -i 2.0 -m 1.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/ki-2.txt
./main -i 3.0 -m 1.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/ki-3.txt
./main -i 4.0 -m 1.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/ki-4.txt

# Varying km values from 1 to 3
./main -i 1.0 -m 1.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/km-1.txt
./main -i 1.0 -m 1.5 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/km-15.txt
./main -i 1.0 -m 2.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/km-2.txt

# Varying Kn values from 0.5 to 2
./main -i 1.0 -m 1.0 -n 0.5 -f 0.2 -s 0.01 -T 200 -o output/kn-05.txt
./main -i 1.0 -m 1.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/kn-1.txt
./main -i 1.0 -m 1.0 -n 1.5 -f 0.2 -s 0.01 -T 200 -o output/kn-15.txt
./main -i 1.0 -m 1.0 -n 2.0 -f 0.2 -s 0.01 -T 200 -o output/kn-2.txt

# Varying Kf values from 0.1 to 0.5
./main -i 1.0 -m 1.0 -n 1.0 -f 0.1 -s 0.01 -T 200 -o output/kf-01.txt
./main -i 1.0 -m 1.0 -n 1.0 -f 0.3 -s 0.01 -T 200 -o output/kf-03.txt
./main -i 1.0 -m 1.0 -n 1.0 -f 0.5 -s 0.01 -T 200 -o output/kf-05.txt

# Varying Kf values from 0.01 to 0.10
./main -i 1.0 -m 1.0 -n 1.0 -f 0.2 -s 0.01 -T 200 -o output/ps-01.txt
./main -i 1.0 -m 1.0 -n 1.0 -f 0.2 -s 0.05 -T 200 -o output/ps-05.txt
./main -i 1.0 -m 1.0 -n 1.0 -f 0.2 -s 0.10 -T 200 -o output/ps-10.txt


#plotting graphs for different ki values
gnuplot -e "set terminal png enhanced size 2000, 5000;set output 'diff-ki.png';set multiplot layout 4, 1;plot 'output/ki-1.txt' w l;plot 'output/ki-2.txt' w l;plot 'output/ki-3.txt' w l;plot 'output/ki-4.txt' w l;"

#plotting graphs for different km values
gnuplot -e "set terminal png enhanced size 2000, 5000;set output 'diff-km.png';set multiplot layout 3, 1;plot 'output/km-1.txt' w l;plot 'output/km-15.txt' w l;plot 'output/km-2.txt' w l;"

#plotting graphs for different kn values
gnuplot -e "set terminal png enhanced size 2000, 5000;set output 'diff-kn.png';set multiplot layout 4, 1;plot 'output/kn-05.txt' w l;plot 'output/kn-1.txt' w l;plot 'output/kn-15.txt' w l;plot 'output/kn-2.txt' w l;"

#plotting graphs for different kf values
gnuplot -e "set terminal png enhanced size 2000, 5000;set output 'diff-kf.png';set multiplot layout 3, 1;plot 'output/kf-01.txt' w l;plot 'output/kf-03.txt' w l;plot 'output/kf-05.txt' w l;"

#plotting graphs for different ps values
gnuplot -e "set terminal png enhanced size 2000, 5000;set output 'diff-ps.png';set multiplot layout 3, 1;plot 'output/ps-01.txt' w l;plot 'output/ps-05.txt' w l;plot 'output/ps-10.txt' w l;"
