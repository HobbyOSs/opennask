#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08f/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08f_OS    ${root_BINARY_DIR}/projects/11_day/harib08f/os.img)
set(11_day_harib08f_SYS	  ${root_BINARY_DIR}/projects/11_day/harib08f/os.sys)
set(11_day_harib08f_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08f/ipl.bin)
set(11_day_harib08f_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08f/ipl10.nas)
set(11_day_harib08f_HEADB ${root_BINARY_DIR}/projects/11_day/harib08f/asmhead.bin)
set(11_day_harib08f_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08f/asmhead.nas)
set(11_day_harib08f_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08f/boot.bin)
set(11_day_harib08f_CCO	  ${root_BINARY_DIR}/projects/11_day/harib08f/boot.o)
set(11_day_harib08f_CCS	  ${root_SOURCE_DIR}/projects/11_day/harib08f/*.c)
set(11_day_harib08f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08f_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08f/naskfunc.nas)
set(11_day_harib08f_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08f/naskfunc.o)
set(11_day_harib08f_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08f/hankaku.txt)
set(11_day_harib08f_FONTB ${root_BINARY_DIR}/projects/11_day/harib08f/hankaku.bin)
set(11_day_harib08f_FONTO ${root_BINARY_DIR}/projects/11_day/harib08f/hankaku.o)
set(11_day_harib08f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08f_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08f/libgo.o)
set(11_day_harib08f_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08f/*.o)


add_custom_target(11_day_harib08f_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08f_OS}
  DEPENDS 11_day_harib08f_img
)
add_custom_target(11_day_harib08f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 11_day_harib08f_img
)
add_custom_target(11_day_harib08f_clean
  COMMAND rm -f ${11_day_harib08f_OS}
  COMMAND rm -f ${11_day_harib08f_SYS}
  COMMAND rm -f ${11_day_harib08f_IPLB}
  COMMAND rm -f ${11_day_harib08f_HEADB}
  COMMAND rm -f ${11_day_harib08f_BOOTB}
  COMMAND rm -f ${11_day_harib08f_WILDOBJ}
)
add_custom_target(11_day_harib08f_ipl
  COMMAND ${NASK} ${11_day_harib08f_IPLS} ${11_day_harib08f_IPLB}
)
add_custom_target(11_day_harib08f_sys
  COMMAND ${NASK} ${11_day_harib08f_HEADS} ${11_day_harib08f_HEADB}
  COMMAND ${FONT} ${11_day_harib08f_FONTS} ${11_day_harib08f_FONTB}
  COMMAND ${B2O}  ${11_day_harib08f_FONTB} ${11_day_harib08f_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08f_LIBGE} ${11_day_harib08f_LIBGC}
  COMMAND ${NASK} ${11_day_harib08f_FUNCS} ${11_day_harib08f_FUNCO}
  COMMAND gcc ${BINOPT} -T ${11_day_harib08f_LDS} ${11_day_harib08f_CCS} ${11_day_harib08f_WILDOBJ} -o ${11_day_harib08f_BOOTB}
  COMMAND cat ${11_day_harib08f_HEADB} ${11_day_harib08f_BOOTB} > ${11_day_harib08f_SYS}
  DEPENDS 11_day_harib08f_ipl
)
add_custom_target(11_day_harib08f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08f_IPLB} -i ${11_day_harib08f_OS}
  COMMAND mcopy -i ${11_day_harib08f_OS} ${11_day_harib08f_SYS} ::
  DEPENDS 11_day_harib08f_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08f/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/asmhead_wine.img)

add_custom_target(11_day_harib08f_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(11_day_harib08f_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(11_day_harib08f_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08f_asmhead_od 11_day_harib08f_asmhead)
add_dependencies(11_day_harib08f_asmhead_od 11_day_harib08f_asmhead_wine)

add_dependencies(images 11_day_harib08f_asmhead)
add_dependencies(wine 11_day_harib08f_asmhead_wine)
add_dependencies(od 11_day_harib08f_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08f/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/ipl10_wine.img)

add_custom_target(11_day_harib08f_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(11_day_harib08f_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(11_day_harib08f_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08f_ipl10_od 11_day_harib08f_ipl10)
add_dependencies(11_day_harib08f_ipl10_od 11_day_harib08f_ipl10_wine)

add_dependencies(images 11_day_harib08f_ipl10)
add_dependencies(wine 11_day_harib08f_ipl10_wine)
add_dependencies(od 11_day_harib08f_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08f/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08f/naskfunc_wine.img)

add_custom_target(11_day_harib08f_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(11_day_harib08f_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(11_day_harib08f_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08f_naskfunc_od 11_day_harib08f_naskfunc)
add_dependencies(11_day_harib08f_naskfunc_od 11_day_harib08f_naskfunc_wine)

add_dependencies(images 11_day_harib08f_naskfunc)
add_dependencies(wine 11_day_harib08f_naskfunc_wine)
add_dependencies(od 11_day_harib08f_naskfunc_od)

#----------------------------------------------------------
