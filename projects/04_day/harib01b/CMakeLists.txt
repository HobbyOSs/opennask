#----------------------------------------------------------
message(STATUS "Entering directory projects/04_day/harib01b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(04_day_harib01b_OS    ${root_BINARY_DIR}/projects/04_day/harib01b/os.img)
set(04_day_harib01b_SYS	  ${root_BINARY_DIR}/projects/04_day/harib01b/os.sys)
set(04_day_harib01b_IPLB  ${root_BINARY_DIR}/projects/04_day/harib01b/ipl.bin)
set(04_day_harib01b_IPLS  ${root_SOURCE_DIR}/projects/04_day/harib01b/ipl10.nas)
set(04_day_harib01b_HEADB ${root_BINARY_DIR}/projects/04_day/harib01b/asmhead.bin)
set(04_day_harib01b_HEADS ${root_SOURCE_DIR}/projects/04_day/harib01b/asmhead.nas)
set(04_day_harib01b_BOOTB ${root_BINARY_DIR}/projects/04_day/harib01b/boot.bin)
set(04_day_harib01b_CCO	  ${root_BINARY_DIR}/projects/04_day/harib01b/boot.o)
set(04_day_harib01b_CCS	  ${root_SOURCE_DIR}/projects/04_day/harib01b/*.c)
set(04_day_harib01b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(04_day_harib01b_FUNCS ${root_SOURCE_DIR}/projects/04_day/harib01b/naskfunc.nas)
set(04_day_harib01b_FUNCO ${root_BINARY_DIR}/projects/04_day/harib01b/naskfunc.o)
set(04_day_harib01b_WILDOBJ ${root_BINARY_DIR}/projects/04_day/harib01b/*.o)

add_custom_target(04_day_harib01b_run
  COMMAND ${QEMU} ${QEMUOPT} ${04_day_harib01b_OS}
  DEPENDS 04_day_harib01b_img
)
add_custom_target(04_day_harib01b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${04_day_harib01b_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 04_day_harib01b_img
)
add_custom_target(04_day_harib01b_clean
  COMMAND rm -f ${04_day_harib01b_OS}
  COMMAND rm -f ${04_day_harib01b_SYS}
  COMMAND rm -f ${04_day_harib01b_IPLB}
  COMMAND rm -f ${04_day_harib01b_HEADB}
  COMMAND rm -f ${04_day_harib01b_BOOTB}
  COMMAND rm -f ${04_day_harib01b_WILDOBJ}
)
add_custom_target(04_day_harib01b_ipl
  COMMAND ${NASK} ${04_day_harib01b_IPLS} ${04_day_harib01b_IPLB}
)
add_custom_target(04_day_harib01b_sys
  COMMAND ${NASK} ${04_day_harib01b_HEADS} ${04_day_harib01b_HEADB}
  COMMAND ${NASK} ${04_day_harib01b_FUNCS} ${04_day_harib01b_FUNCO}
  COMMAND gcc ${BINOPT} -T ${04_day_harib01b_LDS} ${04_day_harib01b_CCS} ${04_day_harib01b_WILDOBJ} -o ${04_day_harib01b_BOOTB}
  COMMAND cat ${04_day_harib01b_HEADB} ${04_day_harib01b_BOOTB} > ${04_day_harib01b_SYS}
  DEPENDS 04_day_harib01b_ipl
)
add_custom_target(04_day_harib01b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${04_day_harib01b_IPLB} -i ${04_day_harib01b_OS}
  COMMAND mcopy -i ${04_day_harib01b_OS} ${04_day_harib01b_SYS} ::
  DEPENDS 04_day_harib01b_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/asmhead_wine.img)

add_custom_target(04_day_harib01b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(04_day_harib01b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(04_day_harib01b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01b_asmhead_od 04_day_harib01b_asmhead)
add_dependencies(04_day_harib01b_asmhead_od 04_day_harib01b_asmhead_wine)

add_dependencies(images 04_day_harib01b_asmhead)
add_dependencies(wine 04_day_harib01b_asmhead_wine)
add_dependencies(od 04_day_harib01b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/ipl10_wine.img)

add_custom_target(04_day_harib01b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(04_day_harib01b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(04_day_harib01b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01b_ipl10_od 04_day_harib01b_ipl10)
add_dependencies(04_day_harib01b_ipl10_od 04_day_harib01b_ipl10_wine)

add_dependencies(images 04_day_harib01b_ipl10)
add_dependencies(wine 04_day_harib01b_ipl10_wine)
add_dependencies(od 04_day_harib01b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/04_day/harib01b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/04_day/harib01b/naskfunc_wine.img)

add_custom_target(04_day_harib01b_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(04_day_harib01b_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(04_day_harib01b_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(04_day_harib01b_naskfunc_od 04_day_harib01b_naskfunc)
add_dependencies(04_day_harib01b_naskfunc_od 04_day_harib01b_naskfunc_wine)

add_dependencies(images 04_day_harib01b_naskfunc)
add_dependencies(wine 04_day_harib01b_naskfunc_wine)
add_dependencies(od 04_day_harib01b_naskfunc_od)

#----------------------------------------------------------
