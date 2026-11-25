#----------------------------------------------------------
message(STATUS "Entering directory projects/25_day/harib22g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(25_day_harib22g_OS    ${root_BINARY_DIR}/projects/25_day/harib22g/os.img)
set(25_day_harib22g_SYS	  ${root_BINARY_DIR}/projects/25_day/harib22g/os.sys)
set(25_day_harib22g_IPLB  ${root_BINARY_DIR}/projects/25_day/harib22g/ipl.bin)
set(25_day_harib22g_IPLS  ${root_SOURCE_DIR}/projects/25_day/harib22g/ipl10.nas)
set(25_day_harib22g_HEADB ${root_BINARY_DIR}/projects/25_day/harib22g/asmhead.bin)
set(25_day_harib22g_HEADS ${root_SOURCE_DIR}/projects/25_day/harib22g/asmhead.nas)
set(25_day_harib22g_BOOTB ${root_BINARY_DIR}/projects/25_day/harib22g/boot.bin)
set(25_day_harib22g_CCO	  ${root_BINARY_DIR}/projects/25_day/harib22g/boot.o)
file(GLOB 25_day_harib22g_CCS ${root_SOURCE_DIR}/projects/25_day/harib22g/*.c)
list(REMOVE_ITEM 25_day_harib22g_CCS
  ${root_SOURCE_DIR}/projects/25_day/harib22g/a.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/hello3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/hello4.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo3.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/star1.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/stars.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/stars2.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/lines.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/walk.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/noodle.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/beepdown.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/color.c
  ${root_SOURCE_DIR}/projects/25_day/harib22g/color2.c)
set(25_day_harib22g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(25_day_harib22g_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(25_day_harib22g_FUNCS ${root_SOURCE_DIR}/projects/25_day/harib22g/naskfunc.nas)
set(25_day_harib22g_FUNCO ${root_BINARY_DIR}/projects/25_day/harib22g/naskfunc.o)
set(25_day_harib22g_FONTS ${root_SOURCE_DIR}/projects/25_day/harib22g/hankaku.txt)
set(25_day_harib22g_FONTB ${root_BINARY_DIR}/projects/25_day/harib22g/hankaku.bin)
set(25_day_harib22g_FONTO ${root_BINARY_DIR}/projects/25_day/harib22g/hankaku.o)
set(25_day_harib22g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(25_day_harib22g_LIBGC ${root_BINARY_DIR}/projects/25_day/harib22g/libgo.o)
set(25_day_harib22g_WILDOBJ ${root_BINARY_DIR}/projects/25_day/harib22g/*.o)


add_custom_target(25_day_harib22g_run
  COMMAND ${QEMU} ${QEMUOPT} ${25_day_harib22g_OS}
  DEPENDS 25_day_harib22g_img
)
add_custom_target(25_day_harib22g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${25_day_harib22g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 25_day_harib22g_img
)
add_custom_target(25_day_harib22g_clean
  COMMAND rm -f ${25_day_harib22g_OS}
  COMMAND rm -f ${25_day_harib22g_SYS}
  COMMAND rm -f ${25_day_harib22g_IPLB}
  COMMAND rm -f ${25_day_harib22g_HEADB}
  COMMAND rm -f ${25_day_harib22g_BOOTB}
  COMMAND rm -f ${25_day_harib22g_WILDOBJ}
)
add_custom_target(25_day_harib22g_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22g_IPLS} ${25_day_harib22g_IPLB}
  DEPENDS ${25_day_harib22g_IPLS}
)
add_custom_command(
  OUTPUT ${25_day_harib22g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22g_HEADS} ${25_day_harib22g_HEADB}
  DEPENDS ${25_day_harib22g_HEADS}
  COMMENT "Generating asmhead.bin for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${25_day_harib22g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${25_day_harib22g_FUNCS} ${25_day_harib22g_FUNCO}
  DEPENDS ${25_day_harib22g_FUNCS}
  COMMENT "Generating naskfunc.o for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${25_day_harib22g_FONTO}
  COMMAND ${FONT} ${25_day_harib22g_FONTS} ${25_day_harib22g_FONTB}
  COMMAND ${B2O}  ${25_day_harib22g_FONTB} ${25_day_harib22g_FONTO} _hankaku
  DEPENDS ${25_day_harib22g_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${25_day_harib22g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${25_day_harib22g_LIBGE} ${25_day_harib22g_LIBGC}
  DEPENDS ${25_day_harib22g_LIBGE}
  COMMENT "Converting golibc for 25_day_harib22g"
)

add_custom_target(25_day_harib22g_sys
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_LDS} ${25_day_harib22g_CCS} ${25_day_harib22g_WILDOBJ} -o ${25_day_harib22g_BOOTB}
  COMMAND cat ${25_day_harib22g_HEADB} ${25_day_harib22g_BOOTB} > ${25_day_harib22g_SYS}
  DEPENDS ${25_day_harib22g_HEADB} 25_day_harib22g_ipl
  DEPENDS ${25_day_harib22g_FUNCO}
  DEPENDS ${25_day_harib22g_FONTO} ${25_day_harib22g_LIBGC}
)
add_custom_target(25_day_harib22g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${25_day_harib22g_IPLB} -i ${25_day_harib22g_OS}
  COMMAND mcopy -i ${25_day_harib22g_OS} ${25_day_harib22g_SYS} ::
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/a.hrb ::a.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/color.hrb ::color.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${25_day_harib22g_OS} ${root_BINARY_DIR}/projects/25_day/harib22g/hello5.hrb ::hello5.hrb
  DEPENDS 25_day_harib22g_sys ${root_BINARY_DIR}/projects/25_day/harib22g/a.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/hello3.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/hello4.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo2.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo3.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/star1.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/stars.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/stars2.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/lines.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/walk.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/noodle.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/beepdown.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/color.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/color2.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/hello.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/hello2.hrb ${root_BINARY_DIR}/projects/25_day/harib22g/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(25_day_harib22g_APIO ${root_BINARY_DIR}/projects/25_day/harib22g/a_nask.o)
set(25_day_harib22g_RANDO ${root_BINARY_DIR}/projects/25_day/harib22g/rand.o)
add_custom_command(
  OUTPUT ${25_day_harib22g_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${25_day_harib22g_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${25_day_harib22g_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22g/a_nask.nas ${25_day_harib22g_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/a_nask.nas
  COMMENT "Generating a_nask.o for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/a.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/a.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/a.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating a.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/hello3.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating hello3.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/hello4.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating hello4.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating winhelo.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo2.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating winhelo2.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo3.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating winhelo3.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/star1.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/star1.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/star1.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating star1.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/stars.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/stars.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/stars.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating stars.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/stars2.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating stars2.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/lines.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/lines.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/lines.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating lines.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/walk.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/walk.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/walk.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating walk.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/color2.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/color2.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/color2.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating color2.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/color.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/color.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/color.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating color.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/beepdown.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating beepdown.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${25_day_harib22g_APPLDS} ${root_SOURCE_DIR}/projects/25_day/harib22g/noodle.c ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO} -o ${root_BINARY_DIR}/projects/25_day/harib22g/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${25_day_harib22g_APIO} ${25_day_harib22g_LIBGC} ${25_day_harib22g_RANDO}
  COMMENT "Generating noodle.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22g/hello.nas ${root_BINARY_DIR}/projects/25_day/harib22g/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/hello.nas
  COMMENT "Generating hello.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22g/hello2.nas ${root_BINARY_DIR}/projects/25_day/harib22g/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/hello2.nas
  COMMENT "Generating hello2.hrb for 25_day_harib22g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/25_day/harib22g/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/25_day/harib22g/hello5.nas ${root_BINARY_DIR}/projects/25_day/harib22g/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/25_day/harib22g/hello5.nas
  COMMENT "Generating hello5.hrb for 25_day_harib22g"
)
