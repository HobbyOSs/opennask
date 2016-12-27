#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08e_OS    ${root_BINARY_DIR}/projects/11_day/harib08e/os.img)
set(11_day_harib08e_SYS	  ${root_BINARY_DIR}/projects/11_day/harib08e/os.sys)
set(11_day_harib08e_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08e/ipl.bin)
set(11_day_harib08e_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08e/ipl10.nas)
set(11_day_harib08e_HEADB ${root_BINARY_DIR}/projects/11_day/harib08e/asmhead.bin)
set(11_day_harib08e_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08e/asmhead.nas)
set(11_day_harib08e_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08e/boot.bin)
set(11_day_harib08e_CCO	  ${root_BINARY_DIR}/projects/11_day/harib08e/boot.o)
set(11_day_harib08e_CCS	  ${root_SOURCE_DIR}/projects/11_day/harib08e/*.c)
set(11_day_harib08e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08e_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08e/naskfunc.nas)
set(11_day_harib08e_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08e/naskfunc.o)
set(11_day_harib08e_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08e/hankaku.txt)
set(11_day_harib08e_FONTB ${root_BINARY_DIR}/projects/11_day/harib08e/hankaku.bin)
set(11_day_harib08e_FONTO ${root_BINARY_DIR}/projects/11_day/harib08e/hankaku.o)
set(11_day_harib08e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08e_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08e/libgo.o)
set(11_day_harib08e_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08e/*.o)


add_custom_target(11_day_harib08e_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08e_OS}
  DEPENDS 11_day_harib08e_img
)
add_custom_target(11_day_harib08e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08e_img
)
add_custom_target(11_day_harib08e_clean
  COMMAND rm -f ${11_day_harib08e_OS}
  COMMAND rm -f ${11_day_harib08e_SYS}
  COMMAND rm -f ${11_day_harib08e_IPLB}
  COMMAND rm -f ${11_day_harib08e_HEADB}
  COMMAND rm -f ${11_day_harib08e_BOOTB}
  COMMAND rm -f ${11_day_harib08e_WILDOBJ}
)
add_custom_target(11_day_harib08e_ipl
  COMMAND ${NASK} ${11_day_harib08e_IPLS} ${11_day_harib08e_IPLB}
)
add_custom_target(11_day_harib08e_sys
  COMMAND ${NASK} ${11_day_harib08e_HEADS} ${11_day_harib08e_HEADB}
  COMMAND ${FONT} ${11_day_harib08e_FONTS} ${11_day_harib08e_FONTB}
  COMMAND ${B2O}  ${11_day_harib08e_FONTB} ${11_day_harib08e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08e_LIBGE} ${11_day_harib08e_LIBGC}
  COMMAND ${NASK} ${11_day_harib08e_FUNCS} ${11_day_harib08e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${11_day_harib08e_LDS} ${11_day_harib08e_CCS} ${11_day_harib08e_WILDOBJ} -o ${11_day_harib08e_BOOTB}
  COMMAND cat ${11_day_harib08e_HEADB} ${11_day_harib08e_BOOTB} > ${11_day_harib08e_SYS}
  DEPENDS 11_day_harib08e_ipl
)
add_custom_target(11_day_harib08e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08e_IPLB} -i ${11_day_harib08e_OS}
  COMMAND mcopy -i ${11_day_harib08e_OS} ${11_day_harib08e_SYS} ::
  DEPENDS 11_day_harib08e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08e/asmhead_wine.img)

add_custom_target(11_day_harib08e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(11_day_harib08e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(11_day_harib08e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08e_asmhead_od 11_day_harib08e_asmhead)
add_dependencies(11_day_harib08e_asmhead_od 11_day_harib08e_asmhead_wine)

add_dependencies(images 11_day_harib08e_asmhead)
add_dependencies(wine 11_day_harib08e_asmhead_wine)
add_dependencies(od 11_day_harib08e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08e/ipl10_wine.img)

add_custom_target(11_day_harib08e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(11_day_harib08e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(11_day_harib08e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08e_ipl10_od 11_day_harib08e_ipl10)
add_dependencies(11_day_harib08e_ipl10_od 11_day_harib08e_ipl10_wine)

add_dependencies(images 11_day_harib08e_ipl10)
add_dependencies(wine 11_day_harib08e_ipl10_wine)
add_dependencies(od 11_day_harib08e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08e/naskfunc_wine.img)

add_custom_target(11_day_harib08e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(11_day_harib08e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(11_day_harib08e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08e_naskfunc_od 11_day_harib08e_naskfunc)
add_dependencies(11_day_harib08e_naskfunc_od 11_day_harib08e_naskfunc_wine)

add_dependencies(images 11_day_harib08e_naskfunc)
add_dependencies(wine 11_day_harib08e_naskfunc_wine)
add_dependencies(od 11_day_harib08e_naskfunc_od)

#----------------------------------------------------------
