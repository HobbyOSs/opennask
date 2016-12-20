#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11i/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11i_OS    ${root_BINARY_DIR}/projects/14_day/harib11i/os.img)
set(14_day_harib11i_SYS	  ${root_BINARY_DIR}/projects/14_day/harib11i/os.sys)
set(14_day_harib11i_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11i/ipl.bin)
set(14_day_harib11i_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11i/ipl10.nas)
set(14_day_harib11i_HEADB ${root_BINARY_DIR}/projects/14_day/harib11i/asmhead.bin)
set(14_day_harib11i_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11i/asmhead.nas)
set(14_day_harib11i_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11i/boot.bin)
set(14_day_harib11i_CCO	  ${root_BINARY_DIR}/projects/14_day/harib11i/boot.o)
set(14_day_harib11i_CCS	  ${root_SOURCE_DIR}/projects/14_day/harib11i/*.c)
set(14_day_harib11i_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11i_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11i/naskfunc.nas)
set(14_day_harib11i_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11i/naskfunc.o)
set(14_day_harib11i_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11i/hankaku.txt)
set(14_day_harib11i_FONTB ${root_BINARY_DIR}/projects/14_day/harib11i/hankaku.bin)
set(14_day_harib11i_FONTO ${root_BINARY_DIR}/projects/14_day/harib11i/hankaku.o)
set(14_day_harib11i_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11i_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11i/libgo.o)
set(14_day_harib11i_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11i/*.o)


add_custom_target(14_day_harib11i_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11i_OS}
  DEPENDS 14_day_harib11i_img
)
add_custom_target(14_day_harib11i_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11i_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 14_day_harib11i_img
)
add_custom_target(14_day_harib11i_clean
  COMMAND rm -f ${14_day_harib11i_OS}
  COMMAND rm -f ${14_day_harib11i_SYS}
  COMMAND rm -f ${14_day_harib11i_IPLB}
  COMMAND rm -f ${14_day_harib11i_HEADB}
  COMMAND rm -f ${14_day_harib11i_BOOTB}
  COMMAND rm -f ${14_day_harib11i_WILDOBJ}
)
add_custom_target(14_day_harib11i_ipl
  COMMAND ${NASK} ${14_day_harib11i_IPLS} ${14_day_harib11i_IPLB}
)
add_custom_target(14_day_harib11i_sys
  COMMAND ${NASK} ${14_day_harib11i_HEADS} ${14_day_harib11i_HEADB}
  COMMAND ${FONT} ${14_day_harib11i_FONTS} ${14_day_harib11i_FONTB}
  COMMAND ${B2O}  ${14_day_harib11i_FONTB} ${14_day_harib11i_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11i_LIBGE} ${14_day_harib11i_LIBGC}
  COMMAND ${NASK} ${14_day_harib11i_FUNCS} ${14_day_harib11i_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11i_LDS} ${14_day_harib11i_CCS} ${14_day_harib11i_WILDOBJ} -o ${14_day_harib11i_BOOTB}
  COMMAND cat ${14_day_harib11i_HEADB} ${14_day_harib11i_BOOTB} > ${14_day_harib11i_SYS}
  DEPENDS 14_day_harib11i_ipl
)
add_custom_target(14_day_harib11i_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11i_IPLB} -i ${14_day_harib11i_OS}
  COMMAND mcopy -i ${14_day_harib11i_OS} ${14_day_harib11i_SYS} ::
  DEPENDS 14_day_harib11i_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11i/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/asmhead_wine.img)

add_custom_target(14_day_harib11i_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(14_day_harib11i_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(14_day_harib11i_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11i_asmhead_od 14_day_harib11i_asmhead)
add_dependencies(14_day_harib11i_asmhead_od 14_day_harib11i_asmhead_wine)

add_dependencies(images 14_day_harib11i_asmhead)
add_dependencies(wine 14_day_harib11i_asmhead_wine)
add_dependencies(od 14_day_harib11i_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11i/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/ipl10_wine.img)

add_custom_target(14_day_harib11i_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(14_day_harib11i_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(14_day_harib11i_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11i_ipl10_od 14_day_harib11i_ipl10)
add_dependencies(14_day_harib11i_ipl10_od 14_day_harib11i_ipl10_wine)

add_dependencies(images 14_day_harib11i_ipl10)
add_dependencies(wine 14_day_harib11i_ipl10_wine)
add_dependencies(od 14_day_harib11i_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11i/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11i/naskfunc_wine.img)

add_custom_target(14_day_harib11i_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(14_day_harib11i_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(14_day_harib11i_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11i_naskfunc_od 14_day_harib11i_naskfunc)
add_dependencies(14_day_harib11i_naskfunc_od 14_day_harib11i_naskfunc_wine)

add_dependencies(images 14_day_harib11i_naskfunc)
add_dependencies(wine 14_day_harib11i_naskfunc_wine)
add_dependencies(od 14_day_harib11i_naskfunc_od)

#----------------------------------------------------------
