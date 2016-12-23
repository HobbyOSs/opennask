#----------------------------------------------------------
message(STATUS "Entering directory projects/19_day/harib16e/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(19_day_harib16e_OS    ${root_BINARY_DIR}/projects/19_day/harib16e/os.img)
set(19_day_harib16e_SYS	  ${root_BINARY_DIR}/projects/19_day/harib16e/os.sys)
set(19_day_harib16e_IPLB  ${root_BINARY_DIR}/projects/19_day/harib16e/ipl.bin)
set(19_day_harib16e_IPLS  ${root_SOURCE_DIR}/projects/19_day/harib16e/ipl10.nas)
set(19_day_harib16e_HEADB ${root_BINARY_DIR}/projects/19_day/harib16e/asmhead.bin)
set(19_day_harib16e_HEADS ${root_SOURCE_DIR}/projects/19_day/harib16e/asmhead.nas)
set(19_day_harib16e_BOOTB ${root_BINARY_DIR}/projects/19_day/harib16e/boot.bin)
set(19_day_harib16e_CCO	  ${root_BINARY_DIR}/projects/19_day/harib16e/boot.o)
set(19_day_harib16e_CCS	  ${root_SOURCE_DIR}/projects/19_day/harib16e/*.c)
set(19_day_harib16e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(19_day_harib16e_FUNCS ${root_SOURCE_DIR}/projects/19_day/harib16e/naskfunc.nas)
set(19_day_harib16e_FUNCO ${root_BINARY_DIR}/projects/19_day/harib16e/naskfunc.o)
set(19_day_harib16e_FONTS ${root_SOURCE_DIR}/projects/19_day/harib16e/hankaku.txt)
set(19_day_harib16e_FONTB ${root_BINARY_DIR}/projects/19_day/harib16e/hankaku.bin)
set(19_day_harib16e_FONTO ${root_BINARY_DIR}/projects/19_day/harib16e/hankaku.o)
set(19_day_harib16e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(19_day_harib16e_LIBGC ${root_BINARY_DIR}/projects/19_day/harib16e/libgo.o)
set(19_day_harib16e_WILDOBJ ${root_BINARY_DIR}/projects/19_day/harib16e/*.o)


add_custom_target(19_day_harib16e_run
  COMMAND ${QEMU} ${QEMUOPT} ${19_day_harib16e_OS}
  DEPENDS 19_day_harib16e_img
)
add_custom_target(19_day_harib16e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${19_day_harib16e_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 19_day_harib16e_img
)
add_custom_target(19_day_harib16e_clean
  COMMAND rm -f ${19_day_harib16e_OS}
  COMMAND rm -f ${19_day_harib16e_SYS}
  COMMAND rm -f ${19_day_harib16e_IPLB}
  COMMAND rm -f ${19_day_harib16e_HEADB}
  COMMAND rm -f ${19_day_harib16e_BOOTB}
  COMMAND rm -f ${19_day_harib16e_WILDOBJ}
)
add_custom_target(19_day_harib16e_ipl
  COMMAND ${NASK} ${19_day_harib16e_IPLS} ${19_day_harib16e_IPLB}
)
add_custom_target(19_day_harib16e_sys
  COMMAND ${NASK} ${19_day_harib16e_HEADS} ${19_day_harib16e_HEADB}
  COMMAND ${FONT} ${19_day_harib16e_FONTS} ${19_day_harib16e_FONTB}
  COMMAND ${B2O}  ${19_day_harib16e_FONTB} ${19_day_harib16e_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${19_day_harib16e_LIBGE} ${19_day_harib16e_LIBGC}
  COMMAND ${NASK} ${19_day_harib16e_FUNCS} ${19_day_harib16e_FUNCO}
  COMMAND gcc ${BINOPT} -T ${19_day_harib16e_LDS} ${19_day_harib16e_CCS} ${19_day_harib16e_WILDOBJ} -o ${19_day_harib16e_BOOTB}
  COMMAND cat ${19_day_harib16e_HEADB} ${19_day_harib16e_BOOTB} > ${19_day_harib16e_SYS}
  DEPENDS 19_day_harib16e_ipl
)
add_custom_target(19_day_harib16e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${19_day_harib16e_IPLB} -i ${19_day_harib16e_OS}
  COMMAND mcopy -i ${19_day_harib16e_OS} ${19_day_harib16e_SYS} ::
  DEPENDS 19_day_harib16e_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/asmhead_wine.img)

add_custom_target(19_day_harib16e_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(19_day_harib16e_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(19_day_harib16e_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_asmhead_od 19_day_harib16e_asmhead)
add_dependencies(19_day_harib16e_asmhead_od 19_day_harib16e_asmhead_wine)

add_dependencies(images 19_day_harib16e_asmhead)
add_dependencies(wine 19_day_harib16e_asmhead_wine)
add_dependencies(od 19_day_harib16e_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(hlt.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/hlt.nas)
set(hlt.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/hlt.img)
set(hlt_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/hlt_wine.img)

add_custom_target(19_day_harib16e_hlt
  COMMAND ${root_BINARY_DIR}/src/opennask ${hlt.img_SRCS} ${hlt.img_OUTS}
)
set(hlt.img_OBJ  ${root_BINARY_DIR}/projects/19_day/harib16e/hlt.o)
add_custom_target(19_day_harib16e_hlt_obj
  COMMAND ${root_BINARY_DIR}/src/opennask ${hlt.img_SRCS} ${hlt.img_OBJ}
)

add_dependencies(19_day_harib16e_sys 19_day_harib16e_hlt_obj)

add_custom_target(19_day_harib16e_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${hlt.img_SRCS} ${hlt_wine.img_OUTS}
)
add_custom_target(19_day_harib16e_hlt_od
  COMMAND ${OD} -t x1 ${hlt.img_OUTS}      > ${hlt.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${hlt_wine.img_OUTS} > ${hlt_wine.img_OUTS}_t.txt
  COMMAND diff -s ${hlt.img_OUTS}_f.txt ${hlt_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_hlt_od 19_day_harib16e_hlt)
add_dependencies(19_day_harib16e_hlt_od 19_day_harib16e_hlt_wine)

add_dependencies(images 19_day_harib16e_hlt)
add_dependencies(wine 19_day_harib16e_hlt_wine)
add_dependencies(od 19_day_harib16e_hlt_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/ipl10_wine.img)

add_custom_target(19_day_harib16e_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(19_day_harib16e_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(19_day_harib16e_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl10.img_OUTS}_f.txt ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_ipl10_od 19_day_harib16e_ipl10)
add_dependencies(19_day_harib16e_ipl10_od 19_day_harib16e_ipl10_wine)

add_dependencies(images 19_day_harib16e_ipl10)
add_dependencies(wine 19_day_harib16e_ipl10_wine)
add_dependencies(od 19_day_harib16e_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/19_day/harib16e/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/19_day/harib16e/naskfunc_wine.img)

add_custom_target(19_day_harib16e_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(19_day_harib16e_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(19_day_harib16e_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(19_day_harib16e_naskfunc_od 19_day_harib16e_naskfunc)
add_dependencies(19_day_harib16e_naskfunc_od 19_day_harib16e_naskfunc_wine)

add_dependencies(images 19_day_harib16e_naskfunc)
add_dependencies(wine 19_day_harib16e_naskfunc_wine)
add_dependencies(od 19_day_harib16e_naskfunc_od)

#----------------------------------------------------------
