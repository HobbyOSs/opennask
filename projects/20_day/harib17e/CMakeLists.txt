#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17e_OS    ${root_BINARY_DIR}/projects/20_day/harib17e/os.img)
set(20_day_harib17e_SYS	  ${root_BINARY_DIR}/projects/20_day/harib17e/os.sys)
set(20_day_harib17e_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17e/ipl.bin)
set(20_day_harib17e_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17e/ipl10.nas)
set(20_day_harib17e_HEADB ${root_BINARY_DIR}/projects/20_day/harib17e/asmhead.bin)
set(20_day_harib17e_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17e/asmhead.nas)
set(20_day_harib17e_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17e/boot.bin)
set(20_day_harib17e_CCO	  ${root_BINARY_DIR}/projects/20_day/harib17e/boot.o)
set(20_day_harib17e_CCS	  ${root_SOURCE_DIR}/projects/20_day/harib17e/*.c)
set(20_day_harib17e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17e_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17e/naskfunc.nas)
set(20_day_harib17e_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17e/naskfunc.o)
set(20_day_harib17e_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17e/hankaku.txt)
set(20_day_harib17e_FONTB ${root_BINARY_DIR}/projects/20_day/harib17e/hankaku.bin)
set(20_day_harib17e_FONTO ${root_BINARY_DIR}/projects/20_day/harib17e/hankaku.o)
set(20_day_harib17e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17e_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17e/libgo.o)
set(20_day_harib17e_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17e/*.o)


add_custom_target(20_day_harib17e_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17e_OS}
  DEPENDS 20_day_harib17e_img
)
add_custom_target(20_day_harib17e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 20_day_harib17e_img
)
add_custom_target(20_day_harib17e_clean
  COMMAND rm -f ${20_day_harib17e_OS}
  COMMAND rm -f ${20_day_harib17e_SYS}
  COMMAND rm -f ${20_day_harib17e_IPLB}
  COMMAND rm -f ${20_day_harib17e_HEADB}
  COMMAND rm -f ${20_day_harib17e_BOOTB}
  COMMAND rm -f ${20_day_harib17e_WILDOBJ}
)
add_custom_target(20_day_harib17e_ipl
  COMMAND ${NASK} ${20_day_harib17e_IPLS} ${20_day_harib17e_IPLB}
)
add_custom_target(20_day_harib17e_sys
  COMMAND ${NASK} ${20_day_harib17e_HEADS} ${20_day_harib17e_HEADB}
  COMMAND ${FONT} ${20_day_harib17e_FONTS} ${20_day_harib17e_FONTB}
  COMMAND ${B2O}  ${20_day_harib17e_FONTB} ${20_day_harib17e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17e_LIBGE} ${20_day_harib17e_LIBGC}
  COMMAND ${NASK} ${20_day_harib17e_FUNCS} ${20_day_harib17e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17e_LDS} ${20_day_harib17e_CCS} ${20_day_harib17e_WILDOBJ} -o ${20_day_harib17e_BOOTB}
  COMMAND cat ${20_day_harib17e_HEADB} ${20_day_harib17e_BOOTB} > ${20_day_harib17e_SYS}
  DEPENDS 20_day_harib17e_ipl
)
add_custom_target(20_day_harib17e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17e_IPLB} -i ${20_day_harib17e_OS}
  COMMAND mcopy -i ${20_day_harib17e_OS} ${20_day_harib17e_SYS} ::
  DEPENDS 20_day_harib17e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/asmhead_wine.img)

add_custom_target(20_day_harib17e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(20_day_harib17e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(20_day_harib17e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_asmhead_od 20_day_harib17e_asmhead)
add_dependencies(20_day_harib17e_asmhead_od 20_day_harib17e_asmhead_wine)

add_dependencies(images 20_day_harib17e_asmhead)
add_dependencies(wine 20_day_harib17e_asmhead_wine)
add_dependencies(od 20_day_harib17e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hlt.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/hlt.nas)
set(hlt.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/hlt.img)
set(hlt_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/hlt_wine.img)

add_custom_target(20_day_harib17e_hlt
  COMMAND ${root_BINARY_DIR}/src/opennask ${hlt.img_SRCS} ${hlt.img_OUTS}
)
add_custom_target(20_day_harib17e_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${hlt.img_SRCS} ${hlt_wine.img_OUTS}
)
add_custom_target(20_day_harib17e_hlt_od
  COMMAND ${OD} -t x1 ${hlt.img_OUTS}      > ${hlt.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hlt_wine.img_OUTS} > ${hlt_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hlt.img_OUTS}_f.txt ${hlt_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_hlt_od 20_day_harib17e_hlt)
add_dependencies(20_day_harib17e_hlt_od 20_day_harib17e_hlt_wine)

add_dependencies(images 20_day_harib17e_hlt)
add_dependencies(wine 20_day_harib17e_hlt_wine)
add_dependencies(od 20_day_harib17e_hlt_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/ipl10_wine.img)

add_custom_target(20_day_harib17e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(20_day_harib17e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(20_day_harib17e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_ipl10_od 20_day_harib17e_ipl10)
add_dependencies(20_day_harib17e_ipl10_od 20_day_harib17e_ipl10_wine)

add_dependencies(images 20_day_harib17e_ipl10)
add_dependencies(wine 20_day_harib17e_ipl10_wine)
add_dependencies(od 20_day_harib17e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/20_day/harib17e/naskfunc_wine.img)

add_custom_target(20_day_harib17e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(20_day_harib17e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(20_day_harib17e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(20_day_harib17e_naskfunc_od 20_day_harib17e_naskfunc)
add_dependencies(20_day_harib17e_naskfunc_od 20_day_harib17e_naskfunc_wine)

add_dependencies(images 20_day_harib17e_naskfunc)
add_dependencies(wine 20_day_harib17e_naskfunc_wine)
add_dependencies(od 20_day_harib17e_naskfunc_od)

#----------------------------------------------------------
