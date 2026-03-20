#!/bin/bash

if [ "$(uname)" == 'Darwin' ]; then
    SED='gsed'
else
    SED='sed'
fi

# ３日目〜２０日目まで処理
NAS_FILES=(`find ./${1} -name \*.nas | sort -u`)

for NAS_FILE in ${NAS_FILES[@]}
do
    NAS_FILE_REL=`echo ${NAS_FILE} | ${SED} -e 's|^\.\/||'`
    # Use .bin suffix for intermediate files like ipl/asmhead for clarity, .img for others
    if [[ "${NAS_FILE_REL}" == */ipl*.nas ]] || [[ "${NAS_FILE_REL}" == */asmhead.nas ]]; then
        BIN_FILE=`echo ${NAS_FILE_REL} | ${SED} -e 's/\.nas/\.bin/g'`
        WINE_BIN_FILE=`echo ${NAS_FILE_REL} | ${SED} -e 's/\.nas/_wine\.bin/g'`
    else
        BIN_FILE=`echo ${NAS_FILE_REL} | ${SED} -e 's/\.nas/\.img/g'`
        WINE_BIN_FILE=`echo ${NAS_FILE_REL} | ${SED} -e 's/\.nas/_wine\.img/g'`
    fi
    HRB_FILE=`echo ${NAS_FILE_REL} | ${SED} -e 's/\.nas/\.hrb/g'`

    TARGET_NAME=`echo ${NAS_FILE_REL} | ${SED} -e 's|\/|_|g' | ${SED} -e 's/\.nas//g'`

    NAS_DIR=(`echo ${NAS_FILE_REL} | ${SED} -r 's|/[^/]+$||'`)
    NAS_DIR_TARGET=(`echo ${NAS_DIR} | ${SED} -e 's|\/|_|g'`)
    CMAKELISTS="${NAS_DIR}/CMakeLists.txt"

    # Use the correct suffix for variable names based on the file type
    if [[ "${NAS_FILE_REL}" == */ipl*.nas ]] || [[ "${NAS_FILE_REL}" == */asmhead.nas ]]; then
        BINARY_NAME=`echo ${NAS_FILE_REL} | xargs basename | ${SED} -e 's/.nas/.bin/g'`
        WINE_BINARY_NAME=`echo ${NAS_FILE_REL} | xargs basename | ${SED} -e 's/.nas/_wine.bin/g'`
    else
        BINARY_NAME=`echo ${NAS_FILE_REL} | xargs basename | ${SED} -e 's/.nas/.img/g'`
        WINE_BINARY_NAME=`echo ${NAS_FILE_REL} | xargs basename | ${SED} -e 's/.nas/_wine.img/g'`
    fi
    HRB_NAME=`echo ${NAS_FILE_REL} | xargs basename | ${SED} -e 's/.nas/.hrb/g'`


    # Determine the target name for the OS image/run target based on directory
    TARGET_OS_NAME="${NAS_DIR_TARGET}"

    if [ -e ${CMAKELISTS} ]; then
        echo "########### next target ###############"                                                 >> ${CMAKELISTS}
    else
        echo "#----------------------------------------------------------"                             > ${CMAKELISTS}
        echo "message(STATUS \"Entering directory projects/${NAS_DIR}/\")"                             >> ${CMAKELISTS}
        echo ""                                                                                        >> ${CMAKELISTS}

        # --- File existence checks ---
        HAS_IPL10=false
        HAS_ASMHEAD=false
        HAS_C=false
        HAS_IPL_ONLY=false # Specific check for ipl.nas only case
        if [ -e "${NAS_DIR}/ipl10.nas" ]; then HAS_IPL10=true; fi
        if [ -e "${NAS_DIR}/asmhead.nas" ]; then HAS_ASMHEAD=true; fi
        if ls "${NAS_DIR}"/*.c > /dev/null 2>&1; then HAS_C=true; fi
        # Check for the specific case of only ipl.nas existing
        if [ -e "${NAS_DIR}/ipl.nas" ] && ! ${HAS_IPL10} && ! ${HAS_ASMHEAD} && ! ${HAS_C}; then
             HAS_IPL_ONLY=true
             IPL_SOURCE_FILE="ipl.nas"
             IPL_OUTPUT_SUFFIX=".bin" # Output as .bin for direct execution
             TARGET_IPL_ONLY="${TARGET_OS_NAME}_ipl_only" # Unique target name
        elif ${HAS_IPL10} && ! ${HAS_ASMHEAD} && ! ${HAS_C}; then
             HAS_IPL_ONLY=true # Treat ipl10.nas only as IPL only case too
             IPL_SOURCE_FILE="ipl10.nas"
             IPL_OUTPUT_SUFFIX=".bin"
             TARGET_IPL_ONLY="${TARGET_OS_NAME}_ipl10_only"
        fi

        # --- Common variable settings ---
        echo "set(FONT \${RAKUSK_EXECUTABLE} --makefont)"                                            >> ${CMAKELISTS}
        echo "set(B2O  \${RAKUSK_EXECUTABLE} --bin2obj)"                                             >> ${CMAKELISTS}
        echo "set(CONV \${root_BINARY_DIR}/objconv/objconv)"                                           >> ${CMAKELISTS}
        echo "set(${NAS_DIR_TARGET}_WILDOBJ \${root_BINARY_DIR}/projects/${NAS_DIR}/*.o)"               >> ${CMAKELISTS}
        echo ""                                                                                        >> ${CMAKELISTS}

        # --- Conditional Build Rules ---

        # Case 1: Full OS Build (ipl10.nas + asmhead.nas + *.c)
        if ${HAS_IPL10} && ${HAS_ASMHEAD} && ${HAS_C}; then
            echo "# Full OS build rules"                                                               >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_OS    \${root_BINARY_DIR}/projects/${NAS_DIR}/os.img)"          >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_SYS   \${root_BINARY_DIR}/projects/${NAS_DIR}/os.sys)"          >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_IPLB  \${root_BINARY_DIR}/projects/${NAS_DIR}/ipl.bin)"         >> ${CMAKELISTS} # Output of ipl target is ipl.bin
            echo "set(${TARGET_OS_NAME}_IPLS  \${root_SOURCE_DIR}/projects/${NAS_DIR}/ipl10.nas)"       >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_HEADB \${root_BINARY_DIR}/projects/${NAS_DIR}/asmhead.bin)"     >> ${CMAKELISTS} # Output of asmhead command is asmhead.bin
            echo "set(${TARGET_OS_NAME}_HEADS \${root_SOURCE_DIR}/projects/${NAS_DIR}/asmhead.nas)"     >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_BOOTB \${root_BINARY_DIR}/projects/${NAS_DIR}/boot.bin)"        >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_CCO   \${root_BINARY_DIR}/projects/${NAS_DIR}/boot.o)"          >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_CCS   \${root_SOURCE_DIR}/projects/${NAS_DIR}/*.c)"             >> ${CMAKELISTS}
            echo "set(${TARGET_OS_NAME}_LDS   \${root_SOURCE_DIR}/projects/os.lds)"                    >> ${CMAKELISTS}
            # naskfunc, hankaku settings
            if [ -e "${NAS_DIR}/naskfunc.nas" ]; then
                echo "set(${TARGET_OS_NAME}_FUNCS \${root_SOURCE_DIR}/projects/${NAS_DIR}/naskfunc.nas)" >> ${CMAKELISTS}
                echo "set(${TARGET_OS_NAME}_FUNCO \${root_BINARY_DIR}/projects/${NAS_DIR}/naskfunc.o)"   >> ${CMAKELISTS}
            fi
            if [ -e "${NAS_DIR}/hankaku.txt" ]; then
                echo "set(${TARGET_OS_NAME}_FONTS \${root_SOURCE_DIR}/projects/${NAS_DIR}/hankaku.txt)"  >> ${CMAKELISTS}
                echo "set(${TARGET_OS_NAME}_FONTB \${root_BINARY_DIR}/projects/${NAS_DIR}/hankaku.bin)"  >> ${CMAKELISTS}
                echo "set(${TARGET_OS_NAME}_FONTO \${root_BINARY_DIR}/projects/${NAS_DIR}/hankaku.o)"    >> ${CMAKELISTS}
                echo "set(${TARGET_OS_NAME}_LIBGE \${root_BINARY_DIR}/golibc/libgo.a)"                   >> ${CMAKELISTS}
                echo "set(${TARGET_OS_NAME}_LIBGC \${root_BINARY_DIR}/projects/${NAS_DIR}/libgo.o)"      >> ${CMAKELISTS}
            fi
            echo ""                                                                                    >> ${CMAKELISTS}

            # run, debug, clean targets (using os.img)
            echo "add_custom_target(${TARGET_OS_NAME}_run"                                             >> ${CMAKELISTS}
            echo "  COMMAND \${QEMU} \${QEMUOPT} \${${TARGET_OS_NAME}_OS}"                             >> ${CMAKELISTS}
            echo "  DEPENDS ${TARGET_OS_NAME}_img"                                                     >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}
            echo "add_custom_target(${TARGET_OS_NAME}_debug"                                           >> ${CMAKELISTS}
            echo "  COMMAND \${QEMU} -s -S \${QEMUOPT} \${${TARGET_OS_NAME}_OS} \${QEMU_DEBUG_OPT}"    >> ${CMAKELISTS}
            echo "  DEPENDS ${TARGET_OS_NAME}_img"                                                     >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}
            echo "add_custom_target(${TARGET_OS_NAME}_clean"                                           >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${${TARGET_OS_NAME}_OS}"                                            >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${${TARGET_OS_NAME}_SYS}"                                           >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${${TARGET_OS_NAME}_IPLB}"                                          >> ${CMAKELISTS} # Clean ipl.bin
            echo "  COMMAND rm -f \${${TARGET_OS_NAME}_HEADB}"                                         >> ${CMAKELISTS} # Clean asmhead.bin
            echo "  COMMAND rm -f \${${TARGET_OS_NAME}_BOOTB}"                                         >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${${NAS_DIR_TARGET}_WILDOBJ}"                                       >> ${CMAKELISTS}
            # Clean individual .bin files generated for wine/od targets
            echo "  COMMAND rm -f \${root_BINARY_DIR}/projects/${NAS_DIR}/ipl10.bin"                   >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${root_BINARY_DIR}/projects/${NAS_DIR}/asmhead.bin"                 >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${root_BINARY_DIR}/projects/${NAS_DIR}/*_wine.bin"                  >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${root_BINARY_DIR}/projects/${NAS_DIR}/*.txt"                       >> ${CMAKELISTS} # Clean od output
            echo ")"                                                                                   >> ${CMAKELISTS}

            # ipl target (generates ipl.bin) - Renamed from _ipl to avoid conflict with individual ipl10 target
            echo "add_custom_target(${TARGET_OS_NAME}_ipl_build"                                       >> ${CMAKELISTS}
            echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${TARGET_OS_NAME}_IPLS} \${${TARGET_OS_NAME}_IPLB}" >> ${CMAKELISTS}
            echo "  DEPENDS \${${TARGET_OS_NAME}_IPLS}"                                                >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}

            # asmhead.bin generation command (OUTPUT is asmhead.bin)
            echo "add_custom_command("                                                                 >> ${CMAKELISTS}
            echo "  OUTPUT \${${TARGET_OS_NAME}_HEADB}"                                                >> ${CMAKELISTS}
            echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${TARGET_OS_NAME}_HEADS} \${${TARGET_OS_NAME}_HEADB}" >> ${CMAKELISTS}
            echo "  DEPENDS \${${TARGET_OS_NAME}_HEADS}"                                               >> ${CMAKELISTS}
            echo "  COMMENT \"Generating asmhead.bin for ${TARGET_OS_NAME}\""                          >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}
            echo ""                                                                                    >> ${CMAKELISTS}

            # sys target
            echo "add_custom_target(${TARGET_OS_NAME}_sys"                                             >> ${CMAKELISTS}
            if [ -e "${NAS_DIR}/hankaku.txt" ]; then
                echo "  COMMAND \${FONT} \${${TARGET_OS_NAME}_FONTS} \${${TARGET_OS_NAME}_FONTB}"       >> ${CMAKELISTS}
                echo "  COMMAND \${B2O}  \${${TARGET_OS_NAME}_FONTB} \${${TARGET_OS_NAME}_FONTO} _hankaku" >> ${CMAKELISTS}
                echo "  COMMAND \${CONV} -fcoff32 -nu \${${TARGET_OS_NAME}_LIBGE} \${${TARGET_OS_NAME}_LIBGC}" >> ${CMAKELISTS}
            fi
            if [ -e "${NAS_DIR}/naskfunc.nas" ]; then
                echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${TARGET_OS_NAME}_FUNCS} \${${TARGET_OS_NAME}_FUNCO}" >> ${CMAKELISTS}
                echo "  COMMAND gcc \${BINOPT} -T \${${TARGET_OS_NAME}_LDS} \${${TARGET_OS_NAME}_CCS} \${${NAS_DIR_TARGET}_WILDOBJ} -o \${${TARGET_OS_NAME}_BOOTB}" >> ${CMAKELISTS}
            else
                echo "  COMMAND gcc \${BINOPT} -T \${${TARGET_OS_NAME}_LDS} \${${TARGET_OS_NAME}_CCS} -o \${${TARGET_OS_NAME}_BOOTB}" >> ${CMAKELISTS}
            fi
            echo "  COMMAND cat \${${TARGET_OS_NAME}_HEADB} \${${TARGET_OS_NAME}_BOOTB} > \${${TARGET_OS_NAME}_SYS}" >> ${CMAKELISTS}
            echo "  DEPENDS \${${TARGET_OS_NAME}_HEADB} ${TARGET_OS_NAME}_ipl_build"                   >> ${CMAKELISTS} # Depends on asmhead.bin and the ipl_build target
            if [ -e "${NAS_DIR}/naskfunc.nas" ]; then
                 echo "  DEPENDS \${${TARGET_OS_NAME}_FUNCO}"                                          >> ${CMAKELISTS}
            fi
             if [ -e "${NAS_DIR}/hankaku.txt" ]; then
                 echo "  DEPENDS \${${TARGET_OS_NAME}_FONTO}"                                          >> ${CMAKELISTS}
             fi
            echo ")"                                                                                   >> ${CMAKELISTS}

            # img target
            echo "add_custom_target(${TARGET_OS_NAME}_img"                                             >> ${CMAKELISTS}
            echo "  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B \${${TARGET_OS_NAME}_IPLB} -i \${${TARGET_OS_NAME}_OS}" >> ${CMAKELISTS}
            echo "  COMMAND mcopy -i \${${TARGET_OS_NAME}_OS} \${${TARGET_OS_NAME}_SYS} ::"            >> ${CMAKELISTS}
            echo "  DEPENDS ${TARGET_OS_NAME}_sys"                                                     >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}

        # Case 2: IPL Only (ipl.nas or ipl10.nas exists alone)
        elif ${HAS_IPL_ONLY}; then
            echo "# IPL only build rules (${IPL_SOURCE_FILE})"                                         >> ${CMAKELISTS}
            IPL_SOURCE="\${root_SOURCE_DIR}/projects/${NAS_DIR}/${IPL_SOURCE_FILE}"
            IPL_OUTPUT="\${root_BINARY_DIR}/projects/${NAS_DIR}/${IPL_SOURCE_FILE%.nas}${IPL_OUTPUT_SUFFIX}"

            echo "set(${TARGET_IPL_ONLY}_IPLS ${IPL_SOURCE})"                                          >> ${CMAKELISTS}
            echo "set(${TARGET_IPL_ONLY}_IPLB ${IPL_OUTPUT})"                                          >> ${CMAKELISTS}
            echo ""                                                                                    >> ${CMAKELISTS}

            # IPL build target
            echo "add_custom_target(${TARGET_IPL_ONLY}"                                                >> ${CMAKELISTS}
            echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${TARGET_IPL_ONLY}_IPLS} \${${TARGET_IPL_ONLY}_IPLB}" >> ${CMAKELISTS}
            echo "  DEPENDS \${${TARGET_IPL_ONLY}_IPLS}"                                               >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}

            # run target (executing IPL binary directly)
            echo "add_custom_target(${TARGET_OS_NAME}_run"                                             >> ${CMAKELISTS}
            # QEMUOPT already contains -fda, so just pass the IPL binary path
            echo "  COMMAND \${QEMU} \${QEMUOPT} \${${TARGET_IPL_ONLY}_IPLB}"                         >> ${CMAKELISTS}
            echo "  DEPENDS ${TARGET_IPL_ONLY}"                                                        >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}
            # debug target
            echo "add_custom_target(${TARGET_OS_NAME}_debug"                                           >> ${CMAKELISTS}
            # QEMUOPT already contains -fda, so just pass the IPL binary path
            echo "  COMMAND \${QEMU} -s -S \${QEMUOPT} \${${TARGET_IPL_ONLY}_IPLB} \${QEMU_DEBUG_OPT}" >> ${CMAKELISTS}
            echo "  DEPENDS ${TARGET_IPL_ONLY}"                                                        >> ${CMAKELISTS}
            echo ")"                                                                                   >> ${CMAKELISTS}
            # clean target
            echo "add_custom_target(${TARGET_OS_NAME}_clean"                                           >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${${TARGET_IPL_ONLY}_IPLB}"                                         >> ${CMAKELISTS}
            echo "  COMMAND rm -f \${root_BINARY_DIR}/projects/${NAS_DIR}/*_wine.bin"                  >> ${CMAKELISTS} # Clean wine output if any
            echo "  COMMAND rm -f \${root_BINARY_DIR}/projects/${NAS_DIR}/*.txt"                       >> ${CMAKELISTS} # Clean od output if any
            echo ")"                                                                                   >> ${CMAKELISTS}

            # Add dependency for the main 'images' target to build the IPL
            echo "add_dependencies(images ${TARGET_IPL_ONLY})"                                         >> ${CMAKELISTS}

        else
            echo "# WARNING: Unsupported file combination in projects/${NAS_DIR}"                      >> ${CMAKELISTS}
        fi

        echo ""                                                                                        >> ${CMAKELISTS}
        echo "########### next target ###############"                                                 >> ${CMAKELISTS}
    fi

    # --- Individual .nas file targets (for wine, od, potentially hrb) ---
    # Always generate these targets if the source file exists, for wine/od comparison.

    # Check if a specific target for this NAS file already exists from IPL only case
    SPECIFIC_TARGET_EXISTS=false
    if ${HAS_IPL_ONLY} && [[ "${NAS_FILE_REL}" == "${NAS_DIR}/${IPL_SOURCE_FILE}" ]]; then
        SPECIFIC_TARGET_EXISTS=true
    fi

    # Add individual NAS build target if it doesn't already exist
    if ! ${SPECIFIC_TARGET_EXISTS} && ! grep -q "add_custom_target(${TARGET_NAME}" ${CMAKELISTS}; then
        echo "set(${TARGET_NAME}_SRCS \${root_SOURCE_DIR}/projects/${NAS_FILE_REL})"                   >> ${CMAKELISTS}
        # Use the determined BIN_FILE (correct suffix .bin or .img)
        echo "set(${TARGET_NAME}_OUTS \${root_BINARY_DIR}/projects/${BIN_FILE})"                       >> ${CMAKELISTS}
        echo ""                                                                                        >> ${CMAKELISTS}
        echo "add_custom_target(${TARGET_NAME}"                                                        >> ${CMAKELISTS}
        echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${TARGET_NAME}_SRCS} \${${TARGET_NAME}_OUTS}"        >> ${CMAKELISTS}
        echo "  DEPENDS \${${TARGET_NAME}_SRCS}"                                                     >> ${CMAKELISTS}
        echo ")"                                                                                       >> ${CMAKELISTS}
        echo ""                                                                                        >> ${CMAKELISTS}

        # Only add dependency to 'images' target if it's NOT ipl10 or asmhead in a full build scenario
        IS_INTERMEDIATE_IN_FULL_BUILD=false
        if ${HAS_IPL10} && ${HAS_ASMHEAD} && ${HAS_C}; then
            if [[ "${NAS_FILE_REL}" == "${NAS_DIR}/ipl10.nas" ]] || [[ "${NAS_FILE_REL}" == "${NAS_DIR}/asmhead.nas" ]]; then
                IS_INTERMEDIATE_IN_FULL_BUILD=true
            fi
        fi
        if ! ${IS_INTERMEDIATE_IN_FULL_BUILD}; then
             echo "add_dependencies(images ${TARGET_NAME})"                                             >> ${CMAKELISTS}
        fi
    fi

    # Add wine target if it doesn't exist
    if ! grep -q "add_custom_target(${TARGET_NAME}_wine" ${CMAKELISTS}; then
         # Ensure source variable is set if not set above
         if ! grep -q "set(${TARGET_NAME}_SRCS" ${CMAKELISTS}; then
             echo "set(${TARGET_NAME}_SRCS \${root_SOURCE_DIR}/projects/${NAS_FILE_REL})"               >> ${CMAKELISTS}
         fi
         # Use the determined WINE_BIN_FILE (correct suffix)
         echo "set(${TARGET_NAME}_WINE_OUTS \${root_BINARY_DIR}/projects/${WINE_BIN_FILE})"             >> ${CMAKELISTS}
         echo "add_custom_target(${TARGET_NAME}_wine"                                                   >> ${CMAKELISTS}
         echo "  COMMAND \${WINE} \${WINE_NASK} \${${TARGET_NAME}_SRCS} \${${TARGET_NAME}_WINE_OUTS}"   >> ${CMAKELISTS}
         echo ")"                                                                                       >> ${CMAKELISTS}
         echo "add_dependencies(wine ${TARGET_NAME}_wine)"                                              >> ${CMAKELISTS}
         echo ""                                                                                        >> ${CMAKELISTS}
    fi

    # Add od target if it doesn't exist
    if ! grep -q "add_custom_target(${TARGET_NAME}_od" ${CMAKELISTS}; then
         # Ensure output variables are set if not set above
         if ! grep -q "set(${TARGET_NAME}_OUTS" ${CMAKELISTS}; then
             echo "set(${TARGET_NAME}_OUTS \${root_BINARY_DIR}/projects/${BIN_FILE})"                   >> ${CMAKELISTS}
         fi
         if ! grep -q "set(${TARGET_NAME}_WINE_OUTS" ${CMAKELISTS}; then
             echo "set(${TARGET_NAME}_WINE_OUTS \${root_BINARY_DIR}/projects/${WINE_BIN_FILE})"         >> ${CMAKELISTS}
         fi

         echo "add_custom_target(${TARGET_NAME}_od"                                                     >> ${CMAKELISTS}
         echo "  COMMAND \${OD} -t x1 \${${TARGET_NAME}_OUTS}      > \${${TARGET_NAME}_OUTS}_f.txt"     >> ${CMAKELISTS}
         echo "  COMMAND \${OD} -t x1 \${${TARGET_NAME}_WINE_OUTS} > \${${TARGET_NAME}_WINE_OUTS}_t.txt" >> ${CMAKELISTS}
         echo "  COMMAND diff -s \${${TARGET_NAME}_OUTS}_f.txt \${${TARGET_NAME}_WINE_OUTS}_t.txt"      >> ${CMAKELISTS}
         echo ")"                                                                                       >> ${CMAKELISTS}
         echo ""                                                                                        >> ${CMAKELISTS}

         # Ensure the dependency target exists before adding dependency
         # The dependency should be the individual NAS build target (${TARGET_NAME})
         DEP_TARGET=${TARGET_NAME}
         # If it's IPL only case, the target name is different
         if ${HAS_IPL_ONLY} && [[ "${NAS_FILE_REL}" == "${NAS_DIR}/${IPL_SOURCE_FILE}" ]]; then
             DEP_TARGET=${TARGET_IPL_ONLY}
         fi

         if grep -q "add_custom_target(${DEP_TARGET}" ${CMAKELISTS}; then
             echo "add_dependencies(${TARGET_NAME}_od ${DEP_TARGET})"                                  >> ${CMAKELISTS}
         fi
         if grep -q "add_custom_target(${TARGET_NAME}_wine" ${CMAKELISTS}; then
             echo "add_dependencies(${TARGET_NAME}_od ${TARGET_NAME}_wine)"                              >> ${CMAKELISTS}
         fi
         echo "add_dependencies(od ${TARGET_NAME}_od)"                                                  >> ${CMAKELISTS}
         echo ""                                                                                        >> ${CMAKELISTS}
    fi


    # .hrb target generation (only for full builds and non-intermediate files)
    if ${HAS_IPL10} && ${HAS_ASMHEAD} && ${HAS_C}; then
        if [[ $NAS_FILE_REL != *naskfunc.nas ]] && [[ $NAS_FILE_REL != *ipl10.nas ]] && [[ $NAS_FILE_REL != *asmhead.nas ]]; then
             if ! grep -q "add_custom_target(${TARGET_NAME}_hrb" ${CMAKELISTS}; then
                 # Ensure source variable is set
                 if ! grep -q "set(${TARGET_NAME}_SRCS" ${CMAKELISTS}; then
                     echo "set(${TARGET_NAME}_SRCS \${root_SOURCE_DIR}/projects/${NAS_FILE_REL})"       >> ${CMAKELISTS}
                 fi
                 echo "set(${TARGET_NAME}_HRB \${root_BINARY_DIR}/projects/${HRB_FILE})"                >> ${CMAKELISTS}
                 echo "add_custom_target(${TARGET_NAME}_hrb"                                            >> ${CMAKELISTS}
                 echo "  COMMAND \${RAKUSK_EXECUTABLE} \${${TARGET_NAME}_SRCS} \${${TARGET_NAME}_HRB}" >> ${CMAKELISTS}
                 # Ensure OS image variable is set for mcopy dependency
                 if ! grep -q "set(${TARGET_OS_NAME}_OS" ${CMAKELISTS}; then
                      echo "set(${TARGET_OS_NAME}_OS    \${root_BINARY_DIR}/projects/${NAS_DIR}/os.img)" >> ${CMAKELISTS}
                 fi
                 echo "  COMMAND mcopy -i \${${TARGET_OS_NAME}_OS} \${${TARGET_NAME}_HRB} ::${HRB_NAME}" >> ${CMAKELISTS}
                 echo "  DEPENDS ${TARGET_OS_NAME}_img"                                                 >> ${CMAKELISTS} # Depends on the full image
                 echo ")"                                                                               >> ${CMAKELISTS}
                 echo ""                                                                                >> ${CMAKELISTS}
                 if [[ $NAS_DIR_TARGET != 01_* ]] && [[ $NAS_DIR_TARGET != 02_* ]] && [[ $NAS_DIR_TARGET != 03_* ]]; then
                     if grep -q "add_custom_target(${TARGET_OS_NAME}_run" ${CMAKELISTS}; then
                         echo "add_dependencies(${TARGET_OS_NAME}_run ${TARGET_NAME}_hrb)"              >> ${CMAKELISTS}
                     fi
                 fi
             fi
        fi
    fi

    echo "#----------------------------------------------------------"                                 >> ${CMAKELISTS}
done
