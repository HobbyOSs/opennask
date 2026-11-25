#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20a_OS    ${root_BINARY_DIR}/projects/23_day/harib20a/os.img)
set(23_day_harib20a_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20a/os.sys)
set(23_day_harib20a_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20a/ipl.bin)
set(23_day_harib20a_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20a/ipl10.nas)
set(23_day_harib20a_HEADB ${root_BINARY_DIR}/projects/23_day/harib20a/asmhead.bin)
set(23_day_harib20a_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20a/asmhead.nas)
set(23_day_harib20a_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20a/boot.bin)
set(23_day_harib20a_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20a/boot.o)
file(GLOB 23_day_harib20a_CCS ${root_SOURCE_DIR}/projects/23_day/harib20a/*.c)
list(REMOVE_ITEM 23_day_harib20a_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20a/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20a/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20a/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo3.c)
set(23_day_harib20a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20a_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20a/naskfunc.nas)
set(23_day_harib20a_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20a/naskfunc.o)
set(23_day_harib20a_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20a/hankaku.txt)
set(23_day_harib20a_FONTB ${root_BINARY_DIR}/projects/23_day/harib20a/hankaku.bin)
set(23_day_harib20a_FONTO ${root_BINARY_DIR}/projects/23_day/harib20a/hankaku.o)
set(23_day_harib20a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20a_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20a/libgo.o)
set(23_day_harib20a_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20a/*.o)


add_custom_target(23_day_harib20a_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20a_OS}
  DEPENDS 23_day_harib20a_img
)
add_custom_target(23_day_harib20a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20a_img
)
add_custom_target(23_day_harib20a_clean
  COMMAND rm -f ${23_day_harib20a_OS}
  COMMAND rm -f ${23_day_harib20a_SYS}
  COMMAND rm -f ${23_day_harib20a_IPLB}
  COMMAND rm -f ${23_day_harib20a_HEADB}
  COMMAND rm -f ${23_day_harib20a_BOOTB}
  COMMAND rm -f ${23_day_harib20a_WILDOBJ}
)
add_custom_target(23_day_harib20a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20a_IPLS} ${23_day_harib20a_IPLB}
  DEPENDS ${23_day_harib20a_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20a_HEADS} ${23_day_harib20a_HEADB}
  DEPENDS ${23_day_harib20a_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${23_day_harib20a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20a_FUNCS} ${23_day_harib20a_FUNCO}
  DEPENDS ${23_day_harib20a_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${23_day_harib20a_FONTO}
  COMMAND ${FONT} ${23_day_harib20a_FONTS} ${23_day_harib20a_FONTB}
  COMMAND ${B2O}  ${23_day_harib20a_FONTB} ${23_day_harib20a_FONTO} _hankaku
  DEPENDS ${23_day_harib20a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${23_day_harib20a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20a_LIBGE} ${23_day_harib20a_LIBGC}
  DEPENDS ${23_day_harib20a_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20a"
)

add_custom_target(23_day_harib20a_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_LDS} ${23_day_harib20a_CCS} ${23_day_harib20a_WILDOBJ} -o ${23_day_harib20a_BOOTB}
  COMMAND cat ${23_day_harib20a_HEADB} ${23_day_harib20a_BOOTB} > ${23_day_harib20a_SYS}
  DEPENDS ${23_day_harib20a_HEADB} 23_day_harib20a_ipl
  DEPENDS ${23_day_harib20a_FUNCO}
  DEPENDS ${23_day_harib20a_FONTO} ${23_day_harib20a_LIBGC}
)
add_custom_target(23_day_harib20a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20a_IPLB} -i ${23_day_harib20a_OS}
  COMMAND mcopy -i ${23_day_harib20a_OS} ${23_day_harib20a_SYS} ::
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20a_OS} ${root_BINARY_DIR}/projects/23_day/harib20a/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20a_sys ${root_BINARY_DIR}/projects/23_day/harib20a/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20a/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20a_APIO ${root_BINARY_DIR}/projects/23_day/harib20a/a_nask.o)
set(23_day_harib20a_RANDO ${root_BINARY_DIR}/projects/23_day/harib20a/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${23_day_harib20a_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20a/a_nask.nas ${23_day_harib20a_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20a/a.c ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20a/hello3.c ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20a/hello4.c ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo.c ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo2.c ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20a_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo3.c ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20a_APIO} ${23_day_harib20a_LIBGC} ${23_day_harib20a_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20a/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20a/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20a/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20a/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20a/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20a/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20a"
)
