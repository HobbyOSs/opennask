#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22c_OS    ${root_BINARY_DIR}/projects/25_day/harib22c/os.img)
set(25_day_harib22c_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22c/os.sys)
set(25_day_harib22c_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22c/ipl.bin)
set(25_day_harib22c_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22c/ipl10.nas)
set(25_day_harib22c_HEADB ${root_BINARY_DIR}/projects/25_day/harib22c/asmhead.bin)
set(25_day_harib22c_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22c/asmhead.nas)
set(25_day_harib22c_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22c/boot.bin)
set(25_day_harib22c_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22c/boot.o)
file(GLOB 25_day_harib22c_CCS ${root_SOURCE_DIR}/projects/25_day/harib22c/*.c)
list(REMOVE_ITEM 25_day_harib22c_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22c/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22c/color2.c)
set(25_day_harib22c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22c_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22c/naskfunc.nas)
set(25_day_harib22c_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22c/naskfunc.o)
set(25_day_harib22c_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22c/hankaku.txt)
set(25_day_harib22c_FONTB ${root_BINARY_DIR}/projects/25_day/harib22c/hankaku.bin)
set(25_day_harib22c_FONTO ${root_BINARY_DIR}/projects/25_day/harib22c/hankaku.o)
set(25_day_harib22c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22c_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22c/libgo.o)
set(25_day_harib22c_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22c/*.o)


add_custom_target(25_day_harib22c_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22c_OS}
  DEPENDS 25_day_harib22c_img
)
add_custom_target(25_day_harib22c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22c_img
)
add_custom_target(25_day_harib22c_clean
  COMMAND rm -f ${25_day_harib22c_OS}
  COMMAND rm -f ${25_day_harib22c_SYS}
  COMMAND rm -f ${25_day_harib22c_IPLB}
  COMMAND rm -f ${25_day_harib22c_HEADB}
  COMMAND rm -f ${25_day_harib22c_BOOTB}
  COMMAND rm -f ${25_day_harib22c_WILDOBJ}
)
add_custom_target(25_day_harib22c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22c_IPLS} ${25_day_harib22c_IPLB}
  DEPENDS ${25_day_harib22c_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22c_HEADS} ${25_day_harib22c_HEADB}
  DEPENDS ${25_day_harib22c_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${25_day_harib22c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22c_FUNCS} ${25_day_harib22c_FUNCO}
  DEPENDS ${25_day_harib22c_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${25_day_harib22c_FONTO}
  COMMAND ${FONT} ${25_day_harib22c_FONTS} ${25_day_harib22c_FONTB}
  COMMAND ${B2O}  ${25_day_harib22c_FONTB} ${25_day_harib22c_FONTO} _hankaku
  DEPENDS ${25_day_harib22c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${25_day_harib22c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22c_LIBGE} ${25_day_harib22c_LIBGC}
  DEPENDS ${25_day_harib22c_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22c"
)

add_custom_target(25_day_harib22c_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_LDS} ${25_day_harib22c_CCS} ${25_day_harib22c_WILDOBJ} -o ${25_day_harib22c_BOOTB}
  COMMAND cat ${25_day_harib22c_HEADB} ${25_day_harib22c_BOOTB} > ${25_day_harib22c_SYS}
  DEPENDS ${25_day_harib22c_HEADB} 25_day_harib22c_ipl
  DEPENDS ${25_day_harib22c_FUNCO}
  DEPENDS ${25_day_harib22c_FONTO} ${25_day_harib22c_LIBGC}
)
add_custom_target(25_day_harib22c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22c_IPLB} -i ${25_day_harib22c_OS}
  COMMAND mcopy -i ${25_day_harib22c_OS} ${25_day_harib22c_SYS} ::
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22c_OS} ${root_BINARY_DIR}/projects/25_day/harib22c/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22c_sys ${root_BINARY_DIR}/projects/25_day/harib22c/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22c/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22c_APIO ${root_BINARY_DIR}/projects/25_day/harib22c/a_nask.o)
set(25_day_harib22c_RANDO ${root_BINARY_DIR}/projects/25_day/harib22c/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${25_day_harib22c_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22c/a_nask.nas ${25_day_harib22c_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/a.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/hello3.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/hello4.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo2.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo3.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/star1.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/stars.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/stars2.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/lines.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/walk.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/color2.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/color.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/beepdown.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22c_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22c/noodle.c ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22c/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22c_APIO} ${25_day_harib22c_LIBGC} ${25_day_harib22c_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22c/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22c/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22c/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22c/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22c/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22c/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22c"
)
