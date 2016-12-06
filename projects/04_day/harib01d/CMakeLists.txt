#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01d/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(04_day_harib01d_OS    ${root_BINARY_DIR}/projects/04_day/harib01d/os.img)
set(04_day_harib01d_SYS	  ${root_BINARY_DIR}/projects/04_day/harib01d/os.sys)
set(04_day_harib01d_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01d/ipl.bin)
set(04_day_harib01d_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01d/ipl10.nas)
set(04_day_harib01d_HEADB ${root_BINARY_DIR}/projects/04_day/harib01d/asmhead.bin)
set(04_day_harib01d_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01d/asmhead.nas)
set(04_day_harib01d_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01d/boot.bin)
set(04_day_harib01d_CCO	  ${root_BINARY_DIR}/projects/04_day/harib01d/boot.o)
set(04_day_harib01d_CCS	  ${root_SOURCE_DIR}/projects/04_day/harib01d/*.c)
set(04_day_harib01d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01d_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01d/naskfunc.o)
set(04_day_harib01d_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01d/naskfunc.nas)

add_custom_target(04_day_harib01d_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01d_OS}
  DEPENDS 04_day_harib01d_img
)
add_custom_target(04_day_harib01d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01d_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 04_day_harib01d_img
)
add_custom_target(04_day_harib01d_clean
  COMMAND rm -f ${04_day_harib01d_OS}
  COMMAND rm -f ${04_day_harib01d_SYS}
  COMMAND rm -f ${04_day_harib01d_IPLB}
  COMMAND rm -f ${04_day_harib01d_HEADB}
  COMMAND rm -f ${04_day_harib01d_BOOTB}
  COMMAND rm -f ${04_day_harib01d_CCO}
  COMMAND rm -f ${04_day_harib01d_FUNCO}
)
add_custom_target(04_day_harib01d_ipl
  COMMAND ${NASK} ${04_day_harib01d_IPLS} ${04_day_harib01d_IPLB}
)
add_custom_target(04_day_harib01d_sys
  COMMAND ${NASK} ${04_day_harib01d_FUNCS} ${04_day_harib01d_FUNCO}
  COMMAND ${NASK} ${04_day_harib01d_HEADS} ${04_day_harib01d_HEADB}
  COMMAND ${NASK} ${04_day_harib01d_FUNCS} ${04_day_harib01d_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01d_LDS} ${04_day_harib01d_CCS} ${04_day_harib01d_FUNCO} -o ${04_day_harib01d_BOOTB}
  COMMAND cat ${04_day_harib01d_HEADB} ${04_day_harib01d_BOOTB} > ${04_day_harib01d_SYS}
  DEPENDS 04_day_harib01d_ipl
)
add_custom_target(04_day_harib01d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01d_IPLB} -i ${04_day_harib01d_OS}
  COMMAND mcopy -i ${04_day_harib01d_OS} ${04_day_harib01d_SYS} ::
  DEPENDS 04_day_harib01d_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01d/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/asmhead_wine.img)

add_custom_target(04_day_harib01d_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(04_day_harib01d_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(04_day_harib01d_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01d_asmhead_od 04_day_harib01d_asmhead)
add_dependencies(04_day_harib01d_asmhead_od 04_day_harib01d_asmhead_wine)

add_dependencies(images 04_day_harib01d_asmhead)
add_dependencies(wine 04_day_harib01d_asmhead_wine)
add_dependencies(od 04_day_harib01d_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01d/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/ipl10_wine.img)

add_custom_target(04_day_harib01d_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(04_day_harib01d_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(04_day_harib01d_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01d_ipl10_od 04_day_harib01d_ipl10)
add_dependencies(04_day_harib01d_ipl10_od 04_day_harib01d_ipl10_wine)

add_dependencies(images 04_day_harib01d_ipl10)
add_dependencies(wine 04_day_harib01d_ipl10_wine)
add_dependencies(od 04_day_harib01d_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01d/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01d/naskfunc_wine.img)

add_custom_target(04_day_harib01d_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(04_day_harib01d_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(04_day_harib01d_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01d_naskfunc_od 04_day_harib01d_naskfunc)
add_dependencies(04_day_harib01d_naskfunc_od 04_day_harib01d_naskfunc_wine)

add_dependencies(images 04_day_harib01d_naskfunc)
add_dependencies(wine 04_day_harib01d_naskfunc_wine)
add_dependencies(od 04_day_harib01d_naskfunc_od)

#----------------------------------------------------------
