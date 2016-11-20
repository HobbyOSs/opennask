#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(06_day_harib03c_OS    ${root_BINARY_DIR}/projects/06_day/harib03c/os.img)
set(06_day_harib03c_SYS	  ${root_BINARY_DIR}/projects/06_day/harib03c/os.sys)
set(06_day_harib03c_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03c/ipl.bin)
set(06_day_harib03c_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03c/ipl10.nas)
set(06_day_harib03c_HEADB ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead.bin)
set(06_day_harib03c_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03c/asmhead.nas)
set(06_day_harib03c_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03c/boot.bin)
set(06_day_harib03c_CCO	  ${root_BINARY_DIR}/projects/06_day/harib03c/boot.o)
set(06_day_harib03c_CCS	  ${root_SOURCE_DIR}/projects/06_day/harib03c/*.c)
set(06_day_harib03c_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)
set(06_day_harib03c_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03c/naskfunc.o)
set(06_day_harib03c_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03c/naskfunc.nas)

add_custom_target(06_day_harib03c_run
  COMMAND ${QEMU} ${QEMUOPT} ${06_day_harib03c_OS}
  DEPENDS 06_day_harib03c_img
)
add_custom_target(06_day_harib03c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${06_day_harib03c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 06_day_harib03c_img
)
add_custom_target(06_day_harib03c_clean
  COMMAND rm -f ${06_day_harib03c_OS}
  COMMAND rm -f ${06_day_harib03c_SYS}
  COMMAND rm -f ${06_day_harib03c_IPLB}
  COMMAND rm -f ${06_day_harib03c_HEADB}
  COMMAND rm -f ${06_day_harib03c_BOOTB}
  COMMAND rm -f ${06_day_harib03c_CCO}
  COMMAND rm -f ${06_day_harib03c_FUNCO}
)
add_custom_target(06_day_harib03c_ipl
  COMMAND ${NASK} ${06_day_harib03c_IPLS} ${06_day_harib03c_IPLB}
)
add_custom_target(06_day_harib03c_sys
  COMMAND ${NASK} ${06_day_harib03c_FUNCS} ${06_day_harib03c_FUNCO}
  COMMAND ${NASK} ${06_day_harib03c_HEADS} ${06_day_harib03c_HEADB}
  COMMAND gcc ${BINOPT} ${06_day_harib03c_CCS} -c -o ${06_day_harib03c_CCO}
  COMMAND ld -o ${06_day_harib03c_BOOTB} -e HariMain -T ${06_day_harib03c_LDS} ${06_day_harib03c_CCO} ${06_day_harib03c_FUNCO}
  COMMAND cat ${06_day_harib03c_HEADB} ${06_day_harib03c_BOOTB} > ${06_day_harib03c_SYS}
  DEPENDS 06_day_harib03c_ipl
)
add_custom_target(06_day_harib03c_img
  COMMAND mformat -f 1440 -C -B ${06_day_harib03c_IPLB} -i ${06_day_harib03c_OS}
  COMMAND mcopy -i ${06_day_harib03c_OS} ${06_day_harib03c_SYS} ::
  DEPENDS 06_day_harib03c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead_wine.img)

add_custom_target(06_day_harib03c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(06_day_harib03c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(06_day_harib03c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 06_day_harib03c_asmhead)
add_dependencies(wine 06_day_harib03c_asmhead_wine)
add_dependencies(od 06_day_harib03c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/ipl10_wine.img)

add_custom_target(06_day_harib03c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(06_day_harib03c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(06_day_harib03c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 06_day_harib03c_ipl10)
add_dependencies(wine 06_day_harib03c_ipl10_wine)
add_dependencies(od 06_day_harib03c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/06_day/harib03c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/06_day/harib03c/naskfunc_wine.img)

add_custom_target(06_day_harib03c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(06_day_harib03c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(06_day_harib03c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 06_day_harib03c_naskfunc)
add_dependencies(wine 06_day_harib03c_naskfunc_wine)
add_dependencies(od 06_day_harib03c_naskfunc_od)

#----------------------------------------------------------
