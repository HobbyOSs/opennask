#----------------------------------------------------------
message(STATUS "Entering directory projects/14_day/harib11h/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(14_day_harib11h_OS    ${root_BINARY_DIR}/projects/14_day/harib11h/os.img)
set(14_day_harib11h_SYS	  ${root_BINARY_DIR}/projects/14_day/harib11h/os.sys)
set(14_day_harib11h_IPLB  ${root_BINARY_DIR}/projects/14_day/harib11h/ipl.bin)
set(14_day_harib11h_IPLS  ${root_SOURCE_DIR}/projects/14_day/harib11h/ipl10.nas)
set(14_day_harib11h_HEADB ${root_BINARY_DIR}/projects/14_day/harib11h/asmhead.bin)
set(14_day_harib11h_HEADS ${root_SOURCE_DIR}/projects/14_day/harib11h/asmhead.nas)
set(14_day_harib11h_BOOTB ${root_BINARY_DIR}/projects/14_day/harib11h/boot.bin)
set(14_day_harib11h_CCO	  ${root_BINARY_DIR}/projects/14_day/harib11h/boot.o)
set(14_day_harib11h_CCS	  ${root_SOURCE_DIR}/projects/14_day/harib11h/*.c)
set(14_day_harib11h_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(14_day_harib11h_FUNCS ${root_SOURCE_DIR}/projects/14_day/harib11h/naskfunc.nas)
set(14_day_harib11h_FUNCO ${root_BINARY_DIR}/projects/14_day/harib11h/naskfunc.o)
set(14_day_harib11h_FONTS ${root_SOURCE_DIR}/projects/14_day/harib11h/hankaku.txt)
set(14_day_harib11h_FONTB ${root_BINARY_DIR}/projects/14_day/harib11h/hankaku.bin)
set(14_day_harib11h_FONTO ${root_BINARY_DIR}/projects/14_day/harib11h/hankaku.o)
set(14_day_harib11h_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(14_day_harib11h_LIBGC ${root_BINARY_DIR}/projects/14_day/harib11h/libgo.o)
set(14_day_harib11h_WILDOBJ ${root_BINARY_DIR}/projects/14_day/harib11h/*.o)


add_custom_target(14_day_harib11h_run
  COMMAND ${QEMU} ${QEMUOPT} ${14_day_harib11h_OS}
  DEPENDS 14_day_harib11h_img
)
add_custom_target(14_day_harib11h_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${14_day_harib11h_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 14_day_harib11h_img
)
add_custom_target(14_day_harib11h_clean
  COMMAND rm -f ${14_day_harib11h_OS}
  COMMAND rm -f ${14_day_harib11h_SYS}
  COMMAND rm -f ${14_day_harib11h_IPLB}
  COMMAND rm -f ${14_day_harib11h_HEADB}
  COMMAND rm -f ${14_day_harib11h_BOOTB}
  COMMAND rm -f ${14_day_harib11h_WILDOBJ}
)
add_custom_target(14_day_harib11h_ipl
  COMMAND ${NASK} ${14_day_harib11h_IPLS} ${14_day_harib11h_IPLB}
)
add_custom_target(14_day_harib11h_sys
  COMMAND ${NASK} ${14_day_harib11h_HEADS} ${14_day_harib11h_HEADB}
  COMMAND ${FONT} ${14_day_harib11h_FONTS} ${14_day_harib11h_FONTB}
  COMMAND ${B2O}  ${14_day_harib11h_FONTB} ${14_day_harib11h_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${14_day_harib11h_LIBGE} ${14_day_harib11h_LIBGC}
  COMMAND ${NASK} ${14_day_harib11h_FUNCS} ${14_day_harib11h_FUNCO}
  COMMAND gcc ${BINOPT} -T ${14_day_harib11h_LDS} ${14_day_harib11h_CCS} ${14_day_harib11h_WILDOBJ} -o ${14_day_harib11h_BOOTB}
  COMMAND cat ${14_day_harib11h_HEADB} ${14_day_harib11h_BOOTB} > ${14_day_harib11h_SYS}
  DEPENDS 14_day_harib11h_ipl
)
add_custom_target(14_day_harib11h_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${14_day_harib11h_IPLB} -i ${14_day_harib11h_OS}
  COMMAND mcopy -i ${14_day_harib11h_OS} ${14_day_harib11h_SYS} ::
  DEPENDS 14_day_harib11h_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11h/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/asmhead_wine.img)

add_custom_target(14_day_harib11h_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(14_day_harib11h_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(14_day_harib11h_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11h_asmhead_od 14_day_harib11h_asmhead)
add_dependencies(14_day_harib11h_asmhead_od 14_day_harib11h_asmhead_wine)

add_dependencies(images 14_day_harib11h_asmhead)
add_dependencies(wine 14_day_harib11h_asmhead_wine)
add_dependencies(od 14_day_harib11h_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11h/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/ipl10_wine.img)

add_custom_target(14_day_harib11h_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(14_day_harib11h_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(14_day_harib11h_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11h_ipl10_od 14_day_harib11h_ipl10)
add_dependencies(14_day_harib11h_ipl10_od 14_day_harib11h_ipl10_wine)

add_dependencies(images 14_day_harib11h_ipl10)
add_dependencies(wine 14_day_harib11h_ipl10_wine)
add_dependencies(od 14_day_harib11h_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/14_day/harib11h/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/14_day/harib11h/naskfunc_wine.img)

add_custom_target(14_day_harib11h_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(14_day_harib11h_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(14_day_harib11h_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(14_day_harib11h_naskfunc_od 14_day_harib11h_naskfunc)
add_dependencies(14_day_harib11h_naskfunc_od 14_day_harib11h_naskfunc_wine)

add_dependencies(images 14_day_harib11h_naskfunc)
add_dependencies(wine 14_day_harib11h_naskfunc_wine)
add_dependencies(od 14_day_harib11h_naskfunc_od)

#----------------------------------------------------------
