#!/bin/sh

# 可以自动添加可执行文件依赖

notfounddlls='KERNEL32.dll'
dllbase=/mingw64/bin/

nc=1
while [ $nc -gt 0 ]; do
    nc=0
    for f in *.exe *.dll; do
        for dep in $(strings $f | grep -i '\.dll$'); do
            if [ ! -e $dep ]; then
                echo $notfounddlls | grep -iw $dep >/dev/null
                if [ $? -ne 0 ]; then
                    dllloc=$(find $dllbase -iname $dep)
                    if [ ! -z $dllloc ]; then
                        cp $dllloc .
                        echo "Copying "$(basename $dllloc)
                        nc=$(($nc + 1))
                    else
                        notfounddlls="$notfounddlls $dep"
                    fi
                fi
            fi
        done
    done
done

echo "System DLLS: "$notfounddlls
