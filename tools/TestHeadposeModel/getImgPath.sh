#run this script to generate jpg image path file 
#for specified directory

#path file name
PATH_FILE="imgPaths.txt"

getImgPath(){
	for file in $1/*
	do
		if [ -d $file ]
		then
			getImgPath $file
		else
			echo $file | grep -i ".*.jpg" >> $PATH_FILE
		fi
	done
	return $?
}

if [ -e "$PATH_FILE" ]
then
	rm $PATH_FILE
	echo "$PATH_FILE removed."
fi

echo "generating $PATH_FILE"

getImgPath $1

echo "done."
