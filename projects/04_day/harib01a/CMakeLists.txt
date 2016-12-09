#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(04_day_harib01a_OS    ${root_BINARY_DIR}/projects/04_day/harib01a/os.img)
set(04_day_harib01a_SYS	  ${root_BINARY_DIR}/projects/04_day/harib01a/os.sys)
set(04_day_harib01a_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01a/ipl.bin)
set(04_day_harib01a_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01a/ipl10.nas)
set(04_day_harib01a_HEADB ${root_BINARY_DIR}/projects/04_day/harib01a/asmhead.bin)
set(04_day_harib01a_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01a/asmhead.nas)
set(04_day_harib01a_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01a/boot.bin)
set(04_day_harib01a_CCO	  ${root_BINARY_DIR}/projects/04_day/harib01a/boot.o)
set(04_day_harib01a_CCS	  ${root_SOURCE_DIR}/projects/04_day/harib01a/*.c)
set(04_day_harib01a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01a_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01a/naskfunc.nas)
set(04_day_harib01a_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01a/naskfunc.o)
set(04_day_harib01a_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01a/*.o)

add_custom_target(04_day_harib01a_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01a_OS}
  DEPENDS 04_day_harib01a_img
)
add_custom_target(04_day_harib01a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01a_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 04_day_harib01a_img
)
add_custom_target(04_day_harib01a_clean
  COMMAND rm -f ${04_day_harib01a_OS}
  COMMAND rm -f ${04_day_harib01a_SYS}
  COMMAND rm -f ${04_day_harib01a_IPLB}
  COMMAND rm -f ${04_day_harib01a_HEADB}
  COMMAND rm -f ${04_day_harib01a_BOOTB}
  COMMAND rm -f ${04_day_harib01a_WILDOBJ}
)
add_custom_target(04_day_harib01a_ipl
  COMMAND ${NASK} ${04_day_harib01a_IPLS} ${04_day_harib01a_IPLB}
)
add_custom_target(04_day_harib01a_sys
  COMMAND ${NASK} ${04_day_harib01a_HEADS} ${04_day_harib01a_HEADB}
  COMMAND ${NASK} ${04_day_harib01a_FUNCS} ${04_day_harib01a_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01a_LDS} ${04_day_harib01a_CCS} ${04_day_harib01a_WILDOBJ} -o ${04_day_harib01a_BOOTB}
  COMMAND cat ${04_day_harib01a_HEADB} ${04_day_harib01a_BOOTB} > ${04_day_harib01a_SYS}
  DEPENDS 04_day_harib01a_ipl
)
add_custom_target(04_day_harib01a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01a_IPLB} -i ${04_day_harib01a_OS}
  COMMAND mcopy -i ${04_day_harib01a_OS} ${04_day_harib01a_SYS} ::
  DEPENDS 04_day_harib01a_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01a/asmhead_wine.img)

add_custom_target(04_day_harib01a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(04_day_harib01a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(04_day_harib01a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01a_asmhead_od 04_day_harib01a_asmhead)
add_dependencies(04_day_harib01a_asmhead_od 04_day_harib01a_asmhead_wine)

add_dependencies(images 04_day_harib01a_asmhead)
add_dependencies(wine 04_day_harib01a_asmhead_wine)
add_dependencies(od 04_day_harib01a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01a/ipl10_wine.img)

add_custom_target(04_day_harib01a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(04_day_harib01a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(04_day_harib01a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01a_ipl10_od 04_day_harib01a_ipl10)
add_dependencies(04_day_harib01a_ipl10_od 04_day_harib01a_ipl10_wine)

add_dependencies(images 04_day_harib01a_ipl10)
add_dependencies(wine 04_day_harib01a_ipl10_wine)
add_dependencies(od 04_day_harib01a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01a/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01a/naskfunc_wine.img)

add_custom_target(04_day_harib01a_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(04_day_harib01a_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(04_day_harib01a_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01a_naskfunc_od 04_day_harib01a_naskfunc)
add_dependencies(04_day_harib01a_naskfunc_od 04_day_harib01a_naskfunc_wine)

add_dependencies(images 04_day_harib01a_naskfunc)
add_dependencies(wine 04_day_harib01a_naskfunc_wine)
add_dependencies(od 04_day_harib01a_naskfunc_od)

#----------------------------------------------------------
