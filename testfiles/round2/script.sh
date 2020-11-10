while getopts n:f: flag
do
	case "${flag}" in
		n) n=${OPTARG};;
		f) f=${OPTARG};;
	esac
done

while true; do
	read -p "Generating REQ-s to $n. Is this OK? y/n" yn
	case $yn in 
		[Yy]* ) break;;
		[Nn]* ) exit;;
		* ) echo "Ne légy fasz";;
	esac
done
cp $f $f.bak
for i in $(seq 0 $n); do
	#echo "$i";
	#echo $f;	
	echo "REQ 28248 $i 0" >> $f;
	echo "." >> $f;
done
