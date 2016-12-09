#----------------------------------------------------------
message(STATUS "Entering directory projects/03_day/harib00j/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(03_day_harib00j_OS    ${root_BINARY_DIR}/projects/03_day/harib00j/os.img)
set(03_day_harib00j_SYS	  ${root_BINARY_DIR}/projects/03_day/harib00j/os.sys)
set(03_day_harib00j_IPLB  ${root_BINARY_DIR}/projects/03_day/harib00j/ipl.bin)
set(03_day_harib00j_IPLS  ${root_SOURCE_DIR}/projects/03_day/harib00j/ipl10.nas)
set(03_day_harib00j_HEADB ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead.bin)
set(03_day_harib00j_HEADS ${root_SOURCE_DIR}/projects/03_day/harib00j/asmhead.nas)
set(03_day_harib00j_BOOTB ${root_BINARY_DIR}/projects/03_day/harib00j/boot.bin)
set(03_day_harib00j_CCO	  ${root_BINARY_DIR}/projects/03_day/harib00j/boot.o)
set(03_day_harib00j_CCS	  ${root_SOURCE_DIR}/projects/03_day/harib00j/*.c)
set(03_day_harib00j_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(03_day_harib00j_FUNCS ${root_SOURCE_DIR}/projects/03_day/harib00j/naskfunc.nas)
set(03_day_harib00j_FUNCO ${root_BINARY_DIR}/projects/03_day/harib00j/naskfunc.o)
set(03_day_harib00j_WILDOBJ ${root_BINARY_DIR}/projects/03_day/harib00j/*.o)

add_custom_target(03_day_harib00j_run
  COMMAND ${QEMU} ${QEMUOPT} ${03_day_harib00j_OS}
  DEPENDS 03_day_harib00j_img
)
add_custom_target(03_day_harib00j_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${03_day_harib00j_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 03_day_harib00j_img
)
add_custom_target(03_day_harib00j_clean
  COMMAND rm -f ${03_day_harib00j_OS}
  COMMAND rm -f ${03_day_harib00j_SYS}
  COMMAND rm -f ${03_day_harib00j_IPLB}
  COMMAND rm -f ${03_day_harib00j_HEADB}
  COMMAND rm -f ${03_day_harib00j_BOOTB}
  COMMAND rm -f ${03_day_harib00j_WILDOBJ}
)
add_custom_target(03_day_harib00j_ipl
  COMMAND ${NASK} ${03_day_harib00j_IPLS} ${03_day_harib00j_IPLB}
)
add_custom_target(03_day_harib00j_sys
  COMMAND ${NASK} ${03_day_harib00j_HEADS} ${03_day_harib00j_HEADB}
  COMMAND ${NASK} ${03_day_harib00j_FUNCS} ${03_day_harib00j_FUNCO}
  COMMAND gcc ${BINOPT} -T ${03_day_harib00j_LDS} ${03_day_harib00j_CCS} ${03_day_harib00j_WILDOBJ} -o ${03_day_harib00j_BOOTB}
  COMMAND cat ${03_day_harib00j_HEADB} ${03_day_harib00j_BOOTB} > ${03_day_harib00j_SYS}
  DEPENDS 03_day_harib00j_ipl
)
add_custom_target(03_day_harib00j_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${03_day_harib00j_IPLB} -i ${03_day_harib00j_OS}
  COMMAND mcopy -i ${03_day_harib00j_OS} ${03_day_harib00j_SYS} ::
  DEPENDS 03_day_harib00j_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/asmhead_wine.img)

add_custom_target(03_day_harib00j_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(03_day_harib00j_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(03_day_harib00j_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00j_asmhead_od 03_day_harib00j_asmhead)
add_dependencies(03_day_harib00j_asmhead_od 03_day_harib00j_asmhead_wine)

add_dependencies(images 03_day_harib00j_asmhead)
add_dependencies(wine 03_day_harib00j_asmhead_wine)
add_dependencies(od 03_day_harib00j_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/ipl10_wine.img)

add_custom_target(03_day_harib00j_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(03_day_harib00j_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(03_day_harib00j_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00j_ipl10_od 03_day_harib00j_ipl10)
add_dependencies(03_day_harib00j_ipl10_od 03_day_harib00j_ipl10_wine)

add_dependencies(images 03_day_harib00j_ipl10)
add_dependencies(wine 03_day_harib00j_ipl10_wine)
add_dependencies(od 03_day_harib00j_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/03_day/harib00j/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/03_day/harib00j/naskfunc_wine.img)

add_custom_target(03_day_harib00j_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(03_day_harib00j_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(03_day_harib00j_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(03_day_harib00j_naskfunc_od 03_day_harib00j_naskfunc)
add_dependencies(03_day_harib00j_naskfunc_od 03_day_harib00j_naskfunc_wine)

add_dependencies(images 03_day_harib00j_naskfunc)
add_dependencies(wine 03_day_harib00j_naskfunc_wine)
add_dependencies(od 03_day_harib00j_naskfunc_od)

#----------------------------------------------------------
