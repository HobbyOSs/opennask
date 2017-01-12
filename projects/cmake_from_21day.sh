#!/bin/bash

if [ "$(uname)" == 'Darwin' ]; then
    SED='gsed'
else
    SED='sed'
fi

# ２１日目〜３０日目まで処理
# アルファベット順で邪魔な a_nask.nasを除去（なんやねんこのファイル名）
NAS_FILES=(`find ./${1} -name \*.nas | grep -v a_nask | sort -u`)

for NAS_FILE in ${NAS_FILES[@]}
do
    NAS_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||'`
    BIN_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/\.img/g'`
    HRB_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/\.hrb/g'`

    WINE_BIN_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/_wine\.img/g'`
    TARGET_NAME=`echo ${NAS_FILE} | ${SED} -e 's|\/|_|g' | ${SED} -e 's/\.nas//g'`

    OS_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/os\.img/g'`
    SYS_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/os\.sys/g'`
    IPL_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/ipl\.bin/g'`
    TARGET_OS_NAME=`echo ${TARGET_NAME} | ${SED} -e 's|_asmhead||g'`

    NAS_DIR=(`echo ${NAS_FILE} | ${SED} -r 's|/[^/]+$||' | ${SED} -e 's|^\.\/||'`)
    NAS_DIR_TARGET=(`echo ${NAS_FILE} | ${SED} -r 's|/[^/]+$||' | ${SED} -e 's|^\.\/||' | ${SED} -e 's|\/|_|g'`)
    CMAKELISTS="${NAS_DIR}/CMakeLists.txt"

    BINARY_NAME=`echo ${NAS_FILE} | xargs basename | ${SED} -e 's/.nas/.img/g'`
    HRB_NAME=`echo ${NAS_FILE} | xargs basename | ${SED} -e 's/.nas/.hrb/g'`
    WINE_BINARY_NAME=`echo ${NAS_FILE} | xargs basename | ${SED} -e 's/.nas/_wine.img/g'`

    if [ -e ${CMAKELISTS} ]; then
	echo "########### next target ###############"                                                | tee -a ${CMAKELISTS}
    else
	echo "#----------------------------------------------------------"                            | tee    ${CMAKELISTS}
	echo "message(STATUS \"Entering directory projects/${NAS_DIR}/\")"                            | tee -a ${CMAKELISTS}
	echo ""								                              | tee -a ${CMAKELISTS}
        echo "set(NASK \${root_BINARY_DIR}/src/opennask)"                                             | tee -a ${CMAKELISTS}
        echo "set(FONT \${root_BINARY_DIR}/src/makefont)"                                             | tee -a ${CMAKELISTS}
        echo "set(B2O  \${root_BINARY_DIR}/src/bin2obj)"                                              | tee -a ${CMAKELISTS}
	echo "set(CONV \${root_BINARY_DIR}/objconv/objconv)"                                          | tee -a ${CMAKELISTS}
        echo "set(${NAS_DIR_TARGET}_OS    \${root_BINARY_DIR}/projects/${NAS_DIR}/os.img)"            | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_SYS	  \${root_BINARY_DIR}/projects/${NAS_DIR}/os.sys)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_IPLB  \${root_BINARY_DIR}/projects/${NAS_DIR}/ipl.bin)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_IPLS  \${root_SOURCE_DIR}/projects/${NAS_DIR}/ipl10.nas)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_HEADB \${root_BINARY_DIR}/projects/${NAS_DIR}/asmhead.bin)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_HEADS \${root_SOURCE_DIR}/projects/${NAS_DIR}/asmhead.nas)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_BOOTB \${root_BINARY_DIR}/projects/${NAS_DIR}/boot.bin)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_CCO	  \${root_BINARY_DIR}/projects/${NAS_DIR}/boot.o)"	      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_CCS	  \${root_SOURCE_DIR}/projects/${NAS_DIR}/*.c)"		      | tee -a ${CMAKELISTS}
	echo "set(${NAS_DIR_TARGET}_LDS	  \${root_SOURCE_DIR}/projects/os.lds)"		              | tee -a ${CMAKELISTS}
	if [ -e "${NAS_DIR}/naskfunc.nas" ]; then
	    echo "set(${NAS_DIR_TARGET}_FUNCS \${root_SOURCE_DIR}/projects/${NAS_DIR}/naskfunc.nas)"  | tee -a ${CMAKELISTS}
	    echo "set(${NAS_DIR_TARGET}_FUNCO \${root_BINARY_DIR}/projects/${NAS_DIR}/naskfunc.o)"    | tee -a ${CMAKELISTS}
	fi
	if [ -e "${NAS_DIR}/hankaku.txt" ]; then
	    echo "set(${NAS_DIR_TARGET}_FONTS \${root_SOURCE_DIR}/projects/${NAS_DIR}/hankaku.txt)"   | tee -a ${CMAKELISTS}
	    echo "set(${NAS_DIR_TARGET}_FONTB \${root_BINARY_DIR}/projects/${NAS_DIR}/hankaku.bin)"   | tee -a ${CMAKELISTS}
	    echo "set(${NAS_DIR_TARGET}_FONTO \${root_BINARY_DIR}/projects/${NAS_DIR}/hankaku.o)"     | tee -a ${CMAKELISTS}
	    # for golibc
            echo "set(${NAS_DIR_TARGET}_LIBGE \${root_BINARY_DIR}/golibc/libgo.a)"                    | tee -a ${CMAKELISTS}
            echo "set(${NAS_DIR_TARGET}_LIBGC \${root_BINARY_DIR}/projects/${NAS_DIR}/libgo.o)"       | tee -a ${CMAKELISTS}
	fi
	echo "set(${NAS_DIR_TARGET}_WILDOBJ \${root_BINARY_DIR}/projects/${NAS_DIR}/*.o)"	      | tee -a ${CMAKELISTS}
	echo ""		        							              | tee -a ${CMAKELISTS}
	echo ""		        							              | tee -a ${CMAKELISTS}
        echo "add_custom_target(${TARGET_OS_NAME}_run"                                                | tee -a ${CMAKELISTS}
        echo "  COMMAND \${QEMU} \${QEMUOPT} \${${NAS_DIR_TARGET}_OS}"                                | tee -a ${CMAKELISTS}
        echo "  DEPENDS ${NAS_DIR_TARGET}_img"                                                        | tee -a ${CMAKELISTS}
        echo ")"                                                                                      | tee -a ${CMAKELISTS}
        echo "add_custom_target(${TARGET_OS_NAME}_debug"                                              | tee -a ${CMAKELISTS}
        echo "  COMMAND \${QEMU} -s -S \${QEMUOPT} \${${NAS_DIR_TARGET}_OS} \${QEMU_DEBUG_OPT}"       | tee -a ${CMAKELISTS}
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
	if [ -e "${NAS_DIR}/hankaku.txt" ]; then
	    echo "  COMMAND \${FONT} \${${NAS_DIR_TARGET}_FONTS} \${${NAS_DIR_TARGET}_FONTB}"              | tee -a ${CMAKELISTS}
	    echo "  COMMAND \${B2O}  \${${NAS_DIR_TARGET}_FONTB} \${${NAS_DIR_TARGET}_FONTO} _hankaku"     | tee -a ${CMAKELISTS}
	    echo "  COMMAND \${CONV} -fcoff32 -nu \${${NAS_DIR_TARGET}_LIBGE} \${${NAS_DIR_TARGET}_LIBGC}" | tee -a ${CMAKELISTS}
	fi
	if [ -e "${NAS_DIR}/naskfunc.nas" ]; then
            echo "  COMMAND \${NASK} \${${NAS_DIR_TARGET}_FUNCS} \${${NAS_DIR_TARGET}_FUNCO}"         | tee -a ${CMAKELISTS}
	    echo "  COMMAND gcc \${BINOPT} -T \${${NAS_DIR_TARGET}_LDS} \${${NAS_DIR_TARGET}_CCS} \${${NAS_DIR_TARGET}_WILDOBJ} -o \${${NAS_DIR_TARGET}_BOOTB}" | tee -a ${CMAKELISTS}
	else
	    echo "  COMMAND gcc \${BINOPT} -T \${${NAS_DIR_TARGET}_LDS} \${${NAS_DIR_TARGET}_CCS} -o \${${NAS_DIR_TARGET}_BOOTB}" | tee -a ${CMAKELISTS}
	fi
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
    echo "set(${BINARY_NAME}_SRCS \${root_SOURCE_DIR}/projects/${NAS_FILE})"                          | tee -a ${CMAKELISTS}
    echo "set(${BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${BIN_FILE})"                          | tee -a ${CMAKELISTS}

    echo "set(${WINE_BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${WINE_BIN_FILE})"                | tee -a ${CMAKELISTS}
    echo ""                                                                                           | tee -a ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}"                                                           | tee -a ${CMAKELISTS}
    echo "  COMMAND \${root_BINARY_DIR}/src/opennask \${${BINARY_NAME}_SRCS} \${${BINARY_NAME}_OUTS}" | tee -a ${CMAKELISTS}
    echo ")"                                                                                          | tee -a ${CMAKELISTS}
    # その他のnaskファイルのオブジェクト化, hdファイルのディレクトリに押し込む
    # mcopy -i ./fat.img ./myfile.bin ::/myfile.bin
    # This copies the file ./myfile.bin to the root of the filesystem contained in the file ./fat.img.
    if [[ $NAS_FILE != *naskfunc.nas ]] && [[ $NAS_FILE != *ipl10.nas ]] && [[ $NAS_FILE != *asmhead.nas ]]; then
	echo "set(${BINARY_NAME}_HRB \${root_BINARY_DIR}/projects/${HRB_FILE})"                           | tee -a ${CMAKELISTS}
	echo "add_custom_target(${TARGET_NAME}_hrb"                                                       | tee -a ${CMAKELISTS}
	echo "  COMMAND \${root_BINARY_DIR}/src/opennask \${${BINARY_NAME}_SRCS} \${${BINARY_NAME}_HRB}"  | tee -a ${CMAKELISTS}
        echo "  COMMAND mcopy -i \${${NAS_DIR_TARGET}_OS} \${${BINARY_NAME}_HRB} ::${HRB_NAME}"           | tee -a ${CMAKELISTS}
        echo "  DEPENDS ${NAS_DIR_TARGET}_img"                                                            | tee -a ${CMAKELISTS}
	echo ")"                                                                                          | tee -a ${CMAKELISTS}
	echo ""                                                                                           | tee -a ${CMAKELISTS}
	if [[ $NAS_DIR_TARGET != 01_* ]] && [[ $NAS_DIR_TARGET != 02_* ]] && [[ $NAS_DIR_TARGET != 03_* ]]; then
	    echo "add_dependencies(${NAS_DIR_TARGET}_run ${TARGET_NAME}_hrb)"				  | tee -a ${CMAKELISTS}
	fi
    fi
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


