#!bash

shell_path=~/.myscripts/shell
python_path=~/.myscripts/python

if [ "$1" == "-h" ]; then
    echo "  -l: (default) List all supported myscripts"
    echo "  -u: List all supported myscripts's Usage"
    exit
fi

for file in $(find $shell_path -type f -name "*.sh"); do

    fname=$(basename $file)
    if [ "$fname" == "myshell_help.sh" ]; then 
        continue
    fi
    echo "$fname"

    if [ "$1" == "-u" ]; then
        $fname -h | sed 's/\(.*\)/    \1/'
    fi
done


for file in $(find $python_path -type f -name "*.py"); do
    fname=$(basename $file)
    echo "$fname"
    if [ "$1" == "-u" ]; then
        python $file -h | sed 's/\(.*\)/    \1/'
    fi
done

