testFolder="/data/tests";
scriptname="singletests.js"

# Dependencies
# ----------------------------------------------------------------------------------
# @param string $1  #   Input string.
# @param int $2     #   Cut an amount of characters from left side of string.
# @param int [$3]   #   Leave an amount of characters in the truncated string.
substr()
{
    local length=${3}

    if [ -z "${length}" ]; then
        length=$((${#1} - ${2}))
    fi

    local str=${1:${2}:${length}}

    if [ "${#str}" -eq "${#1}" ]; then
        echo "${1}"
    else
        echo "${str}"
    fi
}

# @param string $1  #   Input string.
# @param string $2  #   String that will be searched in input string.
# @param int [$3]   #   Offset of an input string.
strpos()
{
    local str=${1}
    local offset=${3}

    if [ -n "${offset}" ]; then
        str=`substr "${str}" ${offset}`
    else
        offset=0
    fi

    str=${str/${2}*/}

    if [ "${#str}" -eq "${#1}" ]; then
        return 0
    fi

    echo $((${#str}+${offset}))
}
# ----------------------------------------------------------------------------------


echo "Scanning GTests in ".$testFolder."..";

gTestSuitesNames=();
declare -A gTestSuites;

for f in $testFolder/src/GeneralStateTestsFiller/*; do
    if [ -d "$f" ]; then
        gTestSuitesNames+=("$(basename -- $f)");
    fi
done

for suite in ${gTestSuitesNames[@]}; do
    gTestNames="";
    for f in $testFolder/src/GeneralStateTestsFiller/$suite/*; do
        if [ -f "$f" ]; then
            testname=$(basename -- $f);
            index=$(strpos "${testname}" "Filler.");
            if [[ $index -eq 0 ]]; then
                continue;
            fi
            name=$(substr "${testname}" 0 $index);
            gTestNames+=$name"#";
        fi
    done
    gTestSuites[$suite]=$gTestNames;
done


# Generate the script 
script="";
script+="function singletests() { \n";
script+="    document.requesthelper.singletest.options.length=0 \n";



for suite in ${!gTestSuites[@]}; do
    script+="    if (document.requesthelper.gtests.options[document.requesthelper.gtests.selectedIndex].text == \"$suite\") { \n";

    tests=$(echo ${gTestSuites[$suite]} | tr "#" "\n")
    for test in $tests; do 
        script+="        var opt = document.createElement(\"option\")\n";
        script+="        opt.value = \"$test\"\n";
        script+="        opt.text = \"$test\"\n";
        script+="        document.requesthelper.singletest.add(opt, null) \n\n";
        #echo $suite" - "$test;
    done

    script+="    }\n"
done

script+="}\n";
script+="function testinit() { \n";
script+="   document.requesthelper.gtests.options.length=0 \n";

for suite in ${!gTestSuites[@]}; do
    script+="        var opt = document.createElement(\"option\")\n";
    script+="        opt.value = \"$suite\"\n";
    script+="        opt.text = \"$suite\"\n";
    if [[ "$suite" == "stExample" ]]; then
        script+="        opt.selected = true\n";
    fi
    script+="        document.requesthelper.gtests.add(opt, null) \n\n";
done

script+="    singletests() \n";
script+="}\n";

echo -e $script > "./"$scriptname;

