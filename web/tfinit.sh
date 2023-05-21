#!/bin/bash
if [ -z "$PYSPECS_PATH" ]
then
    >&2 echo "Error: env variable 'PYSPECS_PATH' is not set!"
    exit 1;
fi

args=""
for var in "$@"
do
  args=$args" $var"
done

cd $PYSPECS_PATH
python3 -m venv ./venv/
source ./venv/bin/activate

tf $args
