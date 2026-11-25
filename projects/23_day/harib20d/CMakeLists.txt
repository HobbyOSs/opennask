#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20d_OS    ${root_BINARY_DIR}/projects/23_day/harib20d/os.img)
set(23_day_harib20d_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20d/os.sys)
set(23_day_harib20d_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20d/ipl.bin)
set(23_day_harib20d_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20d/ipl10.nas)
set(23_day_harib20d_HEADB ${root_BINARY_DIR}/projects/23_day/harib20d/asmhead.bin)
set(23_day_harib20d_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20d/asmhead.nas)
set(23_day_harib20d_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20d/boot.bin)
set(23_day_harib20d_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20d/boot.o)
file(GLOB 23_day_harib20d_CCS ${root_SOURCE_DIR}/projects/23_day/harib20d/*.c)
list(REMOVE_ITEM 23_day_harib20d_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20d/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/star1.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/stars.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/stars2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20d/lines.c)
set(23_day_harib20d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20d_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20d/naskfunc.nas)
set(23_day_harib20d_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20d/naskfunc.o)
set(23_day_harib20d_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20d/hankaku.txt)
set(23_day_harib20d_FONTB ${root_BINARY_DIR}/projects/23_day/harib20d/hankaku.bin)
set(23_day_harib20d_FONTO ${root_BINARY_DIR}/projects/23_day/harib20d/hankaku.o)
set(23_day_harib20d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20d_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20d/libgo.o)
set(23_day_harib20d_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20d/*.o)


add_custom_target(23_day_harib20d_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20d_OS}
  DEPENDS 23_day_harib20d_img
)
add_custom_target(23_day_harib20d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20d_img
)
add_custom_target(23_day_harib20d_clean
  COMMAND rm -f ${23_day_harib20d_OS}
  COMMAND rm -f ${23_day_harib20d_SYS}
  COMMAND rm -f ${23_day_harib20d_IPLB}
  COMMAND rm -f ${23_day_harib20d_HEADB}
  COMMAND rm -f ${23_day_harib20d_BOOTB}
  COMMAND rm -f ${23_day_harib20d_WILDOBJ}
)
add_custom_target(23_day_harib20d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20d_IPLS} ${23_day_harib20d_IPLB}
  DEPENDS ${23_day_harib20d_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20d_HEADS} ${23_day_harib20d_HEADB}
  DEPENDS ${23_day_harib20d_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${23_day_harib20d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20d_FUNCS} ${23_day_harib20d_FUNCO}
  DEPENDS ${23_day_harib20d_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${23_day_harib20d_FONTO}
  COMMAND ${FONT} ${23_day_harib20d_FONTS} ${23_day_harib20d_FONTB}
  COMMAND ${B2O}  ${23_day_harib20d_FONTB} ${23_day_harib20d_FONTO} _hankaku
  DEPENDS ${23_day_harib20d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${23_day_harib20d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20d_LIBGE} ${23_day_harib20d_LIBGC}
  DEPENDS ${23_day_harib20d_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20d"
)

add_custom_target(23_day_harib20d_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_LDS} ${23_day_harib20d_CCS} ${23_day_harib20d_WILDOBJ} -o ${23_day_harib20d_BOOTB}
  COMMAND cat ${23_day_harib20d_HEADB} ${23_day_harib20d_BOOTB} > ${23_day_harib20d_SYS}
  DEPENDS ${23_day_harib20d_HEADB} 23_day_harib20d_ipl
  DEPENDS ${23_day_harib20d_FUNCO}
  DEPENDS ${23_day_harib20d_FONTO} ${23_day_harib20d_LIBGC}
)
add_custom_target(23_day_harib20d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20d_IPLB} -i ${23_day_harib20d_OS}
  COMMAND mcopy -i ${23_day_harib20d_OS} ${23_day_harib20d_SYS} ::
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20d_OS} ${root_BINARY_DIR}/projects/23_day/harib20d/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20d_sys ${root_BINARY_DIR}/projects/23_day/harib20d/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/star1.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/stars.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/stars2.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/lines.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20d/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20d_APIO ${root_BINARY_DIR}/projects/23_day/harib20d/a_nask.o)
set(23_day_harib20d_RANDO ${root_BINARY_DIR}/projects/23_day/harib20d/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${23_day_harib20d_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20d/a_nask.nas ${23_day_harib20d_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/a.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/hello3.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/hello4.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo2.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo3.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/star1.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/star1.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/star1.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating star1.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/stars.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/stars.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/stars.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating stars.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/stars2.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating stars2.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/lines.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20d_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20d/lines.c ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/lines.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20d_APIO} ${23_day_harib20d_LIBGC} ${23_day_harib20d_RANDO}
  COMMENT "Generating lines.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20d/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20d/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20d/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20d/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20d/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20d/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20d"
)
