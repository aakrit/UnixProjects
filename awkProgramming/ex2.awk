
BEGIN {FS="\t"} #create tab to be the field separator 

/\tCA\t/ {sum_CA+=$4; ca++} #add all the rain in CA in Jan 

/\tAK\t/ {sum_AK+=$4; ak++} #add all rain in AK in Jan

/\tTX\t/ {sum_TX+=$4; tx++} #add all rain in TX in Jan

END {
print "\nAll values are reported in inches\n"
print "\nThe average rainfall for month of January in California is= ", sum_CA/ca
print "\nThe average rainfall for month of January in Alaska is= ", sum_AK/ak
print "\nThe average rainfall for month of January in Texas is= ", sum_TX/tx
print "\n"
}
