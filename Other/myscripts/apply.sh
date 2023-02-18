#!bash

shell_path=~/.myscripts/shell
python_path=~/.myscripts/python

rm $shell_path/*
rm $python_path/*

for file in $(find shell -type f -name "*.sh"); do
    fname=$(basename $file)
    echo "copy shell/$fname to $shell_path/myshell_$fname"
    cp $file $shell_path/myshell_$fname
done


for file in $(find python -type f -name "*.py"); do
    fname=$(basename $file)
    echo "copy python/$fname to $python_path/mypython_$fname"
    cp $file $python_path/mypython_$fname
done
