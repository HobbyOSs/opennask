#----------------------------------------------------------
message(STATUS "Entering directory projects/20_day/harib17g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(20_day_harib17g_WILDOBJ ${root_BINARY_DIR}/projects/20_day/harib17g/*.o)

# Full OS build rules
set(20_day_harib17g_OS    ${root_BINARY_DIR}/projects/20_day/harib17g/os.img)
set(20_day_harib17g_SYS   ${root_BINARY_DIR}/projects/20_day/harib17g/os.sys)
set(20_day_harib17g_IPLB  ${root_BINARY_DIR}/projects/20_day/harib17g/ipl.bin)
set(20_day_harib17g_IPLS  ${root_SOURCE_DIR}/projects/20_day/harib17g/ipl10.nas)
set(20_day_harib17g_HEADB ${root_BINARY_DIR}/projects/20_day/harib17g/asmhead.bin)
set(20_day_harib17g_HEADS ${root_SOURCE_DIR}/projects/20_day/harib17g/asmhead.nas)
set(20_day_harib17g_BOOTB ${root_BINARY_DIR}/projects/20_day/harib17g/boot.bin)
set(20_day_harib17g_CCO   ${root_BINARY_DIR}/projects/20_day/harib17g/boot.o)
set(20_day_harib17g_CCS   ${root_SOURCE_DIR}/projects/20_day/harib17g/*.c)
set(20_day_harib17g_LDS   ${root_SOURCE_DIR}/projects/os.lds)
set(20_day_harib17g_FUNCS ${root_SOURCE_DIR}/projects/20_day/harib17g/naskfunc.nas)
set(20_day_harib17g_FUNCO ${root_BINARY_DIR}/projects/20_day/harib17g/naskfunc.o)
set(20_day_harib17g_FONTS ${root_SOURCE_DIR}/projects/20_day/harib17g/hankaku.txt)
set(20_day_harib17g_FONTB ${root_BINARY_DIR}/projects/20_day/harib17g/hankaku.bin)
set(20_day_harib17g_FONTO ${root_BINARY_DIR}/projects/20_day/harib17g/hankaku.o)
set(20_day_harib17g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(20_day_harib17g_LIBGC ${root_BINARY_DIR}/projects/20_day/harib17g/libgo.o)

add_custom_target(20_day_harib17g_run
  COMMAND ${QEMU} ${QEMUOPT} ${20_day_harib17g_OS}
  DEPENDS 20_day_harib17g_img
)
add_custom_target(20_day_harib17g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${20_day_harib17g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 20_day_harib17g_img
)
add_custom_target(20_day_harib17g_clean
  COMMAND rm -f ${20_day_harib17g_OS}
  COMMAND rm -f ${20_day_harib17g_SYS}
  COMMAND rm -f ${20_day_harib17g_IPLB}
  COMMAND rm -f ${20_day_harib17g_HEADB}
  COMMAND rm -f ${20_day_harib17g_BOOTB}
  COMMAND rm -f ${20_day_harib17g_WILDOBJ}
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17g/ipl10.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17g/asmhead.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17g/*_wine.bin
  COMMAND rm -f ${root_BINARY_DIR}/projects/20_day/harib17g/*.txt
)
add_custom_target(20_day_harib17g_ipl_build
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17g_IPLS} ${20_day_harib17g_IPLB}
  DEPENDS ${20_day_harib17g_IPLS}
)
add_custom_command(
  OUTPUT ${20_day_harib17g_HEADB}
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17g_HEADS} ${20_day_harib17g_HEADB}
  DEPENDS ${20_day_harib17g_HEADS}
  COMMENT "Generating asmhead.bin for 20_day_harib17g"
)

add_custom_target(20_day_harib17g_sys
  COMMAND ${FONT} ${20_day_harib17g_FONTS} ${20_day_harib17g_FONTB}
  COMMAND ${B2O}  ${20_day_harib17g_FONTB} ${20_day_harib17g_FONTO} _hankaku
  COMMAND ${CONV} -fcoff32 -nu ${20_day_harib17g_LIBGE} ${20_day_harib17g_LIBGC}
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17g_FUNCS} ${20_day_harib17g_FUNCO}
  COMMAND gcc ${BINOPT} -T ${20_day_harib17g_LDS} ${20_day_harib17g_CCS} ${20_day_harib17g_WILDOBJ} -o ${20_day_harib17g_BOOTB}
  COMMAND cat ${20_day_harib17g_HEADB} ${20_day_harib17g_BOOTB} > ${20_day_harib17g_SYS}
  DEPENDS ${20_day_harib17g_HEADB} 20_day_harib17g_ipl_build
  DEPENDS ${20_day_harib17g_FUNCO}
  DEPENDS ${20_day_harib17g_FONTO}
)
add_custom_target(20_day_harib17g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${20_day_harib17g_IPLB} -i ${20_day_harib17g_OS}
  COMMAND mcopy -i ${20_day_harib17g_OS} ${20_day_harib17g_SYS} ::
  DEPENDS 20_day_harib17g_sys
)

########### next target ###############
set(20_day_harib17g_asmhead_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/asmhead.nas)
set(20_day_harib17g_asmhead_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/asmhead.bin)

add_custom_target(20_day_harib17g_asmhead
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17g_asmhead_SRCS} ${20_day_harib17g_asmhead_OUTS}
  DEPENDS ${20_day_harib17g_asmhead_SRCS}
)

set(20_day_harib17g_asmhead_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/asmhead_wine.bin)
add_custom_target(20_day_harib17g_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17g_asmhead_SRCS} ${20_day_harib17g_asmhead_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17g_asmhead_wine)

add_custom_target(20_day_harib17g_asmhead_od
  COMMAND ${OD} -t x1 ${20_day_harib17g_asmhead_OUTS}      > ${20_day_harib17g_asmhead_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17g_asmhead_WINE_OUTS} > ${20_day_harib17g_asmhead_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17g_asmhead_OUTS}_f.txt ${20_day_harib17g_asmhead_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_asmhead_od 20_day_harib17g_asmhead)
add_dependencies(20_day_harib17g_asmhead_od 20_day_harib17g_asmhead_wine)
add_dependencies(od 20_day_harib17g_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17g_hello_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/hello.nas)
set(20_day_harib17g_hello_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/hello.img)

add_custom_target(20_day_harib17g_hello
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17g_hello_SRCS} ${20_day_harib17g_hello_OUTS}
  DEPENDS ${20_day_harib17g_hello_SRCS}
)

add_dependencies(images 20_day_harib17g_hello)
set(20_day_harib17g_hello_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/hello_wine.img)
add_custom_target(20_day_harib17g_hello_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17g_hello_SRCS} ${20_day_harib17g_hello_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17g_hello_wine)

add_custom_target(20_day_harib17g_hello_od
  COMMAND ${OD} -t x1 ${20_day_harib17g_hello_OUTS}      > ${20_day_harib17g_hello_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17g_hello_WINE_OUTS} > ${20_day_harib17g_hello_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17g_hello_OUTS}_f.txt ${20_day_harib17g_hello_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_hello_od 20_day_harib17g_hello)
add_dependencies(20_day_harib17g_hello_od 20_day_harib17g_hello_wine)
add_dependencies(od 20_day_harib17g_hello_od)

set(20_day_harib17g_hello_HRB ${root_BINARY_DIR}/projects/20_day/harib17g/hello.hrb)
add_custom_target(20_day_harib17g_hello_hrb
  COMMAND ${root_BINARY_DIR}/src/opennask ${20_day_harib17g_hello_SRCS} ${20_day_harib17g_hello_HRB}
  COMMAND mcopy -i ${20_day_harib17g_OS} ${20_day_harib17g_hello_HRB} ::hello.hrb
  DEPENDS 20_day_harib17g_img
)

add_dependencies(20_day_harib17g_run 20_day_harib17g_hello_hrb)
#----------------------------------------------------------
########### next target ###############
set(20_day_harib17g_ipl10_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/ipl10.nas)
set(20_day_harib17g_ipl10_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/ipl10.bin)

add_custom_target(20_day_harib17g_ipl10
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17g_ipl10_SRCS} ${20_day_harib17g_ipl10_OUTS}
  DEPENDS ${20_day_harib17g_ipl10_SRCS}
)

set(20_day_harib17g_ipl10_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/ipl10_wine.bin)
add_custom_target(20_day_harib17g_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17g_ipl10_SRCS} ${20_day_harib17g_ipl10_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17g_ipl10_wine)

add_custom_target(20_day_harib17g_ipl10_od
  COMMAND ${OD} -t x1 ${20_day_harib17g_ipl10_OUTS}      > ${20_day_harib17g_ipl10_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17g_ipl10_WINE_OUTS} > ${20_day_harib17g_ipl10_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17g_ipl10_OUTS}_f.txt ${20_day_harib17g_ipl10_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_ipl10_od 20_day_harib17g_ipl10)
add_dependencies(20_day_harib17g_ipl10_od 20_day_harib17g_ipl10_wine)
add_dependencies(od 20_day_harib17g_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(20_day_harib17g_naskfunc_SRCS ${root_SOURCE_DIR}/projects/20_day/harib17g/naskfunc.nas)
set(20_day_harib17g_naskfunc_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/naskfunc.img)

add_custom_target(20_day_harib17g_naskfunc
  COMMAND ${GOSK_EXECUTABLE} ${20_day_harib17g_naskfunc_SRCS} ${20_day_harib17g_naskfunc_OUTS}
  DEPENDS ${20_day_harib17g_naskfunc_SRCS}
)

add_dependencies(images 20_day_harib17g_naskfunc)
set(20_day_harib17g_naskfunc_WINE_OUTS ${root_BINARY_DIR}/projects/20_day/harib17g/naskfunc_wine.img)
add_custom_target(20_day_harib17g_naskfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${20_day_harib17g_naskfunc_SRCS} ${20_day_harib17g_naskfunc_WINE_OUTS}
)
add_dependencies(wine 20_day_harib17g_naskfunc_wine)

add_custom_target(20_day_harib17g_naskfunc_od
  COMMAND ${OD} -t x1 ${20_day_harib17g_naskfunc_OUTS}      > ${20_day_harib17g_naskfunc_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${20_day_harib17g_naskfunc_WINE_OUTS} > ${20_day_harib17g_naskfunc_WINE_OUTS}_t.txt
  COMMAND diff -s ${20_day_harib17g_naskfunc_OUTS}_f.txt ${20_day_harib17g_naskfunc_WINE_OUTS}_t.txt
)

add_dependencies(20_day_harib17g_naskfunc_od 20_day_harib17g_naskfunc)
add_dependencies(20_day_harib17g_naskfunc_od 20_day_harib17g_naskfunc_wine)
add_dependencies(od 20_day_harib17g_naskfunc_od)

#----------------------------------------------------------
