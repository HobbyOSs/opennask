#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22d_OS    ${root_BINARY_DIR}/projects/25_day/harib22d/os.img)
set(25_day_harib22d_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22d/os.sys)
set(25_day_harib22d_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22d/ipl.bin)
set(25_day_harib22d_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22d/ipl10.nas)
set(25_day_harib22d_HEADB ${root_BINARY_DIR}/projects/25_day/harib22d/asmhead.bin)
set(25_day_harib22d_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22d/asmhead.nas)
set(25_day_harib22d_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22d/boot.bin)
set(25_day_harib22d_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22d/boot.o)
file(GLOB 25_day_harib22d_CCS ${root_SOURCE_DIR}/projects/25_day/harib22d/*.c)
list(REMOVE_ITEM 25_day_harib22d_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22d/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22d/color2.c)
set(25_day_harib22d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22d_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22d/naskfunc.nas)
set(25_day_harib22d_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22d/naskfunc.o)
set(25_day_harib22d_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22d/hankaku.txt)
set(25_day_harib22d_FONTB ${root_BINARY_DIR}/projects/25_day/harib22d/hankaku.bin)
set(25_day_harib22d_FONTO ${root_BINARY_DIR}/projects/25_day/harib22d/hankaku.o)
set(25_day_harib22d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22d_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22d/libgo.o)
set(25_day_harib22d_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22d/*.o)


add_custom_target(25_day_harib22d_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22d_OS}
  DEPENDS 25_day_harib22d_img
)
add_custom_target(25_day_harib22d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22d_img
)
add_custom_target(25_day_harib22d_clean
  COMMAND rm -f ${25_day_harib22d_OS}
  COMMAND rm -f ${25_day_harib22d_SYS}
  COMMAND rm -f ${25_day_harib22d_IPLB}
  COMMAND rm -f ${25_day_harib22d_HEADB}
  COMMAND rm -f ${25_day_harib22d_BOOTB}
  COMMAND rm -f ${25_day_harib22d_WILDOBJ}
)
add_custom_target(25_day_harib22d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22d_IPLS} ${25_day_harib22d_IPLB}
  DEPENDS ${25_day_harib22d_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22d_HEADS} ${25_day_harib22d_HEADB}
  DEPENDS ${25_day_harib22d_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${25_day_harib22d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22d_FUNCS} ${25_day_harib22d_FUNCO}
  DEPENDS ${25_day_harib22d_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${25_day_harib22d_FONTO}
  COMMAND ${FONT} ${25_day_harib22d_FONTS} ${25_day_harib22d_FONTB}
  COMMAND ${B2O}  ${25_day_harib22d_FONTB} ${25_day_harib22d_FONTO} _hankaku
  DEPENDS ${25_day_harib22d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${25_day_harib22d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22d_LIBGE} ${25_day_harib22d_LIBGC}
  DEPENDS ${25_day_harib22d_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22d"
)

add_custom_target(25_day_harib22d_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_LDS} ${25_day_harib22d_CCS} ${25_day_harib22d_WILDOBJ} -o ${25_day_harib22d_BOOTB}
  COMMAND cat ${25_day_harib22d_HEADB} ${25_day_harib22d_BOOTB} > ${25_day_harib22d_SYS}
  DEPENDS ${25_day_harib22d_HEADB} 25_day_harib22d_ipl
  DEPENDS ${25_day_harib22d_FUNCO}
  DEPENDS ${25_day_harib22d_FONTO} ${25_day_harib22d_LIBGC}
)
add_custom_target(25_day_harib22d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22d_IPLB} -i ${25_day_harib22d_OS}
  COMMAND mcopy -i ${25_day_harib22d_OS} ${25_day_harib22d_SYS} ::
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22d_OS} ${root_BINARY_DIR}/projects/25_day/harib22d/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22d_sys ${root_BINARY_DIR}/projects/25_day/harib22d/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22d/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22d_APIO ${root_BINARY_DIR}/projects/25_day/harib22d/a_nask.o)
set(25_day_harib22d_RANDO ${root_BINARY_DIR}/projects/25_day/harib22d/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${25_day_harib22d_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22d/a_nask.nas ${25_day_harib22d_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/a.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/hello3.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/hello4.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo2.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo3.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/star1.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/stars.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/stars2.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/lines.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/walk.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/color2.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/color.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/beepdown.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22d_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22d/noodle.c ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22d/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22d_APIO} ${25_day_harib22d_LIBGC} ${25_day_harib22d_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22d/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22d/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22d/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22d/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22d/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22d/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22d"
)
