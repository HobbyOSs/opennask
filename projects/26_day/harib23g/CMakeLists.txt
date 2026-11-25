#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23g_OS    ${root_BINARY_DIR}/projects/26_day/harib23g/os.img)
set(26_day_harib23g_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23g/os.sys)
set(26_day_harib23g_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23g/ipl.bin)
set(26_day_harib23g_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23g/ipl10.nas)
set(26_day_harib23g_HEADB ${root_BINARY_DIR}/projects/26_day/harib23g/asmhead.bin)
set(26_day_harib23g_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23g/asmhead.nas)
set(26_day_harib23g_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23g/boot.bin)
set(26_day_harib23g_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23g/boot.o)
file(GLOB 26_day_harib23g_CCS ${root_SOURCE_DIR}/projects/26_day/harib23g/*.c)
list(REMOVE_ITEM 26_day_harib23g_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23g/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23g/color2.c)
set(26_day_harib23g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23g_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23g_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23g/naskfunc.nas)
set(26_day_harib23g_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23g/naskfunc.o)
set(26_day_harib23g_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23g/hankaku.txt)
set(26_day_harib23g_FONTB ${root_BINARY_DIR}/projects/26_day/harib23g/hankaku.bin)
set(26_day_harib23g_FONTO ${root_BINARY_DIR}/projects/26_day/harib23g/hankaku.o)
set(26_day_harib23g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23g_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23g/libgo.o)
set(26_day_harib23g_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23g/*.o)


add_custom_target(26_day_harib23g_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23g_OS}
  DEPENDS 26_day_harib23g_img
)
add_custom_target(26_day_harib23g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23g_img
)
add_custom_target(26_day_harib23g_clean
  COMMAND rm -f ${26_day_harib23g_OS}
  COMMAND rm -f ${26_day_harib23g_SYS}
  COMMAND rm -f ${26_day_harib23g_IPLB}
  COMMAND rm -f ${26_day_harib23g_HEADB}
  COMMAND rm -f ${26_day_harib23g_BOOTB}
  COMMAND rm -f ${26_day_harib23g_WILDOBJ}
)
add_custom_target(26_day_harib23g_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23g_IPLS} ${26_day_harib23g_IPLB}
  DEPENDS ${26_day_harib23g_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23g_HEADS} ${26_day_harib23g_HEADB}
  DEPENDS ${26_day_harib23g_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${26_day_harib23g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23g_FUNCS} ${26_day_harib23g_FUNCO}
  DEPENDS ${26_day_harib23g_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${26_day_harib23g_FONTO}
  COMMAND ${FONT} ${26_day_harib23g_FONTS} ${26_day_harib23g_FONTB}
  COMMAND ${B2O}  ${26_day_harib23g_FONTB} ${26_day_harib23g_FONTO} _hankaku
  DEPENDS ${26_day_harib23g_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${26_day_harib23g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23g_LIBGE} ${26_day_harib23g_LIBGC}
  DEPENDS ${26_day_harib23g_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23g"
)

add_custom_target(26_day_harib23g_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_LDS} ${26_day_harib23g_CCS} ${26_day_harib23g_WILDOBJ} -o ${26_day_harib23g_BOOTB}
  COMMAND cat ${26_day_harib23g_HEADB} ${26_day_harib23g_BOOTB} > ${26_day_harib23g_SYS}
  DEPENDS ${26_day_harib23g_HEADB} 26_day_harib23g_ipl
  DEPENDS ${26_day_harib23g_FUNCO}
  DEPENDS ${26_day_harib23g_FONTO} ${26_day_harib23g_LIBGC}
)
add_custom_target(26_day_harib23g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23g_IPLB} -i ${26_day_harib23g_OS}
  COMMAND mcopy -i ${26_day_harib23g_OS} ${26_day_harib23g_SYS} ::
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23g_OS} ${root_BINARY_DIR}/projects/26_day/harib23g/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23g_sys ${root_BINARY_DIR}/projects/26_day/harib23g/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23g/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23g_APIO ${root_BINARY_DIR}/projects/26_day/harib23g/a_nask.o)
set(26_day_harib23g_RANDO ${root_BINARY_DIR}/projects/26_day/harib23g/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23g_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23g_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${26_day_harib23g_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23g/a_nask.nas ${26_day_harib23g_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/a.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/hello3.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/hello4.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo2.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo3.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/star1.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/stars.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/stars2.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/lines.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/walk.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/color2.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/color.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/beepdown.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23g_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23g/noodle.c ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23g/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23g_APIO} ${26_day_harib23g_LIBGC} ${26_day_harib23g_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23g/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23g/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23g/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23g/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23g/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23g/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23g/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23g/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23g"
)
