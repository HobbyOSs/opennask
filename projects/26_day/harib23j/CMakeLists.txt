#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23j/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23j_OS    ${root_BINARY_DIR}/projects/26_day/harib23j/os.img)
set(26_day_harib23j_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23j/os.sys)
set(26_day_harib23j_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23j/ipl.bin)
set(26_day_harib23j_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23j/ipl10.nas)
set(26_day_harib23j_HEADB ${root_BINARY_DIR}/projects/26_day/harib23j/asmhead.bin)
set(26_day_harib23j_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23j/asmhead.nas)
set(26_day_harib23j_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23j/boot.bin)
set(26_day_harib23j_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23j/boot.o)
file(GLOB 26_day_harib23j_CCS ${root_SOURCE_DIR}/projects/26_day/harib23j/*.c)
list(REMOVE_ITEM 26_day_harib23j_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23j/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23j/color2.c)
set(26_day_harib23j_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23j_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23j_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23j/naskfunc.nas)
set(26_day_harib23j_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23j/naskfunc.o)
set(26_day_harib23j_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23j/hankaku.txt)
set(26_day_harib23j_FONTB ${root_BINARY_DIR}/projects/26_day/harib23j/hankaku.bin)
set(26_day_harib23j_FONTO ${root_BINARY_DIR}/projects/26_day/harib23j/hankaku.o)
set(26_day_harib23j_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23j_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23j/libgo.o)
set(26_day_harib23j_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23j/*.o)


add_custom_target(26_day_harib23j_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23j_OS}
  DEPENDS 26_day_harib23j_img
)
add_custom_target(26_day_harib23j_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23j_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23j_img
)
add_custom_target(26_day_harib23j_clean
  COMMAND rm -f ${26_day_harib23j_OS}
  COMMAND rm -f ${26_day_harib23j_SYS}
  COMMAND rm -f ${26_day_harib23j_IPLB}
  COMMAND rm -f ${26_day_harib23j_HEADB}
  COMMAND rm -f ${26_day_harib23j_BOOTB}
  COMMAND rm -f ${26_day_harib23j_WILDOBJ}
)
add_custom_target(26_day_harib23j_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23j_IPLS} ${26_day_harib23j_IPLB}
  DEPENDS ${26_day_harib23j_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23j_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23j_HEADS} ${26_day_harib23j_HEADB}
  DEPENDS ${26_day_harib23j_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${26_day_harib23j_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23j_FUNCS} ${26_day_harib23j_FUNCO}
  DEPENDS ${26_day_harib23j_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${26_day_harib23j_FONTO}
  COMMAND ${FONT} ${26_day_harib23j_FONTS} ${26_day_harib23j_FONTB}
  COMMAND ${B2O}  ${26_day_harib23j_FONTB} ${26_day_harib23j_FONTO} _hankaku
  DEPENDS ${26_day_harib23j_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${26_day_harib23j_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23j_LIBGE} ${26_day_harib23j_LIBGC}
  DEPENDS ${26_day_harib23j_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23j"
)

add_custom_target(26_day_harib23j_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_LDS} ${26_day_harib23j_CCS} ${26_day_harib23j_WILDOBJ} -o ${26_day_harib23j_BOOTB}
  COMMAND cat ${26_day_harib23j_HEADB} ${26_day_harib23j_BOOTB} > ${26_day_harib23j_SYS}
  DEPENDS ${26_day_harib23j_HEADB} 26_day_harib23j_ipl
  DEPENDS ${26_day_harib23j_FUNCO}
  DEPENDS ${26_day_harib23j_FONTO} ${26_day_harib23j_LIBGC}
)
add_custom_target(26_day_harib23j_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23j_IPLB} -i ${26_day_harib23j_OS}
  COMMAND mcopy -i ${26_day_harib23j_OS} ${26_day_harib23j_SYS} ::
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23j_OS} ${root_BINARY_DIR}/projects/26_day/harib23j/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23j_sys ${root_BINARY_DIR}/projects/26_day/harib23j/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23j/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23j_APIO ${root_BINARY_DIR}/projects/26_day/harib23j/a_nask.o)
set(26_day_harib23j_RANDO ${root_BINARY_DIR}/projects/26_day/harib23j/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23j_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23j_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${26_day_harib23j_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23j/a_nask.nas ${26_day_harib23j_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/a.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/hello3.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/hello4.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo2.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo3.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/star1.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/stars.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/stars2.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/lines.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/walk.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/color2.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/color.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/beepdown.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23j_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23j/noodle.c ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23j/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23j_APIO} ${26_day_harib23j_LIBGC} ${26_day_harib23j_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23j/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23j/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23j/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23j/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23j"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23j/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23j/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23j/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23j/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23j"
)
