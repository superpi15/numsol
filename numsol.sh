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
printf "" > numsol.csv

STR="\"ScaleFactors\""
for num in $( seq $SFF $SFE )
do
	STR=`printf "%s , %d" "$STR" "$num"`
done
echo "$STR" >> numsol.csv 

for line in $( seq 1 $MAXL )
do
	printf "\rline %3d/%3d" $line $MAXL
	STR="$line"
	for num in $( seq $SFF $SFE )
	do
		PROB=`awk -v LINE=$line 'NR==LINE{print}' numsol.$num.tmp | awk '{print $4}'`
		STR=`printf "%s , %s " "$STR" "$PROB"`
	done
	echo "$STR" >> numsol.csv
done
printf "\ndone\n"

echo "Removing numsol.*.tmp"
rm numsol.*.tmp

# Generate Plotting File

printf "" > numsol.plot
echo "set datafile separator \",\""             >> numsol.plot
echo "set key autotitle columnhead"             >> numsol.plot
echo "set title 'Prefill vs. Scale-Factor'"     >> numsol.plot
echo "set yrange [0:]"                  >> numsol.plot
echo "set term png"                             >> numsol.plot
echo "set output 'numsol.png'"                  >> numsol.plot

STR="plot 'numsol.csv' "
for num in $( seq $SFF $SFE )
do
	STR=`printf "%s , \'\' using 1:%d with lines " "$STR" "$num"`
done
echo "$STR" >> numsol.plot 
