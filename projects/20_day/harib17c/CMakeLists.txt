#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17c_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17c/*.o)

# Full OS build rules
set(20_day_harib17c_OS    ${root_BINARY_DIR}/projects/20_day/harib17c/os.img)
set(20_day_harib17c_SYS   ${root_BINARY_DIR}/projects/20_day/harib17c/os.sys)
set(20_day_harib17c_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17c/ipl.bin)
set(20_day_harib17c_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17c/ipl10.nas)
set(20_day_harib17c_HEADB ${root_BINARY_DIR}/projects/20_day/harib17c/asmhead.bin)
set(20_day_harib17c_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17c/asmhead.nas)
set(20_day_harib17c_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17c/boot.bin)
set(20_day_harib17c_CCO   ${root_BINARY_DIR}/projects/20_day/harib17c/boot.o)
set(20_day_harib17c_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17c/*.c)
set(20_day_harib17c_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17c_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17c/naskfunc.nas)
set(20_day_harib17c_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17c/naskfunc.o)
set(20_day_harib17c_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17c/hankaku.txt)
set(20_day_harib17c_FONTB ${root_BINARY_DIR}/projects/20_day/harib17c/hankaku.bin)
set(20_day_harib17c_FONTO ${root_BINARY_DIR}/projects/20_day/harib17c/hankaku.o)
set(20_day_harib17c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17c_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17c/libgo.o)

add_custom_target(20_day_harib17c_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17c_OS}
  DEPENDS 20_day_harib17c_img
)
add_custom_target(20_day_harib17c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17c_img
)
add_custom_target(20_day_harib17c_clean
  COMMAND rm -f ${20_day_harib17c_OS}
  COMMAND rm -f ${20_day_harib17c_SYS}
  COMMAND rm -f ${20_day_harib17c_IPLB}
  COMMAND rm -f ${20_day_harib17c_HEADB}
  COMMAND rm -f ${20_day_harib17c_BOOTB}
  COMMAND rm -f ${20_day_harib17c_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17c/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17c/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17c/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17c/*.txt
)
add_custom_target(20_day_harib17c_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17c_IPLS} ${20_day_harib17c_IPLB}
  DEPENDS ${20_day_harib17c_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17c_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17c_HEADS} ${20_day_harib17c_HEADB}
  DEPENDS ${20_day_harib17c_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17c"
)

add_custom_target(20_day_harib17c_sys
  COMMAND ${FONT} ${20_day_harib17c_FONTS} ${20_day_harib17c_FONTB}
  COMMAND ${B2O}  ${20_day_harib17c_FONTB} ${20_day_harib17c_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17c_LIBGE} ${20_day_harib17c_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17c_FUNCS} ${20_day_harib17c_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17c_LDS} ${20_day_harib17c_CCS} ${20_day_harib17c_WILDOBJ} -o ${20_day_harib17c_BOOTB}
  COMMAND cat ${20_day_harib17c_HEADB} ${20_day_harib17c_BOOTB} > ${20_day_harib17c_SYS}
  DEPENDS ${20_day_harib17c_HEADB} 20_day_harib17c_ipl_build
  DEPENDS ${20_day_harib17c_FUNCO}
  DEPENDS ${20_day_harib17c_FONTO}
)
add_custom_target(20_day_harib17c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17c_IPLB} -i ${20_day_harib17c_OS}
  COMMAND mcopy -i ${20_day_harib17c_OS} ${20_day_harib17c_SYS} ::
  DEPENDS 20_day_harib17c_sys
)

########### next target ###############
set(20_day_harib17c_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17c/asmhead.nas)
set(20_day_harib17c_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/asmhead.bin)

add_custom_target(20_day_harib17c_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17c_asmhead_SRCS} ${20_day_harib17c_asmhead_OUTS}
  DEPENDS ${20_day_harib17c_asmhead_SRCS}
)

set(20_day_harib17c_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/asmhead_wine.bin)
add_custom_target(20_day_harib17c_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17c_asmhead_SRCS} ${20_day_harib17c_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17c_asmhead_wine)

add_custom_target(20_day_harib17c_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17c_asmhead_OUTS}      > ${20_day_harib17c_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17c_asmhead_WINE_OUTS} > ${20_day_harib17c_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17c_asmhead_OUTS}_f.txt ${20_day_harib17c_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17c_asmhead_od 20_day_harib17c_asmhead)
add_dependencies(20_day_harib17c_asmhead_od 20_day_harib17c_asmhead_wine)
add_dependencies(od 20_day_harib17c_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17c_hlt_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17c/hlt.nas)
set(20_day_harib17c_hlt_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/hlt.img)

add_custom_target(20_day_harib17c_hlt
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17c_hlt_SRCS} ${20_day_harib17c_hlt_OUTS}
  DEPENDS ${20_day_harib17c_hlt_SRCS}
)

add_dependencies(images 20_day_harib17c_hlt)
set(20_day_harib17c_hlt_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/hlt_wine.img)
add_custom_target(20_day_harib17c_hlt_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17c_hlt_SRCS} ${20_day_harib17c_hlt_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17c_hlt_wine)

add_custom_target(20_day_harib17c_hlt_od
  COMMAND ${OD} -t x1 ${20_day_harib17c_hlt_OUTS}      > ${20_day_harib17c_hlt_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17c_hlt_WINE_OUTS} > ${20_day_harib17c_hlt_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17c_hlt_OUTS}_f.txt ${20_day_harib17c_hlt_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17c_hlt_od 20_day_harib17c_hlt)
add_dependencies(20_day_harib17c_hlt_od 20_day_harib17c_hlt_wine)
add_dependencies(od 20_day_harib17c_hlt_od)

set(20_day_harib17c_hlt_HRB ${root_BINARY_DIR}/projects/20_day/harib17c/hlt.hrb)
add_custom_target(20_day_harib17c_hlt_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${20_day_harib17c_hlt_SRCS} ${20_day_harib17c_hlt_HRB}
  COMMAND mcopy -i ${20_day_harib17c_OS} ${20_day_harib17c_hlt_HRB} ::hlt.hrb
  DEPENDS 20_day_harib17c_img
)

add_dependencies(20_day_harib17c_run 20_day_harib17c_hlt_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17c_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17c/ipl10.nas)
set(20_day_harib17c_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/ipl10.bin)

add_custom_target(20_day_harib17c_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17c_ipl10_SRCS} ${20_day_harib17c_ipl10_OUTS}
  DEPENDS ${20_day_harib17c_ipl10_SRCS}
)

set(20_day_harib17c_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/ipl10_wine.bin)
add_custom_target(20_day_harib17c_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17c_ipl10_SRCS} ${20_day_harib17c_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17c_ipl10_wine)

add_custom_target(20_day_harib17c_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17c_ipl10_OUTS}      > ${20_day_harib17c_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17c_ipl10_WINE_OUTS} > ${20_day_harib17c_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17c_ipl10_OUTS}_f.txt ${20_day_harib17c_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17c_ipl10_od 20_day_harib17c_ipl10)
add_dependencies(20_day_harib17c_ipl10_od 20_day_harib17c_ipl10_wine)
add_dependencies(od 20_day_harib17c_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17c_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17c/naskfunc.nas)
set(20_day_harib17c_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/naskfunc.img)

add_custom_target(20_day_harib17c_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17c_naskfunc_SRCS} ${20_day_harib17c_naskfunc_OUTS}
  DEPENDS ${20_day_harib17c_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17c_naskfunc)
set(20_day_harib17c_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17c/naskfunc_wine.img)
add_custom_target(20_day_harib17c_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17c_naskfunc_SRCS} ${20_day_harib17c_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17c_naskfunc_wine)

add_custom_target(20_day_harib17c_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17c_naskfunc_OUTS}      > ${20_day_harib17c_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17c_naskfunc_WINE_OUTS} > ${20_day_harib17c_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17c_naskfunc_OUTS}_f.txt ${20_day_harib17c_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17c_naskfunc_od 20_day_harib17c_naskfunc)
add_dependencies(20_day_harib17c_naskfunc_od 20_day_harib17c_naskfunc_wine)
add_dependencies(od 20_day_harib17c_naskfunc_od)

#----------------------------------------------------------
