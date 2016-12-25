#----------------------------------------------------------
message(STATUS "Entering directory projects/30_day/harib27c/haribote/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(30_day_harib27c_haribote_OS    ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/os.img)
set(30_day_harib27c_haribote_SYS	  ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/os.sys)
set(30_day_harib27c_haribote_IPLB  ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/ipl.bin)
set(30_day_harib27c_haribote_IPLS  ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/ipl10.nas)
set(30_day_harib27c_haribote_HEADB ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/asmhead.bin)
set(30_day_harib27c_haribote_HEADS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/asmhead.nas)
set(30_day_harib27c_haribote_BOOTB ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/boot.bin)
set(30_day_harib27c_haribote_CCO	  ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/boot.o)
set(30_day_harib27c_haribote_CCS	  ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/*.c)
set(30_day_harib27c_haribote_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(30_day_harib27c_haribote_FUNCS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/naskfunc.nas)
set(30_day_harib27c_haribote_FUNCO ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/naskfunc.o)
set(30_day_harib27c_haribote_FONTS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/hankaku.txt)
set(30_day_harib27c_haribote_FONTB ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/hankaku.bin)
set(30_day_harib27c_haribote_FONTO ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/hankaku.o)
set(30_day_harib27c_haribote_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(30_day_harib27c_haribote_LIBGC ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/libgo.o)
set(30_day_harib27c_haribote_WILDOBJ ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/*.o)


add_custom_target(30_day_harib27c_haribote_run
  COMMAND ${QEMU} ${QEMUOPT} ${30_day_harib27c_haribote_OS}
  DEPENDS 30_day_harib27c_haribote_img
)
add_custom_target(30_day_harib27c_haribote_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${30_day_harib27c_haribote_OS} -redir tcp:5555:127.0.0.1:1234 &
  DEPENDS 30_day_harib27c_haribote_img
)
add_custom_target(30_day_harib27c_haribote_clean
  COMMAND rm -f ${30_day_harib27c_haribote_OS}
  COMMAND rm -f ${30_day_harib27c_haribote_SYS}
  COMMAND rm -f ${30_day_harib27c_haribote_IPLB}
  COMMAND rm -f ${30_day_harib27c_haribote_HEADB}
  COMMAND rm -f ${30_day_harib27c_haribote_BOOTB}
  COMMAND rm -f ${30_day_harib27c_haribote_WILDOBJ}
)
add_custom_target(30_day_harib27c_haribote_ipl
  COMMAND ${NASK} ${30_day_harib27c_haribote_IPLS} ${30_day_harib27c_haribote_IPLB}
)
add_custom_target(30_day_harib27c_haribote_sys
  COMMAND ${NASK} ${30_day_harib27c_haribote_HEADS} ${30_day_harib27c_haribote_HEADB}
  COMMAND ${FONT} ${30_day_harib27c_haribote_FONTS} ${30_day_harib27c_haribote_FONTB}
  COMMAND ${B2O}  ${30_day_harib27c_haribote_FONTB} ${30_day_harib27c_haribote_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${30_day_harib27c_haribote_LIBGE} ${30_day_harib27c_haribote_LIBGC}
  COMMAND ${NASK} ${30_day_harib27c_haribote_FUNCS} ${30_day_harib27c_haribote_FUNCO}
  COMMAND gcc ${BINOPT} -T ${30_day_harib27c_haribote_LDS} ${30_day_harib27c_haribote_CCS} ${30_day_harib27c_haribote_WILDOBJ} -o ${30_day_harib27c_haribote_BOOTB}
  COMMAND cat ${30_day_harib27c_haribote_HEADB} ${30_day_harib27c_haribote_BOOTB} > ${30_day_harib27c_haribote_SYS}
  DEPENDS 30_day_harib27c_haribote_ipl
)
add_custom_target(30_day_harib27c_haribote_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${30_day_harib27c_haribote_IPLB} -i ${30_day_harib27c_haribote_OS}
  COMMAND mcopy -i ${30_day_harib27c_haribote_OS} ${30_day_harib27c_haribote_SYS} ::
  DEPENDS 30_day_harib27c_haribote_sys
)
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/asmhead_wine.img)

add_custom_target(30_day_harib27c_haribote_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(30_day_harib27c_haribote_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(30_day_harib27c_haribote_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
  COMMAND diff -s ${asmhead.img_OUTS}_f.txt ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(30_day_harib27c_haribote_asmhead_od 30_day_harib27c_haribote_asmhead)
add_dependencies(30_day_harib27c_haribote_asmhead_od 30_day_harib27c_haribote_asmhead_wine)

add_dependencies(images 30_day_harib27c_haribote_asmhead)
add_dependencies(wine 30_day_harib27c_haribote_asmhead_wine)
add_dependencies(od 30_day_harib27c_haribote_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl20.img_SRCS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/ipl20.nas)
set(ipl20.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/ipl20.img)
set(ipl20_wine.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/ipl20_wine.img)

add_custom_target(30_day_harib27c_haribote_ipl20
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl20.img_SRCS} ${ipl20.img_OUTS}
)
set(ipl20.img_HRB ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/ipl20.hrb)
add_custom_target(30_day_harib27c_haribote_ipl20_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl20.img_SRCS} ${ipl20.img_HRB}
  COMMAND mcopy -i ${30_day_harib27c_haribote_OS} ${ipl20.img_HRB} ::ipl20.hrb
  DEPENDS 30_day_harib27c_haribote_img
)

add_dependencies(30_day_harib27c_haribote_run 30_day_harib27c_haribote_ipl20_hrb)
add_custom_target(30_day_harib27c_haribote_ipl20_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl20.img_SRCS} ${ipl20_wine.img_OUTS}
)
add_custom_target(30_day_harib27c_haribote_ipl20_od
  COMMAND ${OD} -t x1 ${ipl20.img_OUTS}      > ${ipl20.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl20_wine.img_OUTS} > ${ipl20_wine.img_OUTS}_t.txt
  COMMAND diff -s ${ipl20.img_OUTS}_f.txt ${ipl20_wine.img_OUTS}_t.txt
)

add_dependencies(30_day_harib27c_haribote_ipl20_od 30_day_harib27c_haribote_ipl20)
add_dependencies(30_day_harib27c_haribote_ipl20_od 30_day_harib27c_haribote_ipl20_wine)

add_dependencies(images 30_day_harib27c_haribote_ipl20)
add_dependencies(wine 30_day_harib27c_haribote_ipl20_wine)
add_dependencies(od 30_day_harib27c_haribote_ipl20_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/30_day/harib27c/haribote/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/naskfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/30_day/harib27c/haribote/naskfunc_wine.img)

add_custom_target(30_day_harib27c_haribote_naskfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(30_day_harib27c_haribote_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(30_day_harib27c_haribote_naskfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
  COMMAND diff -s ${naskfunc.img_OUTS}_f.txt ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(30_day_harib27c_haribote_naskfunc_od 30_day_harib27c_haribote_naskfunc)
add_dependencies(30_day_harib27c_haribote_naskfunc_od 30_day_harib27c_haribote_naskfunc_wine)

add_dependencies(images 30_day_harib27c_haribote_naskfunc)
add_dependencies(wine 30_day_harib27c_haribote_naskfunc_wine)
add_dependencies(od 30_day_harib27c_haribote_naskfunc_od)

#----------------------------------------------------------
