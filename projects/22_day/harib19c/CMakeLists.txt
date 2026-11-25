#----------------------------------------------------------
message(STATUS "Entering directory projects/22_day/harib19c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(22_day_harib19c_OS    ${root_BINARY_DIR}/projects/22_day/harib19c/os.img)
set(22_day_harib19c_SYS	  ${root_BINARY_DIR}/projects/22_day/harib19c/os.sys)
set(22_day_harib19c_IPLB  ${root_BINARY_DIR}/projects/22_day/harib19c/ipl.bin)
set(22_day_harib19c_IPLS  ${root_SOURCE_DIR}/projects/22_day/harib19c/ipl10.nas)
set(22_day_harib19c_HEADB ${root_BINARY_DIR}/projects/22_day/harib19c/asmhead.bin)
set(22_day_harib19c_HEADS ${root_SOURCE_DIR}/projects/22_day/harib19c/asmhead.nas)
set(22_day_harib19c_BOOTB ${root_BINARY_DIR}/projects/22_day/harib19c/boot.bin)
set(22_day_harib19c_CCO	  ${root_BINARY_DIR}/projects/22_day/harib19c/boot.o)
file(GLOB 22_day_harib19c_CCS ${root_SOURCE_DIR}/projects/22_day/harib19c/*.c)
list(REMOVE_ITEM 22_day_harib19c_CCS
  ${root_SOURCE_DIR}/projects/22_day/harib19c/a.c
  ${root_SOURCE_DIR}/projects/22_day/harib19c/hello3.c
  ${root_SOURCE_DIR}/projects/22_day/harib19c/bug1.c
  ${root_SOURCE_DIR}/projects/22_day/harib19c/bug2.c
  ${root_SOURCE_DIR}/projects/22_day/harib19c/bug3.c)
set(22_day_harib19c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(22_day_harib19c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(22_day_harib19c_FUNCS ${root_SOURCE_DIR}/projects/22_day/harib19c/naskfunc.nas)
set(22_day_harib19c_FUNCO ${root_BINARY_DIR}/projects/22_day/harib19c/naskfunc.o)
set(22_day_harib19c_FONTS ${root_SOURCE_DIR}/projects/22_day/harib19c/hankaku.txt)
set(22_day_harib19c_FONTB ${root_BINARY_DIR}/projects/22_day/harib19c/hankaku.bin)
set(22_day_harib19c_FONTO ${root_BINARY_DIR}/projects/22_day/harib19c/hankaku.o)
set(22_day_harib19c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(22_day_harib19c_LIBGC ${root_BINARY_DIR}/projects/22_day/harib19c/libgo.o)
set(22_day_harib19c_WILDOBJ ${root_BINARY_DIR}/projects/22_day/harib19c/*.o)


add_custom_target(22_day_harib19c_run
  COMMAND ${QEMU} ${QEMUOPT} ${22_day_harib19c_OS}
  DEPENDS 22_day_harib19c_img
)
add_custom_target(22_day_harib19c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${22_day_harib19c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 22_day_harib19c_img
)
add_custom_target(22_day_harib19c_clean
  COMMAND rm -f ${22_day_harib19c_OS}
  COMMAND rm -f ${22_day_harib19c_SYS}
  COMMAND rm -f ${22_day_harib19c_IPLB}
  COMMAND rm -f ${22_day_harib19c_HEADB}
  COMMAND rm -f ${22_day_harib19c_BOOTB}
  COMMAND rm -f ${22_day_harib19c_WILDOBJ}
)
add_custom_target(22_day_harib19c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19c_IPLS} ${22_day_harib19c_IPLB}
  DEPENDS ${22_day_harib19c_IPLS}
)
add_custom_command(
  OUTPUT ${22_day_harib19c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19c_HEADS} ${22_day_harib19c_HEADB}
  DEPENDS ${22_day_harib19c_HEADS}
  COMMENT "Generating asmhead.bin for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${22_day_harib19c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${22_day_harib19c_FUNCS} ${22_day_harib19c_FUNCO}
  DEPENDS ${22_day_harib19c_FUNCS}
  COMMENT "Generating naskfunc.o for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${22_day_harib19c_FONTO}
  COMMAND ${FONT} ${22_day_harib19c_FONTS} ${22_day_harib19c_FONTB}
  COMMAND ${B2O}  ${22_day_harib19c_FONTB} ${22_day_harib19c_FONTO} _hankaku
  DEPENDS ${22_day_harib19c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${22_day_harib19c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${22_day_harib19c_LIBGE} ${22_day_harib19c_LIBGC}
  DEPENDS ${22_day_harib19c_LIBGE}
  COMMENT "Converting golibc for 22_day_harib19c"
)

add_custom_target(22_day_harib19c_sys
  COMMAND gcc ${BINOPT} -T ${22_day_harib19c_LDS} ${22_day_harib19c_CCS} ${22_day_harib19c_WILDOBJ} -o ${22_day_harib19c_BOOTB}
  COMMAND cat ${22_day_harib19c_HEADB} ${22_day_harib19c_BOOTB} > ${22_day_harib19c_SYS}
  DEPENDS ${22_day_harib19c_HEADB} 22_day_harib19c_ipl
  DEPENDS ${22_day_harib19c_FUNCO}
  DEPENDS ${22_day_harib19c_FONTO} ${22_day_harib19c_LIBGC}
)
add_custom_target(22_day_harib19c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${22_day_harib19c_IPLB} -i ${22_day_harib19c_OS}
  COMMAND mcopy -i ${22_day_harib19c_OS} ${22_day_harib19c_SYS} ::
  COMMAND mcopy -i ${22_day_harib19c_OS} ${root_BINARY_DIR}/projects/22_day/harib19c/a.hrb ::a.hrb
  COMMAND mcopy -i ${22_day_harib19c_OS} ${root_BINARY_DIR}/projects/22_day/harib19c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${22_day_harib19c_OS} ${root_BINARY_DIR}/projects/22_day/harib19c/bug1.hrb ::bug1.hrb
  COMMAND mcopy -i ${22_day_harib19c_OS} ${root_BINARY_DIR}/projects/22_day/harib19c/bug2.hrb ::bug2.hrb
  COMMAND mcopy -i ${22_day_harib19c_OS} ${root_BINARY_DIR}/projects/22_day/harib19c/bug3.hrb ::bug3.hrb
  COMMAND mcopy -i ${22_day_harib19c_OS} ${root_BINARY_DIR}/projects/22_day/harib19c/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${22_day_harib19c_OS} ${root_BINARY_DIR}/projects/22_day/harib19c/hello2.hrb ::hello2.hrb
  DEPENDS 22_day_harib19c_sys ${root_BINARY_DIR}/projects/22_day/harib19c/a.hrb ${root_BINARY_DIR}/projects/22_day/harib19c/hello3.hrb ${root_BINARY_DIR}/projects/22_day/harib19c/bug1.hrb ${root_BINARY_DIR}/projects/22_day/harib19c/bug2.hrb ${root_BINARY_DIR}/projects/22_day/harib19c/bug3.hrb ${root_BINARY_DIR}/projects/22_day/harib19c/hello.hrb ${root_BINARY_DIR}/projects/22_day/harib19c/hello2.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(22_day_harib19c_APIO ${root_BINARY_DIR}/projects/22_day/harib19c/a_nask.o)
add_custom_command(
  OUTPUT ${22_day_harib19c_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19c/a_nask.nas ${22_day_harib19c_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/a_nask.nas
  COMMENT "Generating a_nask.o for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19c/a.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19c_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19c/a.c ${22_day_harib19c_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/a.c ${22_day_harib19c_APIO}
  COMMENT "Generating a.hrb for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19c/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19c_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19c/hello3.c ${22_day_harib19c_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/hello3.c ${22_day_harib19c_APIO}
  COMMENT "Generating hello3.hrb for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19c/bug1.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19c_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19c/bug1.c ${22_day_harib19c_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19c/bug1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/bug1.c ${22_day_harib19c_APIO}
  COMMENT "Generating bug1.hrb for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19c/bug2.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19c_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19c/bug2.c ${22_day_harib19c_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19c/bug2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/bug2.c ${22_day_harib19c_APIO}
  COMMENT "Generating bug2.hrb for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19c/bug3.hrb
  COMMAND gcc ${BINOPT} -T ${22_day_harib19c_APPLDS} ${root_SOURCE_DIR}/projects/22_day/harib19c/bug3.c ${22_day_harib19c_APIO} -o ${root_BINARY_DIR}/projects/22_day/harib19c/bug3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/bug3.c ${22_day_harib19c_APIO}
  COMMENT "Generating bug3.hrb for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19c/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19c/hello.nas ${root_BINARY_DIR}/projects/22_day/harib19c/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/hello.nas
  COMMENT "Generating hello.hrb for 22_day_harib19c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/22_day/harib19c/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/22_day/harib19c/hello2.nas ${root_BINARY_DIR}/projects/22_day/harib19c/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/22_day/harib19c/hello2.nas
  COMMENT "Generating hello2.hrb for 22_day_harib19c"
)
