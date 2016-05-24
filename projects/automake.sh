#!/bin/bash

NAS_FILES=(`find . -name \*.nas -print | sort`)

echo "Please add following statements in configure.ac"
echo "-----------------------------------------------"
echo "AC_CONFIG_FILES(["

for FILE in ${NAS_FILES[@]}
do
    FILEPATH=$FILE
    FILENAME=${FILE%.*}
    FILE_DIR=`dirname ${FILEPATH}`
    INDEX=`basename ${FILE_DIR}`
    BIN_NAME="${INDEX}_"`basename ${FILENAME}`
    SRC_NAME=`basename ${FILENAME}`

    echo -e "#"                                                 > ${FILE_DIR}/Makefile.am
    echo -e "# Generated `date` "                              >> ${FILE_DIR}/Makefile.am
    echo -e "#"                                                >> ${FILE_DIR}/Makefile.am
    echo -e ""                                                 >> ${FILE_DIR}/Makefile.am
    echo -e "NASK = \$(top_builddir)/src/opennask\$(EXEEXT)"   >> ${FILE_DIR}/Makefile.am
    echo -e ""                                             >> ${FILE_DIR}/Makefile.am
    echo -e "bin_PROGRAMS = ${NASK} ${BIN_NAME}.img"       >> ${FILE_DIR}/Makefile.am
    echo -e "${BIN_NAME}_img_SOURCES = ${SRC_NAME}.nas"    >> ${FILE_DIR}/Makefile.am
    echo -e ""                                             >> ${FILE_DIR}/Makefile.am
    echo -e "${BIN_NAME}.img\$(EXEEXT) : ${SRC_NAME}.nas"  >> ${FILE_DIR}/Makefile.am
    echo -e "\t\t\$(NASK) ${SRC_NAME}.nas ${BIN_NAME}.img" >> ${FILE_DIR}/Makefile.am
    echo -e ""                                             >> ${FILE_DIR}/Makefile.am

    AC_CONFIG_FILE=`echo ${FILE_DIR}/Makefile | sed -e 's/^\./projects/g'`
    echo -e "\t${AC_CONFIG_FILE}"
done

echo "])"
