#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(04_day_harib01e_OS    ${root_BINARY_DIR}/projects/04_day/harib01e/os.img)
set(04_day_harib01e_SYS	  ${root_BINARY_DIR}/projects/04_day/harib01e/os.sys)
set(04_day_harib01e_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01e/ipl.bin)
set(04_day_harib01e_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01e/ipl10.nas)
set(04_day_harib01e_HEADB ${root_BINARY_DIR}/projects/04_day/harib01e/asmhead.bin)
set(04_day_harib01e_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01e/asmhead.nas)
set(04_day_harib01e_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01e/boot.bin)
set(04_day_harib01e_CCO	  ${root_BINARY_DIR}/projects/04_day/harib01e/boot.o)
set(04_day_harib01e_CCS	  ${root_SOURCE_DIR}/projects/04_day/harib01e/*.c)
set(04_day_harib01e_LDS	  ${root_SOURCE_DIR}/projects/bootpack.ld)
set(04_day_harib01e_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01e/naskfunc.o)
set(04_day_harib01e_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01e/naskfunc.nas)

add_custom_target(04_day_harib01e_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01e_OS}
  DEPENDS 04_day_harib01e_img
)
add_custom_target(04_day_harib01e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 04_day_harib01e_img
)
add_custom_target(04_day_harib01e_clean
  COMMAND rm -f ${04_day_harib01e_OS}
  COMMAND rm -f ${04_day_harib01e_SYS}
  COMMAND rm -f ${04_day_harib01e_IPLB}
  COMMAND rm -f ${04_day_harib01e_HEADB}
  COMMAND rm -f ${04_day_harib01e_BOOTB}
  COMMAND rm -f ${04_day_harib01e_CCO}
  COMMAND rm -f ${04_day_harib01e_FUNCO}
)
add_custom_target(04_day_harib01e_ipl
  COMMAND ${NASK} ${04_day_harib01e_IPLS} ${04_day_harib01e_IPLB}
)
add_custom_target(04_day_harib01e_sys
  COMMAND ${NASK} ${04_day_harib01e_FUNCS} ${04_day_harib01e_FUNCO}
  COMMAND ${NASK} ${04_day_harib01e_HEADS} ${04_day_harib01e_HEADB}
  COMMAND ${NASK} ${04_day_harib01e_FUNCS} ${04_day_harib01e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01e_LDS} ${04_day_harib01e_CCS} ${04_day_harib01e_FUNCO} -o ${04_day_harib01e_BOOTB}
  COMMAND cat ${04_day_harib01e_HEADB} ${04_day_harib01e_BOOTB} > ${04_day_harib01e_SYS}
  DEPENDS 04_day_harib01e_ipl
)
add_custom_target(04_day_harib01e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01e_IPLB} -i ${04_day_harib01e_OS}
  COMMAND mcopy -i ${04_day_harib01e_OS} ${04_day_harib01e_SYS} ::
  DEPENDS 04_day_harib01e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/asmhead_wine.img)

add_custom_target(04_day_harib01e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(04_day_harib01e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(04_day_harib01e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01e_asmhead)
add_dependencies(wine 04_day_harib01e_asmhead_wine)
add_dependencies(od 04_day_harib01e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/ipl10_wine.img)

add_custom_target(04_day_harib01e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(04_day_harib01e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(04_day_harib01e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01e_ipl10)
add_dependencies(wine 04_day_harib01e_ipl10_wine)
add_dependencies(od 04_day_harib01e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01e/naskfunc_wine.img)

add_custom_target(04_day_harib01e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(04_day_harib01e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(04_day_harib01e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 04_day_harib01e_naskfunc)
add_dependencies(wine 04_day_harib01e_naskfunc_wine)
add_dependencies(od 04_day_harib01e_naskfunc_od)

#----------------------------------------------------------
