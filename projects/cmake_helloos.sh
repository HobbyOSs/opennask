#!/bin/bash

# 初期のアセンブラのみの構成を処理する
HELLO_OSS=(`find . -name helloos.nas | sort -u`)

for HELLO_OS in ${HELLO_OSS[@]}
do
    HELLO_OS=`echo ${HELLO_OS} | ${SED} -e 's|^\.\/||'`
    BIN_FILE=`echo ${HELLO_OS} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/\.img/g'`
    HRB_FILE=`echo ${HELLO_OS} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/\.hrb/g'`

    WINE_BIN_FILE=`echo ${HELLO_OS} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/_wine\.img/g'`
    TARGET_NAME=`echo ${HELLO_OS} | ${SED} -e 's|\/|_|g' | ${SED} -e 's/\.nas//g'`

    OS_FILE=`echo ${HELLO_OS} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/os\.img/g'`
    SYS_FILE=`echo ${HELLO_OS} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/os\.sys/g'`
    IPL_FILE=`echo ${HELLO_OS} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/ipl\.bin/g'`
    TARGET_OS_NAME=`echo ${TARGET_NAME} | ${SED} -e 's|_asmhead||g'`

    NAS_DIR=(`echo ${HELLO_OS} | ${SED} -r 's|/[^/]+$||' | ${SED} -e 's|^\.\/||'`)
    NAS_DIR_TARGET=(`echo ${HELLO_OS} | ${SED} -r 's|/[^/]+$||' | ${SED} -e 's|^\.\/||' | ${SED} -e 's|\/|_|g'`)
    CMAKELISTS="${NAS_DIR}/CMakeLists.txt"

    BINARY_NAME=`echo ${HELLO_OS} | xargs basename | ${SED} -e 's/.nas/.img/g'`
    HRB_NAME=`echo ${HELLO_OS} | xargs basename | ${SED} -e 's/.nas/.hrb/g'`
    WINE_BINARY_NAME=`echo ${HELLO_OS} | xargs basename | ${SED} -e 's/.nas/_wine.img/g'`

    if [ -e ${CMAKELISTS} ]; then
	echo "#----------------------------------------------------------"                            | tee    ${CMAKELISTS}
	echo "message(STATUS \"Entering directory projects/${NAS_DIR}/\")"                            | tee -a ${CMAKELISTS}
	echo ""								                              | tee -a ${CMAKELISTS}
        echo "set(NASK \${root_BINARY_DIR}/src/opennask)"                                             | tee -a ${CMAKELISTS}
        echo "set(${NAS_DIR_TARGET}_OS    \${root_BINARY_DIR}/projects/${NAS_DIR}/os.img)"            | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_SYS	  \${root_BINARY_DIR}/projects/${NAS_DIR}/os.sys)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_IPLB  \${root_BINARY_DIR}/projects/${NAS_DIR}/ipl.bin)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_IPLS  \${root_SOURCE_DIR}/projects/${NAS_DIR}/ipl10.nas)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_HEADB \${root_BINARY_DIR}/projects/${NAS_DIR}/asmhead.bin)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_HEADS \${root_SOURCE_DIR}/projects/${NAS_DIR}/asmhead.nas)"	      | tee -a ${CMAKELISTS}
	echo ""		        							              | tee -a ${CMAKELISTS}
	echo ""		        							              | tee -a ${CMAKELISTS}
        echo "add_custom_target(${TARGET_OS_NAME}_run"                                                | tee -a ${CMAKELISTS}
        echo "  COMMAND \${QEMU} \${QEMUOPT} \${${NAS_DIR_TARGET}_OS}"                                | tee -a ${CMAKELISTS}
        echo "  DEPENDS ${NAS_DIR_TARGET}_img"                                                        | tee -a ${CMAKELISTS}
        echo ")"                                                                                      | tee -a ${CMAKELISTS}
        echo "add_custom_target(${TARGET_OS_NAME}_debug"                                              | tee -a ${CMAKELISTS}
        echo "  COMMAND \${QEMU} -s -S \${QEMUOPT} \${${NAS_DIR_TARGET}_OS} -redir tcp:5555:127.0.0.1:1234 &"     | tee -a ${CMAKELISTS}
        echo "  DEPENDS ${NAS_DIR_TARGET}_img"                                                        | tee -a ${CMAKELISTS}
        echo ")"                                                                                      | tee -a ${CMAKELISTS}
        echo "add_custom_target(${TARGET_OS_NAME}_clean"                                              | tee -a ${CMAKELISTS}
        echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_OS}"                                               | tee -a ${CMAKELISTS}
	echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_SYS}"                                              | tee -a ${CMAKELISTS}
	echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_IPLB}"                                             | tee -a ${CMAKELISTS}
	echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_HEADB}"                                            | tee -a ${CMAKELISTS}
	echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_BOOTB}"                                            | tee -a ${CMAKELISTS}
	echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_WILDOBJ}"                                          | tee -a ${CMAKELISTS}
        echo ")"                                                                                      | tee -a ${CMAKELISTS}
        echo "add_custom_target(${TARGET_OS_NAME}_ipl"                                                | tee -a ${CMAKELISTS}
        echo "  COMMAND \${NASK} \${${NAS_DIR_TARGET}_IPLS} \${${NAS_DIR_TARGET}_IPLB}"	              | tee -a ${CMAKELISTS}
	echo ")"                                                                                      | tee -a ${CMAKELISTS}
	echo "add_custom_target(${TARGET_OS_NAME}_sys"                                                | tee -a ${CMAKELISTS}
        echo "  COMMAND \${NASK} \${${NAS_DIR_TARGET}_HEADS} \${${NAS_DIR_TARGET}_HEADB}"             | tee -a ${CMAKELISTS}
        echo "  COMMAND cat \${${NAS_DIR_TARGET}_HEADB} \${${NAS_DIR_TARGET}_BOOTB} > \${${NAS_DIR_TARGET}_SYS}" | tee -a ${CMAKELISTS}
        echo "  DEPENDS ${NAS_DIR_TARGET}_ipl"                                                        | tee -a ${CMAKELISTS}
        echo ")"                                                                                      | tee -a ${CMAKELISTS}
        echo "add_custom_target(${TARGET_OS_NAME}_img"                                                | tee -a ${CMAKELISTS}
        echo "  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B \${${NAS_DIR_TARGET}_IPLB} -i \${${NAS_DIR_TARGET}_OS}" | tee -a ${CMAKELISTS}
        echo "  COMMAND mcopy -i \${${NAS_DIR_TARGET}_OS} \${${NAS_DIR_TARGET}_SYS} ::"               | tee -a ${CMAKELISTS}
        echo "  DEPENDS ${NAS_DIR_TARGET}_sys"                                                        | tee -a ${CMAKELISTS}
        echo ")"                                                                                      | tee -a ${CMAKELISTS}

	echo "########### next target ###############"                                                | tee -a ${CMAKELISTS}
    fi
    echo "set(${BINARY_NAME}_SRCS \${root_SOURCE_DIR}/projects/${HELLO_OS})"                          | tee -a ${CMAKELISTS}
    echo "set(${BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${BIN_FILE})"                          | tee -a ${CMAKELISTS}

    echo "set(${WINE_BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${WINE_BIN_FILE})"                | tee -a ${CMAKELISTS}
    echo ""                                                                                           | tee -a ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}"                                                           | tee -a ${CMAKELISTS}
    echo "  COMMAND \${root_BINARY_DIR}/src/opennask \${${BINARY_NAME}_SRCS} \${${BINARY_NAME}_OUTS}" | tee -a ${CMAKELISTS}
    echo ")"                                                                                          | tee -a ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}_wine"                                                      | tee -a ${CMAKELISTS}
    echo "  COMMAND \${WINE} \${WINE_NASK} \${${BINARY_NAME}_SRCS} \${${WINE_BINARY_NAME}_OUTS}"      | tee -a ${CMAKELISTS}
    echo ")"                                                                                          | tee -a ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}_od"                                                        | tee -a ${CMAKELISTS}
    echo "  COMMAND \${OD} -t x1 \${${BINARY_NAME}_OUTS}      > \${${BINARY_NAME}_OUTS}_f.txt"        | tee -a ${CMAKELISTS}
    echo "  COMMAND \${OD} -t x1 \${${WINE_BINARY_NAME}_OUTS} > \${${WINE_BINARY_NAME}_OUTS}_t.txt"   | tee -a ${CMAKELISTS}
    echo "  COMMAND diff -s \${${BINARY_NAME}_OUTS}_f.txt \${${WINE_BINARY_NAME}_OUTS}_t.txt"         | tee -a ${CMAKELISTS}
    echo ")"                                                                                          | tee -a ${CMAKELISTS}
    echo ""                                                                                           | tee -a ${CMAKELISTS}
    echo "add_dependencies(${TARGET_NAME}_od ${TARGET_NAME})"                                         | tee -a ${CMAKELISTS}
    echo "add_dependencies(${TARGET_NAME}_od ${TARGET_NAME}_wine)"                                    | tee -a ${CMAKELISTS}
    echo ""                                                                                           | tee -a ${CMAKELISTS}
    echo "add_dependencies(images ${TARGET_NAME})"                                                    | tee -a ${CMAKELISTS}
    echo "add_dependencies(wine ${TARGET_NAME}_wine)"                                                 | tee -a ${CMAKELISTS}
    echo "add_dependencies(od ${TARGET_NAME}_od)"                                                     | tee -a ${CMAKELISTS}
    echo ""                                                                                           | tee -a ${CMAKELISTS}
    echo "#----------------------------------------------------------"                                | tee -a ${CMAKELISTS}
done
