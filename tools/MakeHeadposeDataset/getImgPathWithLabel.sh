#run this script to generate jpg image path file 
#for specified directory

#path file name
PATH_FILE="imgPathsWithLabel.txt"

getImgPath(){
	for file in $1/*
	do
		if [ -d $file ]
		then
			getImgPath $file
		else
			if echo $file | grep ".*视点.*\.jpg"
			then
			#get image label
			label=${file##*_}
			label=${label%%.*}
			#convert string to decimal digit
			((label=10#$label))
			label=$(($label-1))
			echo $file $label >> $PATH_FILE
			fi
		fi
	done
	return $?
}

if [ $# -lt 1 ]
then
	echo "$0 need specified directory." 
	exit 1
fi

if [ -e "$PATH_FILE" ]
then
	rm $PATH_FILE
	echo "$PATH_FILE removed."
fi

echo "generating $PATH_FILE"

getImgPath $1

echo "done."
