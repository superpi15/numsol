SFF=$1
SFE=$2
TH=$3
NUMSOL=./a.out
if [ "$SFF" == "" ];
then
	echo "numsol.sh ScaleFactor-First ScaleFactor-End Threshold"
	exit 0
fi

if [ ! -f "$NUMSOL" ]
then
	echo "$NUMSOL does not exist" 
	exit 0
fi

MAXL=0
for num in $( seq $SFF $SFE )
do
	printf "\r%3d/%3d" $num $SFE
	$NUMSOL $num $TH > numsol.tmp 
	grep "val" numsol.tmp > numsol.$num.tmp 
	LINE=`cat numsol.$num.tmp | wc -l `
	MAXL=$(( $LINE > $MAXL ? $LINE : $MAXL ))
done
printf "\ndone\n"

echo "Extracting Result"
printf "" > numsol.res
for line in $( seq 1 $MAXL )
do
	printf "\rline %3d/%3d" $line $MAXL
	STR=""
	for num in $( seq $SFF $SFE )
	do
		if [ "$num" != "$SFF" ]
		then 
			STR=`printf "%s , " "$STR" `
		fi
		PROB=`awk -v LINE=$line 'NR==LINE{print}' numsol.$num.tmp | awk '{print $4}'`
		STR=`printf "%s %s " "$STR" "$PROB"`
	done
	echo "$STR" >> numsol.res
done
printf "\ndone\n"

echo "Removing numsol.*.tmp"
rm numsol.*.tmp
