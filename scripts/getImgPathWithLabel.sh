#run this script to generate jpg image path file 
#for specified directory

#input directory
INPUT_DIRECTORY="."
#label
LABEL="0"
#path file name
PATH_FILE="imgPaths.txt"
#regex
REG_EXP=".*.jpg"


getImgPath(){
	for file in $1/*
	do
		if [ -d $file ]
		then
			getImgPath $file 
		else
			if echo $file | grep -i "$REG_EXP" > /dev/null
			then
				echo "$file $LABEL" >> $PATH_FILE
				echo $file $LABEL
			fi
		fi
	done
	return $?
}

if [ $# -lt 4 ]
then
	echo "input directory,label,output file name,regex needed"
	exit 1
fi

INPUT_DIRECTORY="$1"
echo "INPUT_DIRECTORY=$INPUT_DIRECTORY"

LABEL="$2"
echo "LABEL=$LABEL"

PATH_FILE="$3"
if [ -e "$PATH_FILE" ]
then
	rm $PATH_FILE
	echo "$PATH_FILE removed."
fi
echo "generating $PATH_FILE"

REG_EXP="$4"
echo "REG_EXP=$REG_EXP"

echo "waiting"

getImgPath $INPUT_DIRECTORY

echo "done."
