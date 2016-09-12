#!/bin/bash

SUB_DIRS=(`find . -name \*.nas | sed -e 's|\.\/||' -e 's|\/.*||' | sort -u`)
NAS_DIRS=(`find . -name \*.nas | sed -r 's|/[^/]+$||'| sort -u`)
NAS_FILES=(`find . -name \*.nas | sort -u`)

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
	CHILD_STR=`echo ${CHILD_DIR} | sed -e 's|\.\/.*\/||' `
	echo "add_subdirectory(${CHILD_STR})" | tee -a ${CMAKELISTS}
    done
    echo ""
done

echo "-------------------------------------------------"
echo "Debugging...creating child CMakeLists.txt"
echo "-------------------------------------------------"
echo ""

for NAS_FILE in ${NAS_FILES[@]}
do
    NAS_DIR=(`echo ${NAS_FILE} | sed -r 's|/[^/]+$||' | sed -e 's|^\.\/||'`)
    CMAKELISTS="${NAS_DIR}/CMakeLists.txt"
    rm -f ${CMAKELISTS}
done

for NAS_FILE in ${NAS_FILES[@]}
do
    NAS_FILE=`echo ${NAS_FILE} | sed -e 's|^\.\/||'`
    BIN_FILE=`echo ${NAS_FILE} | sed -e 's|^\.\/||' | sed -e 's/.nas/.img/g'`
    WINE_BIN_FILE=`echo ${NAS_FILE} | sed -e 's|^\.\/||' | sed -e 's/.nas/_wine.img/g'`
    TARGET_NAME=`echo ${NAS_FILE} | sed -e 's|\/|_|g' | sed -e 's/.nas//g'`

    NAS_DIR=(`echo ${NAS_FILE} | sed -r 's|/[^/]+$||' | sed -e 's|^\.\/||'`)
    CMAKELISTS="${NAS_DIR}/CMakeLists.txt"

    SOURCE_NAME=`echo ${NAS_FILE} | xargs basename`
    BINARY_NAME=`echo ${NAS_FILE} | xargs basename | sed -e 's/.nas/.img/g'`
    WINE_BINARY_NAME=`echo ${NAS_FILE} | xargs basename | sed -e 's/.nas/_wine.img/g'`

    if [ -e ${CMAKELISTS} ]; then
	echo "########### next target ###############"                       | tee -a ${CMAKELISTS}
    else
	echo "#----------------------------------------------------------"   | tee    ${CMAKELISTS}
	echo "message(STATUS \"Entering directory projects/${NAS_DIR}/\")"   | tee -a ${CMAKELISTS}
	echo ""								     | tee -a ${CMAKELISTS}
	echo "########### next target ###############"                       | tee -a ${CMAKELISTS}
    fi
    echo "set(${BINARY_NAME}_SRCS \${root_SOURCE_DIR}/projects/${NAS_FILE})" | tee -a ${CMAKELISTS}
    echo "set(${BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${BIN_FILE})" | tee -a ${CMAKELISTS}
    echo "set(${WINE_BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${WINE_BIN_FILE})" | tee -a ${CMAKELISTS}
    echo ""                                                                  | tee -a ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}"                                  | tee -a ${CMAKELISTS}
    echo "  COMMAND \${root_BINARY_DIR}/src/opennask \${${BINARY_NAME}_SRCS} \${${BINARY_NAME}_OUTS}" | tee -a ${CMAKELISTS}
    echo ")"                                                                 | tee -a ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}_wine"                             | tee -a ${CMAKELISTS}
    echo "  COMMAND \${WINE} \${WINE_NASK} \${${BINARY_NAME}_SRCS} \${${WINE_BINARY_NAME}_OUTS}" | tee -a ${CMAKELISTS}
    echo ")"                                                                 | tee -a ${CMAKELISTS}
    echo ""                                                                  | tee -a ${CMAKELISTS}
    echo "add_dependencies(images ${TARGET_NAME})"                           | tee -a ${CMAKELISTS}
    echo "add_dependencies(wine ${TARGET_NAME}_wine)"                        | tee -a ${CMAKELISTS}
    echo ""                                                                  | tee -a ${CMAKELISTS}
done
