#! /bin/bash
file="employee.txt"
function create_emp()
{
	echo "emp name:"
	read name
	echo "emp num:"
	read num
	echo "teleph:"
	read tel
	if grep -i "^.* $num .*$" "$file" 
	then
	   echo "record is already there"
	else 
	echo "$name $num $tel" >> "$file"
	echo "record inserted successfully"
	fi
}

function del_emp(){
	echo "enter employee number to be deleted"
	read num
	res=$( grep -i "^.* $num .*$" "$file"  )
	if [ ! -z "$res" ]
	then 
	  grep -v "^.* $num .*$" "$file" > temp.txt
	  mv temp.txt "$file"
	  echo "record deleted successfully"
	else
	  echo "record doesn't exit"
	fi
	  
}
function search_emp()
{ 
	echo "enter employee number to search"
	read num
	res=$( grep "^.* $num .*$" "$file" )
	if [ -z "$res " ]
	then
	  echo "record not found"
	else
	  echo "employee details : $res"
	fi   
	
}

function disp_emp(){
	search_emp
}

function list_emp()
{
	echo "The employee details as follows:"
	cat "$file"
}
	
function sort_emp(){
	sort -k 2 -o "$file" "$file"
	#sort -k 2 "$file" > "$file"
	echo "sorted"
	echo
	list_emp
}



while true
do
echo "Menu:"
echo "1. Create employee record"
echo "2. Delete employee record"
echo "3. Search employee record"
echo "4. Display employee record"
echo "5. sort employee record"
echo "6. list all the employee records"
echo "7. exit"
echo "enter your choice"
read val
case $val in
1)
	create_emp
	;;
2)
	del_emp
	;;
3)	
	search_emp
	;;
4)	
	disp_emp
	;;
	
5)
	sort_emp
	;;
6)
	list_emp
	;;
7)	
	exit 0
	;;
*)
	echo "enter valid choice value"
	;;
esac
	echo
done


