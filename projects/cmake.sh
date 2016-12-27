#!/bin/bash

if [ "$(uname)" == 'Darwin' ]; then
    SED='gsed'
else
    SED='sed'
fi

SUB_DIRS=(`find . -name \*.nas | ${SED} -e 's|\.\/||' -e 's|\/.*||' | sort -u`)
NAS_DIRS=(`find . -name \*.nas | ${SED} -r 's|/[^/]+$||'| sort -u`)

echo "Please add following statements in CMakeLists.txt"
echo "-------------------------------------------------"

for SUB_DIR in ${SUB_DIRS[@]}
do
    echo "add_subdirectory(${SUB_DIR})"
done
echo ""

echo "-------------------------------------------------"
echo "Debugging...creating subdir's CMakeLists.txt"
echo "-------------------------------------------------"
echo ""

for SUB_DIR in ${SUB_DIRS[@]}
do
    CHILD_DIRS=(`printf -- '%s\n' "${NAS_DIRS[@]}" | grep ${SUB_DIR}`)
    CMAKELISTS="${SUB_DIR}/CMakeLists.txt"

    echo "#----------------------------------------------------------" | tee    ${CMAKELISTS}
    echo "message(STATUS \"Entering directory projects/${SUB_DIR}/\")" | tee -a ${CMAKELISTS}
    for CHILD_DIR in ${CHILD_DIRS[@]}
    do
	CHILD_STR=`echo ${CHILD_DIR} | ${SED} -e 's|\.\/.*\/||' `
	echo "add_subdirectory(${CHILD_STR})" | tee -a ${CMAKELISTS}
    done
    echo ""
done

echo "-------------------------------------------------"
echo "Debugging...creating child CMakeLists.txt"
echo "-------------------------------------------------"
echo ""

NAS_FILES=(`find . -name \*.nas | grep -v asmhead.nas | grep -v helloos.nas | sort -u`)
for NAS_FILE in ${NAS_FILES[@]}
do
    NAS_DIR=(`echo ${NAS_FILE} | ${SED} -r 's|/[^/]+$||' | ${SED} -e 's|^\.\/||'`)
    CMAKELISTS="${NAS_DIR}/CMakeLists.txt"
    rm -f ${CMAKELISTS}
done

bash ./cmake_helloos.sh
bash ./cmake_02day.sh
bash ./cmake_until_20day.sh 03_day
bash ./cmake_until_20day.sh 04_day
bash ./cmake_until_20day.sh 05_day
bash ./cmake_until_20day.sh 06_day
bash ./cmake_until_20day.sh 07_day
bash ./cmake_until_20day.sh 08_day
bash ./cmake_until_20day.sh 09_day
bash ./cmake_until_20day.sh 10_day
bash ./cmake_until_20day.sh 11_day
bash ./cmake_until_20day.sh 12_day
bash ./cmake_until_20day.sh 13_day
bash ./cmake_until_20day.sh 14_day
bash ./cmake_until_20day.sh 15_day
bash ./cmake_until_20day.sh 16_day
bash ./cmake_until_20day.sh 17_day
bash ./cmake_until_20day.sh 18_day
bash ./cmake_until_20day.sh 19_day
bash ./cmake_until_20day.sh 20_day

exit 0
