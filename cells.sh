#!/bin/bash

for i in {1..320}
do 
    gnuplot -persist <<-EOFMarker
        set terminal jpeg
        set output "pic$i.jpeg"
        set xlabel 'Distance'
        set ylabel 'Cells'
        set yrange [0:2]
        plot "$i.dat" using 2:3 with lines
EOFMarker
done

ffmpeg -i pic%d.jpeg cells.mp4
rm *.jpeg
