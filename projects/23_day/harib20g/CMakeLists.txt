#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20g_OS    ${root_BINARY_DIR}/projects/23_day/harib20g/os.img)
set(23_day_harib20g_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20g/os.sys)
set(23_day_harib20g_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20g/ipl.bin)
set(23_day_harib20g_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20g/ipl10.nas)
set(23_day_harib20g_HEADB ${root_BINARY_DIR}/projects/23_day/harib20g/asmhead.bin)
set(23_day_harib20g_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20g/asmhead.nas)
set(23_day_harib20g_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20g/boot.bin)
set(23_day_harib20g_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20g/boot.o)
file(GLOB 23_day_harib20g_CCS ${root_SOURCE_DIR}/projects/23_day/harib20g/*.c)
list(REMOVE_ITEM 23_day_harib20g_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20g/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/star1.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/stars.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/stars2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/lines.c
  ${root_SOURCE_DIR}/projects/23_day/harib20g/walk.c)
set(23_day_harib20g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20g_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20g_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20g/naskfunc.nas)
set(23_day_harib20g_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20g/naskfunc.o)
set(23_day_harib20g_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20g/hankaku.txt)
set(23_day_harib20g_FONTB ${root_BINARY_DIR}/projects/23_day/harib20g/hankaku.bin)
set(23_day_harib20g_FONTO ${root_BINARY_DIR}/projects/23_day/harib20g/hankaku.o)
set(23_day_harib20g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20g_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20g/libgo.o)
set(23_day_harib20g_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20g/*.o)


add_custom_target(23_day_harib20g_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20g_OS}
  DEPENDS 23_day_harib20g_img
)
add_custom_target(23_day_harib20g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20g_img
)
add_custom_target(23_day_harib20g_clean
  COMMAND rm -f ${23_day_harib20g_OS}
  COMMAND rm -f ${23_day_harib20g_SYS}
  COMMAND rm -f ${23_day_harib20g_IPLB}
  COMMAND rm -f ${23_day_harib20g_HEADB}
  COMMAND rm -f ${23_day_harib20g_BOOTB}
  COMMAND rm -f ${23_day_harib20g_WILDOBJ}
)
add_custom_target(23_day_harib20g_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20g_IPLS} ${23_day_harib20g_IPLB}
  DEPENDS ${23_day_harib20g_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20g_HEADS} ${23_day_harib20g_HEADB}
  DEPENDS ${23_day_harib20g_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${23_day_harib20g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20g_FUNCS} ${23_day_harib20g_FUNCO}
  DEPENDS ${23_day_harib20g_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${23_day_harib20g_FONTO}
  COMMAND ${FONT} ${23_day_harib20g_FONTS} ${23_day_harib20g_FONTB}
  COMMAND ${B2O}  ${23_day_harib20g_FONTB} ${23_day_harib20g_FONTO} _hankaku
  DEPENDS ${23_day_harib20g_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${23_day_harib20g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20g_LIBGE} ${23_day_harib20g_LIBGC}
  DEPENDS ${23_day_harib20g_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20g"
)

add_custom_target(23_day_harib20g_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_LDS} ${23_day_harib20g_CCS} ${23_day_harib20g_WILDOBJ} -o ${23_day_harib20g_BOOTB}
  COMMAND cat ${23_day_harib20g_HEADB} ${23_day_harib20g_BOOTB} > ${23_day_harib20g_SYS}
  DEPENDS ${23_day_harib20g_HEADB} 23_day_harib20g_ipl
  DEPENDS ${23_day_harib20g_FUNCO}
  DEPENDS ${23_day_harib20g_FONTO} ${23_day_harib20g_LIBGC}
)
add_custom_target(23_day_harib20g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20g_IPLB} -i ${23_day_harib20g_OS}
  COMMAND mcopy -i ${23_day_harib20g_OS} ${23_day_harib20g_SYS} ::
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20g_OS} ${root_BINARY_DIR}/projects/23_day/harib20g/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20g_sys ${root_BINARY_DIR}/projects/23_day/harib20g/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/star1.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/stars.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/stars2.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/lines.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/walk.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20g/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20g_APIO ${root_BINARY_DIR}/projects/23_day/harib20g/a_nask.o)
set(23_day_harib20g_RANDO ${root_BINARY_DIR}/projects/23_day/harib20g/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20g_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20g_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${23_day_harib20g_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20g/a_nask.nas ${23_day_harib20g_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/a.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/hello3.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/hello4.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo2.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo3.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/star1.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/star1.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/star1.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating star1.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/stars.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/stars.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/stars.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating stars.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/stars2.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating stars2.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/lines.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/lines.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/lines.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating lines.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/walk.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20g_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20g/walk.c ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20g/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/walk.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20g_APIO} ${23_day_harib20g_LIBGC} ${23_day_harib20g_RANDO}
  COMMENT "Generating walk.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20g/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20g/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20g/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20g/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20g/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20g/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20g/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20g/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20g"
)
