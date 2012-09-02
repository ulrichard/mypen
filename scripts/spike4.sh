#! /bin/sh

cat ../logs/refnr.log         | perl spike4.pl > ../logs/refnr.out
cat ../logs/line_parallel.log | perl spike4.pl > ../logs/line_parallel.out
cat ../logs/line_perp.log     | perl spike4.pl > ../logs/line_perp.out


