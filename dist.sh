#!/bin/bash

for i in {1..320}
do 
    gnuplot -persist <<-EOFMarker
        set terminal jpeg
        set output "dist$i.jpeg"
        set xlabel 'Distance'
        set ylabel 'Displacement'
        set yrange [-0.01:0.1]
        plot "$i.dat" using 2:4 with lines
EOFMarker
done

ffmpeg -i dist%d.jpeg displacement.mp4
rm *.jpeg
