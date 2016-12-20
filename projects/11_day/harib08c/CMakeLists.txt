#----------------------------------------------------------
message(STATUS "Entering directory projects/11_day/harib08c/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(11_day_harib08c_OS    ${root_BINARY_DIR}/projects/11_day/harib08c/os.img)
set(11_day_harib08c_SYS	  ${root_BINARY_DIR}/projects/11_day/harib08c/os.sys)
set(11_day_harib08c_IPLB  ${root_BINARY_DIR}/projects/11_day/harib08c/ipl.bin)
set(11_day_harib08c_IPLS  ${root_SOURCE_DIR}/projects/11_day/harib08c/ipl10.nas)
set(11_day_harib08c_HEADB ${root_BINARY_DIR}/projects/11_day/harib08c/asmhead.bin)
set(11_day_harib08c_HEADS ${root_SOURCE_DIR}/projects/11_day/harib08c/asmhead.nas)
set(11_day_harib08c_BOOTB ${root_BINARY_DIR}/projects/11_day/harib08c/boot.bin)
set(11_day_harib08c_CCO	  ${root_BINARY_DIR}/projects/11_day/harib08c/boot.o)
set(11_day_harib08c_CCS	  ${root_SOURCE_DIR}/projects/11_day/harib08c/*.c)
set(11_day_harib08c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(11_day_harib08c_FUNCS ${root_SOURCE_DIR}/projects/11_day/harib08c/naskfunc.nas)
set(11_day_harib08c_FUNCO ${root_BINARY_DIR}/projects/11_day/harib08c/naskfunc.o)
set(11_day_harib08c_FONTS ${root_SOURCE_DIR}/projects/11_day/harib08c/hankaku.txt)
set(11_day_harib08c_FONTB ${root_BINARY_DIR}/projects/11_day/harib08c/hankaku.bin)
set(11_day_harib08c_FONTO ${root_BINARY_DIR}/projects/11_day/harib08c/hankaku.o)
set(11_day_harib08c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(11_day_harib08c_LIBGC ${root_BINARY_DIR}/projects/11_day/harib08c/libgo.o)
set(11_day_harib08c_WILDOBJ ${root_BINARY_DIR}/projects/11_day/harib08c/*.o)


add_custom_target(11_day_harib08c_run
  COMMAND ${QEMU} ${QEMUOPT} ${11_day_harib08c_OS}
  DEPENDS 11_day_harib08c_img
)
add_custom_target(11_day_harib08c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${11_day_harib08c_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 11_day_harib08c_img
)
add_custom_target(11_day_harib08c_clean
  COMMAND rm -f ${11_day_harib08c_OS}
  COMMAND rm -f ${11_day_harib08c_SYS}
  COMMAND rm -f ${11_day_harib08c_IPLB}
  COMMAND rm -f ${11_day_harib08c_HEADB}
  COMMAND rm -f ${11_day_harib08c_BOOTB}
  COMMAND rm -f ${11_day_harib08c_WILDOBJ}
)
add_custom_target(11_day_harib08c_ipl
  COMMAND ${NASK} ${11_day_harib08c_IPLS} ${11_day_harib08c_IPLB}
)
add_custom_target(11_day_harib08c_sys
  COMMAND ${NASK} ${11_day_harib08c_HEADS} ${11_day_harib08c_HEADB}
  COMMAND ${FONT} ${11_day_harib08c_FONTS} ${11_day_harib08c_FONTB}
  COMMAND ${B2O}  ${11_day_harib08c_FONTB} ${11_day_harib08c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${11_day_harib08c_LIBGE} ${11_day_harib08c_LIBGC}
  COMMAND ${NASK} ${11_day_harib08c_FUNCS} ${11_day_harib08c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${11_day_harib08c_LDS} ${11_day_harib08c_CCS} ${11_day_harib08c_WILDOBJ} -o ${11_day_harib08c_BOOTB}
  COMMAND cat ${11_day_harib08c_HEADB} ${11_day_harib08c_BOOTB} > ${11_day_harib08c_SYS}
  DEPENDS 11_day_harib08c_ipl
)
add_custom_target(11_day_harib08c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${11_day_harib08c_IPLB} -i ${11_day_harib08c_OS}
  COMMAND mcopy -i ${11_day_harib08c_OS} ${11_day_harib08c_SYS} ::
  DEPENDS 11_day_harib08c_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08c/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/asmhead_wine.img)

add_custom_target(11_day_harib08c_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(11_day_harib08c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(11_day_harib08c_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08c_asmhead_od 11_day_harib08c_asmhead)
add_dependencies(11_day_harib08c_asmhead_od 11_day_harib08c_asmhead_wine)

add_dependencies(images 11_day_harib08c_asmhead)
add_dependencies(wine 11_day_harib08c_asmhead_wine)
add_dependencies(od 11_day_harib08c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08c/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/ipl10_wine.img)

add_custom_target(11_day_harib08c_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(11_day_harib08c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(11_day_harib08c_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08c_ipl10_od 11_day_harib08c_ipl10)
add_dependencies(11_day_harib08c_ipl10_od 11_day_harib08c_ipl10_wine)

add_dependencies(images 11_day_harib08c_ipl10)
add_dependencies(wine 11_day_harib08c_ipl10_wine)
add_dependencies(od 11_day_harib08c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/11_day/harib08c/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/11_day/harib08c/naskfunc_wine.img)

add_custom_target(11_day_harib08c_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(11_day_harib08c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(11_day_harib08c_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(11_day_harib08c_naskfunc_od 11_day_harib08c_naskfunc)
add_dependencies(11_day_harib08c_naskfunc_od 11_day_harib08c_naskfunc_wine)

add_dependencies(images 11_day_harib08c_naskfunc)
add_dependencies(wine 11_day_harib08c_naskfunc_wine)
add_dependencies(od 11_day_harib08c_naskfunc_od)

#----------------------------------------------------------
