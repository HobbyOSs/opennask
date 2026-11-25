#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22e_OS    ${root_BINARY_DIR}/projects/25_day/harib22e/os.img)
set(25_day_harib22e_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22e/os.sys)
set(25_day_harib22e_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22e/ipl.bin)
set(25_day_harib22e_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22e/ipl10.nas)
set(25_day_harib22e_HEADB ${root_BINARY_DIR}/projects/25_day/harib22e/asmhead.bin)
set(25_day_harib22e_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22e/asmhead.nas)
set(25_day_harib22e_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22e/boot.bin)
set(25_day_harib22e_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22e/boot.o)
file(GLOB 25_day_harib22e_CCS ${root_SOURCE_DIR}/projects/25_day/harib22e/*.c)
list(REMOVE_ITEM 25_day_harib22e_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22e/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22e/color2.c)
set(25_day_harib22e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22e_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22e/naskfunc.nas)
set(25_day_harib22e_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22e/naskfunc.o)
set(25_day_harib22e_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22e/hankaku.txt)
set(25_day_harib22e_FONTB ${root_BINARY_DIR}/projects/25_day/harib22e/hankaku.bin)
set(25_day_harib22e_FONTO ${root_BINARY_DIR}/projects/25_day/harib22e/hankaku.o)
set(25_day_harib22e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22e_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22e/libgo.o)
set(25_day_harib22e_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22e/*.o)


add_custom_target(25_day_harib22e_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22e_OS}
  DEPENDS 25_day_harib22e_img
)
add_custom_target(25_day_harib22e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22e_img
)
add_custom_target(25_day_harib22e_clean
  COMMAND rm -f ${25_day_harib22e_OS}
  COMMAND rm -f ${25_day_harib22e_SYS}
  COMMAND rm -f ${25_day_harib22e_IPLB}
  COMMAND rm -f ${25_day_harib22e_HEADB}
  COMMAND rm -f ${25_day_harib22e_BOOTB}
  COMMAND rm -f ${25_day_harib22e_WILDOBJ}
)
add_custom_target(25_day_harib22e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22e_IPLS} ${25_day_harib22e_IPLB}
  DEPENDS ${25_day_harib22e_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22e_HEADS} ${25_day_harib22e_HEADB}
  DEPENDS ${25_day_harib22e_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${25_day_harib22e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22e_FUNCS} ${25_day_harib22e_FUNCO}
  DEPENDS ${25_day_harib22e_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${25_day_harib22e_FONTO}
  COMMAND ${FONT} ${25_day_harib22e_FONTS} ${25_day_harib22e_FONTB}
  COMMAND ${B2O}  ${25_day_harib22e_FONTB} ${25_day_harib22e_FONTO} _hankaku
  DEPENDS ${25_day_harib22e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${25_day_harib22e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22e_LIBGE} ${25_day_harib22e_LIBGC}
  DEPENDS ${25_day_harib22e_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22e"
)

add_custom_target(25_day_harib22e_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_LDS} ${25_day_harib22e_CCS} ${25_day_harib22e_WILDOBJ} -o ${25_day_harib22e_BOOTB}
  COMMAND cat ${25_day_harib22e_HEADB} ${25_day_harib22e_BOOTB} > ${25_day_harib22e_SYS}
  DEPENDS ${25_day_harib22e_HEADB} 25_day_harib22e_ipl
  DEPENDS ${25_day_harib22e_FUNCO}
  DEPENDS ${25_day_harib22e_FONTO} ${25_day_harib22e_LIBGC}
)
add_custom_target(25_day_harib22e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22e_IPLB} -i ${25_day_harib22e_OS}
  COMMAND mcopy -i ${25_day_harib22e_OS} ${25_day_harib22e_SYS} ::
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22e_OS} ${root_BINARY_DIR}/projects/25_day/harib22e/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22e_sys ${root_BINARY_DIR}/projects/25_day/harib22e/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22e/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22e_APIO ${root_BINARY_DIR}/projects/25_day/harib22e/a_nask.o)
set(25_day_harib22e_RANDO ${root_BINARY_DIR}/projects/25_day/harib22e/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${25_day_harib22e_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22e/a_nask.nas ${25_day_harib22e_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/a.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/hello3.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/hello4.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo2.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo3.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/star1.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/stars.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/stars2.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/lines.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/walk.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/color2.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/color.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/beepdown.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22e_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22e/noodle.c ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22e/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22e_APIO} ${25_day_harib22e_LIBGC} ${25_day_harib22e_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22e/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22e/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22e/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22e/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22e/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22e/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22e"
)
