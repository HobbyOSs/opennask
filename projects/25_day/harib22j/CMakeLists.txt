#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22j/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22j_OS    ${root_BINARY_DIR}/projects/25_day/harib22j/os.img)
set(25_day_harib22j_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22j/os.sys)
set(25_day_harib22j_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22j/ipl.bin)
set(25_day_harib22j_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22j/ipl10.nas)
set(25_day_harib22j_HEADB ${root_BINARY_DIR}/projects/25_day/harib22j/asmhead.bin)
set(25_day_harib22j_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22j/asmhead.nas)
set(25_day_harib22j_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22j/boot.bin)
set(25_day_harib22j_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22j/boot.o)
file(GLOB 25_day_harib22j_CCS ${root_SOURCE_DIR}/projects/25_day/harib22j/*.c)
list(REMOVE_ITEM 25_day_harib22j_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22j/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22j/color2.c)
set(25_day_harib22j_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22j_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22j_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22j/naskfunc.nas)
set(25_day_harib22j_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22j/naskfunc.o)
set(25_day_harib22j_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22j/hankaku.txt)
set(25_day_harib22j_FONTB ${root_BINARY_DIR}/projects/25_day/harib22j/hankaku.bin)
set(25_day_harib22j_FONTO ${root_BINARY_DIR}/projects/25_day/harib22j/hankaku.o)
set(25_day_harib22j_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22j_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22j/libgo.o)
set(25_day_harib22j_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22j/*.o)


add_custom_target(25_day_harib22j_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22j_OS}
  DEPENDS 25_day_harib22j_img
)
add_custom_target(25_day_harib22j_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22j_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22j_img
)
add_custom_target(25_day_harib22j_clean
  COMMAND rm -f ${25_day_harib22j_OS}
  COMMAND rm -f ${25_day_harib22j_SYS}
  COMMAND rm -f ${25_day_harib22j_IPLB}
  COMMAND rm -f ${25_day_harib22j_HEADB}
  COMMAND rm -f ${25_day_harib22j_BOOTB}
  COMMAND rm -f ${25_day_harib22j_WILDOBJ}
)
add_custom_target(25_day_harib22j_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22j_IPLS} ${25_day_harib22j_IPLB}
  DEPENDS ${25_day_harib22j_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22j_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22j_HEADS} ${25_day_harib22j_HEADB}
  DEPENDS ${25_day_harib22j_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${25_day_harib22j_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22j_FUNCS} ${25_day_harib22j_FUNCO}
  DEPENDS ${25_day_harib22j_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${25_day_harib22j_FONTO}
  COMMAND ${FONT} ${25_day_harib22j_FONTS} ${25_day_harib22j_FONTB}
  COMMAND ${B2O}  ${25_day_harib22j_FONTB} ${25_day_harib22j_FONTO} _hankaku
  DEPENDS ${25_day_harib22j_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${25_day_harib22j_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22j_LIBGE} ${25_day_harib22j_LIBGC}
  DEPENDS ${25_day_harib22j_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22j"
)

add_custom_target(25_day_harib22j_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_LDS} ${25_day_harib22j_CCS} ${25_day_harib22j_WILDOBJ} -o ${25_day_harib22j_BOOTB}
  COMMAND cat ${25_day_harib22j_HEADB} ${25_day_harib22j_BOOTB} > ${25_day_harib22j_SYS}
  DEPENDS ${25_day_harib22j_HEADB} 25_day_harib22j_ipl
  DEPENDS ${25_day_harib22j_FUNCO}
  DEPENDS ${25_day_harib22j_FONTO} ${25_day_harib22j_LIBGC}
)
add_custom_target(25_day_harib22j_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22j_IPLB} -i ${25_day_harib22j_OS}
  COMMAND mcopy -i ${25_day_harib22j_OS} ${25_day_harib22j_SYS} ::
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22j_OS} ${root_BINARY_DIR}/projects/25_day/harib22j/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22j_sys ${root_BINARY_DIR}/projects/25_day/harib22j/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22j/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22j_APIO ${root_BINARY_DIR}/projects/25_day/harib22j/a_nask.o)
set(25_day_harib22j_RANDO ${root_BINARY_DIR}/projects/25_day/harib22j/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22j_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22j_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${25_day_harib22j_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22j/a_nask.nas ${25_day_harib22j_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/a.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/hello3.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/hello4.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo2.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo3.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/star1.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/stars.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/stars2.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/lines.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/walk.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/color2.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/color.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/beepdown.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22j_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22j/noodle.c ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22j/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22j_APIO} ${25_day_harib22j_LIBGC} ${25_day_harib22j_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22j/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22j/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22j/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22j/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22j/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22j/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22j/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22j/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22j"
)
