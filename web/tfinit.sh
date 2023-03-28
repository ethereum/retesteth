#!/bin/bash
if [ -z "$PYSPECS_PATH" ]
then
    >&2 echo "Error: env variable 'PYSPECS_PATH' is not set!"
    exit 1;
fi

cd $PYSPECS_PATH
python3 -m venv ./venv/
source ./venv/bin/activate

tf $1 $2 $3 $4 $5 $6 $7 $8 $9 $10 $11 $12 $13 $14 $15 $16 $17 $18 $19 $20 $21 $22 $23 $24 $25 $26
