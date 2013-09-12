#! /bin/sh

awk 'BEGIN {FS="\t"} #create a gawk script to run inside a bash script with tab to be the field separator

/\tYRS\t/ { #examine only the 1st row of the fine to determine which field to use for the month based on the second argument by the user
	for (i=0; i < NF; i++){ #loop through each field (NF)
		if($i=="'"$2"'"){ #check if any of the fields equals the month provided in the second argument
			Month_field=i #if so then save that field in variable
		}
	}
}

/\t'"$1"'\t/ {sum_ST+=$Month_field;st++} #add all the rain for the state ($1) during the month (Mo_val) 

END {

avg = sum_ST/st # store average value in new variable
if (avg != 0){
	print "\nThe average rainfall for '"$1"' during month of '"$2"' was: ", avg "inches\n"
	print "\n"
}
else print "\nIncorrect format: Please specify the state and month in abbreviation form and all uppercase ie. Texas January should be written as TX JAN \n"


}' normal.precip.txt
