#----------------------------------------------------------
message(STATUS "Entering directory projects/06_day/harib03c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(06_day_harib03c_OS    ${root_BINARY_DIR}/projects/06_day/harib03c/os.img)
set(06_day_harib03c_SYS	  ${root_BINARY_DIR}/projects/06_day/harib03c/os.sys)
set(06_day_harib03c_IPLB  ${root_BINARY_DIR}/projects/06_day/harib03c/ipl.bin)
set(06_day_harib03c_IPLS  ${root_SOURCE_DIR}/projects/06_day/harib03c/ipl10.nas)
set(06_day_harib03c_HEADB ${root_BINARY_DIR}/projects/06_day/harib03c/asmhead.bin)
set(06_day_harib03c_HEADS ${root_SOURCE_DIR}/projects/06_day/harib03c/asmhead.nas)
set(06_day_harib03c_BOOTB ${root_BINARY_DIR}/projects/06_day/harib03c/boot.bin)
set(06_day_harib03c_CCO	  ${root_BINARY_DIR}/projects/06_day/harib03c/boot.o)
set(06_day_harib03c_CCS	  ${root_SOURCE_DIR}/projects/06_day/harib03c/*.c)
set(06_day_harib03c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(06_day_harib03c_FUNCS ${root_SOURCE_DIR}/projects/06_day/harib03c/naskfunc.nas)
set(06_day_harib03c_FUNCO ${root_BINARY_DIR}/projects/06_day/harib03c/naskfunc.o)
set(06_day_harib03c_FONTS ${root_SOURCE_DIR}/projects/06_day/harib03c/hankaku.txt)
set(06_day_harib03c_FONTB ${root_BINARY_DIR}/projects/06_day/harib03c/hankaku.bin)
set(06_day_harib03c_FONTO ${root_BINARY_DIR}/projects/06_day/harib03c/hankaku.o)
set(06_day_harib03c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(06_day_harib03c_LIBGC ${root_BINARY_DIR}/projects/06_day/harib03c/libgo.o)
set(06_day_harib03c_WILDOBJ ${root_BINARY_DIR}/projects/06_day/harib03c/*.o)


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
  COMMAND rm -f ${06_day_harib03c_WILDOBJ}
)
add_custom_target(06_day_harib03c_ipl
  COMMAND ${NASK} ${06_day_harib03c_IPLS} ${06_day_harib03c_IPLB}
)
add_custom_target(06_day_harib03c_sys
  COMMAND ${NASK} ${06_day_harib03c_HEADS} ${06_day_harib03c_HEADB}
  COMMAND ${FONT} ${06_day_harib03c_FONTS} ${06_day_harib03c_FONTB}
  COMMAND ${B2O}  ${06_day_harib03c_FONTB} ${06_day_harib03c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${06_day_harib03c_LIBGE} ${06_day_harib03c_LIBGC}
  COMMAND ${NASK} ${06_day_harib03c_FUNCS} ${06_day_harib03c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${06_day_harib03c_LDS} ${06_day_harib03c_CCS} ${06_day_harib03c_WILDOBJ} -o ${06_day_harib03c_BOOTB}
  COMMAND cat ${06_day_harib03c_HEADB} ${06_day_harib03c_BOOTB} > ${06_day_harib03c_SYS}
  DEPENDS 06_day_harib03c_ipl
)
add_custom_target(06_day_harib03c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${06_day_harib03c_IPLB} -i ${06_day_harib03c_OS}
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
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(06_day_harib03c_asmhead_od 06_day_harib03c_asmhead)
add_dependencies(06_day_harib03c_asmhead_od 06_day_harib03c_asmhead_wine)

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
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(06_day_harib03c_ipl10_od 06_day_harib03c_ipl10)
add_dependencies(06_day_harib03c_ipl10_od 06_day_harib03c_ipl10_wine)

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
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(06_day_harib03c_naskfunc_od 06_day_harib03c_naskfunc)
add_dependencies(06_day_harib03c_naskfunc_od 06_day_harib03c_naskfunc_wine)

add_dependencies(images 06_day_harib03c_naskfunc)
add_dependencies(wine 06_day_harib03c_naskfunc_wine)
add_dependencies(od 06_day_harib03c_naskfunc_od)

#----------------------------------------------------------
