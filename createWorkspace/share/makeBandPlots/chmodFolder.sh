for i in `ls`; 
do 
	if [ -d $i ] ; then 
		chmod 744 -R $i ; 
	else 
		chmod 666 $i; 
	fi; 
done
