#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01h/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(04_day_harib01h_OS    ${root_BINARY_DIR}/projects/04_day/harib01h/os.img)
set(04_day_harib01h_SYS	  ${root_BINARY_DIR}/projects/04_day/harib01h/os.sys)
set(04_day_harib01h_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01h/ipl.bin)
set(04_day_harib01h_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01h/ipl10.nas)
set(04_day_harib01h_HEADB ${root_BINARY_DIR}/projects/04_day/harib01h/asmhead.bin)
set(04_day_harib01h_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01h/asmhead.nas)
set(04_day_harib01h_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01h/boot.bin)
set(04_day_harib01h_CCO	  ${root_BINARY_DIR}/projects/04_day/harib01h/boot.o)
set(04_day_harib01h_CCS	  ${root_SOURCE_DIR}/projects/04_day/harib01h/*.c)
set(04_day_harib01h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01h_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01h/naskfunc.nas)
set(04_day_harib01h_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01h/naskfunc.o)
set(04_day_harib01h_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01h/*.o)


add_custom_target(04_day_harib01h_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01h_OS}
  DEPENDS 04_day_harib01h_img
)
add_custom_target(04_day_harib01h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01h_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 04_day_harib01h_img
)
add_custom_target(04_day_harib01h_clean
  COMMAND rm -f ${04_day_harib01h_OS}
  COMMAND rm -f ${04_day_harib01h_SYS}
  COMMAND rm -f ${04_day_harib01h_IPLB}
  COMMAND rm -f ${04_day_harib01h_HEADB}
  COMMAND rm -f ${04_day_harib01h_BOOTB}
  COMMAND rm -f ${04_day_harib01h_WILDOBJ}
)
add_custom_target(04_day_harib01h_ipl
  COMMAND ${NASK} ${04_day_harib01h_IPLS} ${04_day_harib01h_IPLB}
)
add_custom_target(04_day_harib01h_sys
  COMMAND ${NASK} ${04_day_harib01h_HEADS} ${04_day_harib01h_HEADB}
  COMMAND ${NASK} ${04_day_harib01h_FUNCS} ${04_day_harib01h_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01h_LDS} ${04_day_harib01h_CCS} ${04_day_harib01h_WILDOBJ} -o ${04_day_harib01h_BOOTB}
  COMMAND cat ${04_day_harib01h_HEADB} ${04_day_harib01h_BOOTB} > ${04_day_harib01h_SYS}
  DEPENDS 04_day_harib01h_ipl
)
add_custom_target(04_day_harib01h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01h_IPLB} -i ${04_day_harib01h_OS}
  COMMAND mcopy -i ${04_day_harib01h_OS} ${04_day_harib01h_SYS} ::
  DEPENDS 04_day_harib01h_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01h/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/asmhead_wine.img)

add_custom_target(04_day_harib01h_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(04_day_harib01h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(04_day_harib01h_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01h_asmhead_od 04_day_harib01h_asmhead)
add_dependencies(04_day_harib01h_asmhead_od 04_day_harib01h_asmhead_wine)

add_dependencies(images 04_day_harib01h_asmhead)
add_dependencies(wine 04_day_harib01h_asmhead_wine)
add_dependencies(od 04_day_harib01h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01h/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/ipl10_wine.img)

add_custom_target(04_day_harib01h_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(04_day_harib01h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(04_day_harib01h_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01h_ipl10_od 04_day_harib01h_ipl10)
add_dependencies(04_day_harib01h_ipl10_od 04_day_harib01h_ipl10_wine)

add_dependencies(images 04_day_harib01h_ipl10)
add_dependencies(wine 04_day_harib01h_ipl10_wine)
add_dependencies(od 04_day_harib01h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01h/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01h/naskfunc_wine.img)

add_custom_target(04_day_harib01h_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(04_day_harib01h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(04_day_harib01h_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01h_naskfunc_od 04_day_harib01h_naskfunc)
add_dependencies(04_day_harib01h_naskfunc_od 04_day_harib01h_naskfunc_wine)

add_dependencies(images 04_day_harib01h_naskfunc)
add_dependencies(wine 04_day_harib01h_naskfunc_wine)
add_dependencies(od 04_day_harib01h_naskfunc_od)

#----------------------------------------------------------
