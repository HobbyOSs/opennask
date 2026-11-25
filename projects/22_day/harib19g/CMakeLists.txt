#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19g_OS    ${root_BINARY_DIR}/projects/22_day/harib19g/os.img)
set(22_day_harib19g_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19g/os.sys)
set(22_day_harib19g_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19g/ipl.bin)
set(22_day_harib19g_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19g/ipl10.nas)
set(22_day_harib19g_HEADB ${root_BINARY_DIR}/projects/22_day/harib19g/asmhead.bin)
set(22_day_harib19g_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19g/asmhead.nas)
set(22_day_harib19g_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19g/boot.bin)
set(22_day_harib19g_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19g/boot.o)
file(GLOB 22_day_harib19g_CCS ${root_SOURCE_DIR}/projects/22_day/harib19g/*.c)
list(REMOVE_ITEM 22_day_harib19g_CCS
  ${root_SOURCE_DIR}/projects/22_day/harib19g/a.c
  ${root_SOURCE_DIR}/projects/22_day/harib19g/hello3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19g/hello4.c
  ${root_SOURCE_DIR}/projects/22_day/harib19g/winhelo.c
  ${root_SOURCE_DIR}/projects/22_day/harib19g/winhelo2.c)
set(22_day_harib19g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19g_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(22_day_harib19g_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19g/naskfunc.nas)
set(22_day_harib19g_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19g/naskfunc.o)
set(22_day_harib19g_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19g/hankaku.txt)
set(22_day_harib19g_FONTB ${root_BINARY_DIR}/projects/22_day/harib19g/hankaku.bin)
set(22_day_harib19g_FONTO ${root_BINARY_DIR}/projects/22_day/harib19g/hankaku.o)
set(22_day_harib19g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19g_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19g/libgo.o)
set(22_day_harib19g_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19g/*.o)


add_custom_target(22_day_harib19g_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19g_OS}
  DEPENDS 22_day_harib19g_img
)
add_custom_target(22_day_harib19g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 22_day_harib19g_img
)
add_custom_target(22_day_harib19g_clean
  COMMAND rm -f ${22_day_harib19g_OS}
  COMMAND rm -f ${22_day_harib19g_SYS}
  COMMAND rm -f ${22_day_harib19g_IPLB}
  COMMAND rm -f ${22_day_harib19g_HEADB}
  COMMAND rm -f ${22_day_harib19g_BOOTB}
  COMMAND rm -f ${22_day_harib19g_WILDOBJ}
)
add_custom_target(22_day_harib19g_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19g_IPLS} ${22_day_harib19g_IPLB}
  DEPENDS ${22_day_harib19g_IPLS}
)
add_custom_command(
  OUTPUT ${22_day_harib19g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19g_HEADS} ${22_day_harib19g_HEADB}
  DEPENDS ${22_day_harib19g_HEADS}
  COMMENT "Generating asmhead.bin for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${22_day_harib19g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19g_FUNCS} ${22_day_harib19g_FUNCO}
  DEPENDS ${22_day_harib19g_FUNCS}
  COMMENT "Generating naskfunc.o for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${22_day_harib19g_FONTO}
  COMMAND ${FONT} ${22_day_harib19g_FONTS} ${22_day_harib19g_FONTB}
  COMMAND ${B2O}  ${22_day_harib19g_FONTB} ${22_day_harib19g_FONTO} _hankaku
  DEPENDS ${22_day_harib19g_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${22_day_harib19g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19g_LIBGE} ${22_day_harib19g_LIBGC}
  DEPENDS ${22_day_harib19g_LIBGE}
  COMMENT "Converting golibc for 22_day_harib19g"
)

add_custom_target(22_day_harib19g_sys
  COMMAND gcc ${BINOPT} -T ${22_day_harib19g_LDS} ${22_day_harib19g_CCS} ${22_day_harib19g_WILDOBJ} -o ${22_day_harib19g_BOOTB}
  COMMAND cat ${22_day_harib19g_HEADB} ${22_day_harib19g_BOOTB} > ${22_day_harib19g_SYS}
  DEPENDS ${22_day_harib19g_HEADB} 22_day_harib19g_ipl
  DEPENDS ${22_day_harib19g_FUNCO}
  DEPENDS ${22_day_harib19g_FONTO} ${22_day_harib19g_LIBGC}
)
add_custom_target(22_day_harib19g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19g_IPLB} -i ${22_day_harib19g_OS}
  COMMAND mcopy -i ${22_day_harib19g_OS} ${22_day_harib19g_SYS} ::
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/a.hrb ::a.hrb
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${22_day_harib19g_OS} ${root_BINARY_DIR}/projects/22_day/harib19g/hello5.hrb ::hello5.hrb
  DEPENDS 22_day_harib19g_sys ${root_BINARY_DIR}/projects/22_day/harib19g/a.hrb ${root_BINARY_DIR}/projects/22_day/harib19g/hello3.hrb ${root_BINARY_DIR}/projects/22_day/harib19g/hello4.hrb ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo.hrb ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo2.hrb ${root_BINARY_DIR}/projects/22_day/harib19g/hello.hrb ${root_BINARY_DIR}/projects/22_day/harib19g/hello2.hrb ${root_BINARY_DIR}/projects/22_day/harib19g/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(22_day_harib19g_APIO ${root_BINARY_DIR}/projects/22_day/harib19g/a_nask.o)
add_custom_command(
  OUTPUT ${22_day_harib19g_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19g/a_nask.nas ${22_day_harib19g_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/a_nask.nas
  COMMENT "Generating a_nask.o for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/a.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19g_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19g/a.c ${22_day_harib19g_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19g/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/a.c ${22_day_harib19g_APIO}
  COMMENT "Generating a.hrb for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19g_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19g/hello3.c ${22_day_harib19g_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19g/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello3.c ${22_day_harib19g_APIO}
  COMMENT "Generating hello3.hrb for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19g_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19g/hello4.c ${22_day_harib19g_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19g/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello4.c ${22_day_harib19g_APIO}
  COMMENT "Generating hello4.hrb for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19g_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19g/winhelo.c ${22_day_harib19g_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/winhelo.c ${22_day_harib19g_APIO}
  COMMENT "Generating winhelo.hrb for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19g_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19g/winhelo2.c ${22_day_harib19g_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19g/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/winhelo2.c ${22_day_harib19g_APIO}
  COMMENT "Generating winhelo2.hrb for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19g/hello.nas ${root_BINARY_DIR}/projects/22_day/harib19g/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello.nas
  COMMENT "Generating hello.hrb for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19g/hello2.nas ${root_BINARY_DIR}/projects/22_day/harib19g/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello2.nas
  COMMENT "Generating hello2.hrb for 22_day_harib19g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19g/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19g/hello5.nas ${root_BINARY_DIR}/projects/22_day/harib19g/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19g/hello5.nas
  COMMENT "Generating hello5.hrb for 22_day_harib19g"
)
