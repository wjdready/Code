#!/bin/bash

pid=$(ps -e | awk "\$4==\"tf_state.sh\" {print \$1}")

if [ "$pid" ]; then
    kill $pid
fi

