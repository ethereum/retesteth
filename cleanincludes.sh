#!/bin/bash

trycompile() {
  cd ./build
  out=`make -j10 2>/dev/null`
  cd ..
  if [[ "$out" == *"recipe for target 'all' failed"* ]]; then
    return 0
  fi
  return 1
}

disableInclude() { 
  pos=$1
  file=$2
  cmd='s/#include/\/\/#include/'$pos
  sed -z -i $cmd $file 
}

enableInclude() { 
  pos=$1
  file=$2
  cmd='s/\/\/#include/#include/'$pos
  sed -z -i $cmd $file 
}

cleanIncludes () {
  file=$1
  countDIncludes=`grep -o "//#include" $file | wc -l`
  if [ countDIncludes > 0 ]; then
    echo "Found existing commented include! Skipping: "$file
    return
  fi
  countIncludes=`grep -o "#include" $file | wc -l`

  for (( c=1; c<=$countIncludes; c++ ))
  do
     disableInclude $c $file
     ((countDIncludes += 1))
     trycompile
     if [ $? == 1 ]; then   
        continue
     fi
     enableInclude $countDIncludes $file
     ((countDIncludes -= 1))
  done
}


folder=$1
sources=`find ./$1 | grep .cpp`

for eachfile in $sources
do 
    cleanIncludes $eachfile
    echo "Processed "$eachfile"..."
done

