#!/bin/bash

if [ "$(uname)" == 'Darwin' ]; then
    SED='gsed'
else
    SED='sed'
fi

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

    echo "#----------------------------------------------------------"                             > ${CMAKELISTS}
    echo "message(STATUS \"Entering directory projects/${NAS_DIR}/\")"                             >> ${CMAKELISTS}
    echo ""								                           >> ${CMAKELISTS}
    # NASK 変数設定は不要。トップレベルの RAKUSK_EXECUTABLE を直接参照する
    echo "set(${NAS_DIR_TARGET}_SRC \${root_SOURCE_DIR}/projects/${HELLO_OS})"                     >> ${CMAKELISTS}
    echo "set(${NAS_DIR_TARGET}_OS \${root_BINARY_DIR}/projects/${NAS_DIR}/os.img)"                >> ${CMAKELISTS}
    echo "set(FONT \${root_BINARY_DIR}/src/makefont)"                                            >> ${CMAKELISTS}
    echo "set(B2O  \${root_BINARY_DIR}/src/bin2obj)"                                             >> ${CMAKELISTS}
    echo ""		        							           >> ${CMAKELISTS}
    echo "add_custom_target(${TARGET_OS_NAME}_run"                                                 >> ${CMAKELISTS}
    echo "  COMMAND \${QEMU} \${QEMUOPT} \${${NAS_DIR_TARGET}_OS}"                                 >> ${CMAKELISTS}
    echo "  DEPENDS ${TARGET_OS_NAME}_img"                                                         >> ${CMAKELISTS}
    echo ")"                                                                                       >> ${CMAKELISTS}
    echo "add_custom_target(${TARGET_OS_NAME}_debug"                                               >> ${CMAKELISTS}
    echo "  COMMAND \${QEMU} -s -S \${QEMUOPT} \${${NAS_DIR_TARGET}_OS} \${QEMU_DEBUG_OPT}"        >> ${CMAKELISTS}
    echo "  DEPENDS ${TARGET_OS_NAME}_img"                                                         >> ${CMAKELISTS}
    echo ")"                                                                                       >> ${CMAKELISTS}
    echo "add_custom_target(${TARGET_OS_NAME}_clean"                                               >> ${CMAKELISTS}
    echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_OS}"                                                >> ${CMAKELISTS}
    echo ")"                                                                                       >> ${CMAKELISTS}
    echo "add_custom_target(${TARGET_OS_NAME}_img"                                                 >> ${CMAKELISTS}
    # rakusk <source> <output> 形式
    echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${NAS_DIR_TARGET}_SRC} \${${NAS_DIR_TARGET}_OS}"      >> ${CMAKELISTS}
    echo "  DEPENDS \${${NAS_DIR_TARGET}_SRC}"                                                     >> ${CMAKELISTS} # 依存関係
    echo ")"                                                                                       >> ${CMAKELISTS}
    echo "########### next target ###############"                                                 >> ${CMAKELISTS}
    echo "set(${BINARY_NAME}_SRCS \${root_SOURCE_DIR}/projects/${HELLO_OS})"                           >> ${CMAKELISTS}
    echo "set(${BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${BIN_FILE})"                           >> ${CMAKELISTS}
    echo "set(${WINE_BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${WINE_BIN_FILE})"                 >> ${CMAKELISTS}
    echo ""                                                                                            >> ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}"                                                            >> ${CMAKELISTS}
    # rakusk <source> <output> 形式
    echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${BINARY_NAME}_SRCS} \${${BINARY_NAME}_OUTS}"            >> ${CMAKELISTS}
    echo "  DEPENDS \${${BINARY_NAME}_SRCS}"                                                         >> ${CMAKELISTS} # 依存関係
    echo ")"                                                                                           >> ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}_wine"                                                       >> ${CMAKELISTS}
    echo "  COMMAND \${WINE} \${WINE_NASK} \${${BINARY_NAME}_SRCS} \${${WINE_BINARY_NAME}_OUTS}"       >> ${CMAKELISTS}
    echo ")"                                                                                           >> ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}_od"                                                         >> ${CMAKELISTS}
    echo "  COMMAND \${OD} -t x1 \${${BINARY_NAME}_OUTS}      > \${${BINARY_NAME}_OUTS}_f.txt"         >> ${CMAKELISTS}
    echo "  COMMAND \${OD} -t x1 \${${WINE_BINARY_NAME}_OUTS} > \${${WINE_BINARY_NAME}_OUTS}_t.txt"    >> ${CMAKELISTS}
    echo "  COMMAND diff -s \${${BINARY_NAME}_OUTS}_f.txt \${${WINE_BINARY_NAME}_OUTS}_t.txt"          >> ${CMAKELISTS}
    echo ")"                                                                                           >> ${CMAKELISTS}
    echo ""                                                                                            >> ${CMAKELISTS}
    echo "add_dependencies(${TARGET_NAME}_od ${TARGET_NAME})"                                          >> ${CMAKELISTS}
    echo "add_dependencies(${TARGET_NAME}_od ${TARGET_NAME}_wine)"                                     >> ${CMAKELISTS}
    echo ""                                                                                            >> ${CMAKELISTS}
    echo "add_dependencies(images ${TARGET_NAME})"                                                     >> ${CMAKELISTS}
    echo "add_dependencies(wine ${TARGET_NAME}_wine)"                                                  >> ${CMAKELISTS}
    echo "add_dependencies(od ${TARGET_NAME}_od)"                                                      >> ${CMAKELISTS}
done
