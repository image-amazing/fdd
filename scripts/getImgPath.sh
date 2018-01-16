#run this script to generate jpg image path file 
#for specified directory

#path file name
PATH_FILE="imgPaths.txt"

REG_EXP=".*"

getImgPath(){
	for file in $1/*
	do
		if [ -d "$file" ]
		then
			getImgPath "$file" 
		else
			echo "$file" | grep -Ei "$REG_EXP" >> $PATH_FILE
			echo "$file" | grep -Ei "$REG_EXP"
		fi
	done
	return $?
}

if [ $# -lt 3 ]
then
	echo "input directory , output file , regex name needed"
	exit 1
fi

PATH_FILE="$2"

if [ -e "$PATH_FILE" ]
then
	rm "$PATH_FILE"
	echo "$PATH_FILE removed."
fi

echo "generating $PATH_FILE"

if [ $# -eq 3 ]
then
	REG_EXP="$3"
fi

echo "REG_EXP=$REG_EXP"

echo "waiting"

getImgPath $1

echo "done."