TAIL_FILES=(`find ./${1} -name \*.nas | grep a_nask | sort -u`)

for NAS_FILE in ${TAIL_FILES[@]}
do

    NAS_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||'`
    BIN_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/\.img/g'`
    HRB_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/\.hrb/g'`

    WINE_BIN_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/\.nas/_wine\.img/g'`
    TARGET_NAME=`echo ${NAS_FILE} | ${SED} -e 's|\/|_|g' | ${SED} -e 's/\.nas//g'`

    OS_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/os\.img/g'`
    SYS_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/os\.sys/g'`
    IPL_FILE=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||' | ${SED} -e 's/asmhead.nas/ipl\.bin/g'`
    TARGET_OS_NAME=`echo ${TARGET_NAME} | ${SED} -e 's|_asmhead||g'`

    NAS_DIR=(`echo ${NAS_FILE} | ${SED} -r 's|/[^/]+$||' | ${SED} -e 's|^\.\/||'`)
    NAS_DIR_TARGET=(`echo ${NAS_FILE} | ${SED} -r 's|/[^/]+$||' | ${SED} -e 's|^\.\/||' | ${SED} -e 's|\/|_|g'`)
    CMAKELISTS="${NAS_DIR}/CMakeLists.txt"

    BINARY_NAME=`echo ${NAS_FILE} | xargs basename | ${SED} -e 's/.nas/.img/g'`
    HRB_NAME=`echo ${NAS_FILE} | xargs basename | ${SED} -e 's/.nas/.hrb/g'`
    WINE_BINARY_NAME=`echo ${NAS_FILE} | xargs basename | ${SED} -e 's/.nas/_wine.img/g'`

    echo "########### next target ###############"                                                    | tee -a ${CMAKELISTS}
    echo "set(${BINARY_NAME}_SRCS \${root_SOURCE_DIR}/projects/${NAS_FILE})"                          | tee -a ${CMAKELISTS}
    echo "set(${BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${BIN_FILE})"                          | tee -a ${CMAKELISTS}

    echo "set(${WINE_BINARY_NAME}_OUTS \${root_BINARY_DIR}/projects/${WINE_BIN_FILE})"                | tee -a ${CMAKELISTS}
    echo ""                                                                                           | tee -a ${CMAKELISTS}
    echo "add_custom_target(${TARGET_NAME}"                                                           | tee -a ${CMAKELISTS}
    echo "  COMMAND \${root_BINARY_DIR}/src/opennask \${${BINARY_NAME}_SRCS} \${${BINARY_NAME}_OUTS}" | tee -a ${CMAKELISTS}
    echo ")"                                                                                          | tee -a ${CMAKELISTS}
    # その他のnaskファイルのオブジェクト化, hdファイルのディレクトリに押し込む
    # mcopy -i ./fat.img ./myfile.bin ::/myfile.bin
    # This copies the file ./myfile.bin to the root of the filesystem contained in the file ./fat.img.
    if [[ $NAS_FILE != *naskfunc.nas ]] && [[ $NAS_FILE != *ipl10.nas ]] && [[ $NAS_FILE != *asmhead.nas ]]; then
	echo "set(${BINARY_NAME}_HRB \${root_BINARY_DIR}/projects/${HRB_FILE})"                           | tee -a ${CMAKELISTS}
	echo "add_custom_target(${TARGET_NAME}_hrb"                                                       | tee -a ${CMAKELISTS}
	echo "  COMMAND \${root_BINARY_DIR}/src/opennask \${${BINARY_NAME}_SRCS} \${${BINARY_NAME}_HRB}"  | tee -a ${CMAKELISTS}
        echo "  COMMAND mcopy -i \${${NAS_DIR_TARGET}_OS} \${${BINARY_NAME}_HRB} ::${HRB_NAME}"           | tee -a ${CMAKELISTS}
        echo "  DEPENDS ${NAS_DIR_TARGET}_img"                                                            | tee -a ${CMAKELISTS}
	echo ")"                                                                                          | tee -a ${CMAKELISTS}
	echo ""                                                                                           | tee -a ${CMAKELISTS}
	echo "add_dependencies(${NAS_DIR_TARGET}_run ${TARGET_NAME}_hrb)"				  | tee -a ${CMAKELISTS}
    fi
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
