#----------------------------------------------------------
message(STATUS "Entering directory projects/26_day/harib23i/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(26_day_harib23i_OS    ${root_BINARY_DIR}/projects/26_day/harib23i/os.img)
set(26_day_harib23i_SYS	  ${root_BINARY_DIR}/projects/26_day/harib23i/os.sys)
set(26_day_harib23i_IPLB  ${root_BINARY_DIR}/projects/26_day/harib23i/ipl.bin)
set(26_day_harib23i_IPLS  ${root_SOURCE_DIR}/projects/26_day/harib23i/ipl10.nas)
set(26_day_harib23i_HEADB ${root_BINARY_DIR}/projects/26_day/harib23i/asmhead.bin)
set(26_day_harib23i_HEADS ${root_SOURCE_DIR}/projects/26_day/harib23i/asmhead.nas)
set(26_day_harib23i_BOOTB ${root_BINARY_DIR}/projects/26_day/harib23i/boot.bin)
set(26_day_harib23i_CCO	  ${root_BINARY_DIR}/projects/26_day/harib23i/boot.o)
file(GLOB 26_day_harib23i_CCS ${root_SOURCE_DIR}/projects/26_day/harib23i/*.c)
list(REMOVE_ITEM 26_day_harib23i_CCS
  ${root_SOURCE_DIR}/projects/26_day/harib23i/a.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/hello3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/hello4.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo3.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/star1.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/stars.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/stars2.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/lines.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/walk.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/noodle.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/beepdown.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/color.c
  ${root_SOURCE_DIR}/projects/26_day/harib23i/color2.c)
set(26_day_harib23i_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(26_day_harib23i_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(26_day_harib23i_FUNCS ${root_SOURCE_DIR}/projects/26_day/harib23i/naskfunc.nas)
set(26_day_harib23i_FUNCO ${root_BINARY_DIR}/projects/26_day/harib23i/naskfunc.o)
set(26_day_harib23i_FONTS ${root_SOURCE_DIR}/projects/26_day/harib23i/hankaku.txt)
set(26_day_harib23i_FONTB ${root_BINARY_DIR}/projects/26_day/harib23i/hankaku.bin)
set(26_day_harib23i_FONTO ${root_BINARY_DIR}/projects/26_day/harib23i/hankaku.o)
set(26_day_harib23i_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(26_day_harib23i_LIBGC ${root_BINARY_DIR}/projects/26_day/harib23i/libgo.o)
set(26_day_harib23i_WILDOBJ ${root_BINARY_DIR}/projects/26_day/harib23i/*.o)


add_custom_target(26_day_harib23i_run
  COMMAND ${QEMU} ${QEMUOPT} ${26_day_harib23i_OS}
  DEPENDS 26_day_harib23i_img
)
add_custom_target(26_day_harib23i_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${26_day_harib23i_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 26_day_harib23i_img
)
add_custom_target(26_day_harib23i_clean
  COMMAND rm -f ${26_day_harib23i_OS}
  COMMAND rm -f ${26_day_harib23i_SYS}
  COMMAND rm -f ${26_day_harib23i_IPLB}
  COMMAND rm -f ${26_day_harib23i_HEADB}
  COMMAND rm -f ${26_day_harib23i_BOOTB}
  COMMAND rm -f ${26_day_harib23i_WILDOBJ}
)
add_custom_target(26_day_harib23i_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23i_IPLS} ${26_day_harib23i_IPLB}
  DEPENDS ${26_day_harib23i_IPLS}
)
add_custom_command(
  OUTPUT ${26_day_harib23i_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23i_HEADS} ${26_day_harib23i_HEADB}
  DEPENDS ${26_day_harib23i_HEADS}
  COMMENT "Generating asmhead.bin for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${26_day_harib23i_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${26_day_harib23i_FUNCS} ${26_day_harib23i_FUNCO}
  DEPENDS ${26_day_harib23i_FUNCS}
  COMMENT "Generating naskfunc.o for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${26_day_harib23i_FONTO}
  COMMAND ${FONT} ${26_day_harib23i_FONTS} ${26_day_harib23i_FONTB}
  COMMAND ${B2O}  ${26_day_harib23i_FONTB} ${26_day_harib23i_FONTO} _hankaku
  DEPENDS ${26_day_harib23i_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${26_day_harib23i_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${26_day_harib23i_LIBGE} ${26_day_harib23i_LIBGC}
  DEPENDS ${26_day_harib23i_LIBGE}
  COMMENT "Converting golibc for 26_day_harib23i"
)

add_custom_target(26_day_harib23i_sys
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_LDS} ${26_day_harib23i_CCS} ${26_day_harib23i_WILDOBJ} -o ${26_day_harib23i_BOOTB}
  COMMAND cat ${26_day_harib23i_HEADB} ${26_day_harib23i_BOOTB} > ${26_day_harib23i_SYS}
  DEPENDS ${26_day_harib23i_HEADB} 26_day_harib23i_ipl
  DEPENDS ${26_day_harib23i_FUNCO}
  DEPENDS ${26_day_harib23i_FONTO} ${26_day_harib23i_LIBGC}
)
add_custom_target(26_day_harib23i_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${26_day_harib23i_IPLB} -i ${26_day_harib23i_OS}
  COMMAND mcopy -i ${26_day_harib23i_OS} ${26_day_harib23i_SYS} ::
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/a.hrb ::a.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/color.hrb ::color.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${26_day_harib23i_OS} ${root_BINARY_DIR}/projects/26_day/harib23i/hello5.hrb ::hello5.hrb
  DEPENDS 26_day_harib23i_sys ${root_BINARY_DIR}/projects/26_day/harib23i/a.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/hello3.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/hello4.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo2.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo3.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/star1.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/stars.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/stars2.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/lines.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/walk.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/noodle.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/beepdown.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/color.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/color2.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/hello.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/hello2.hrb ${root_BINARY_DIR}/projects/26_day/harib23i/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(26_day_harib23i_APIO ${root_BINARY_DIR}/projects/26_day/harib23i/a_nask.o)
set(26_day_harib23i_RANDO ${root_BINARY_DIR}/projects/26_day/harib23i/rand.o)
add_custom_command(
  OUTPUT ${26_day_harib23i_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${26_day_harib23i_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${26_day_harib23i_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23i/a_nask.nas ${26_day_harib23i_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/a_nask.nas
  COMMENT "Generating a_nask.o for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/a.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/a.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/a.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating a.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/hello3.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating hello3.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/hello4.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating hello4.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating winhelo.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo2.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating winhelo2.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo3.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating winhelo3.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/star1.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/star1.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/star1.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating star1.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/stars.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/stars.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/stars.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating stars.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/stars2.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating stars2.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/lines.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/lines.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/lines.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating lines.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/walk.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/walk.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/walk.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating walk.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/color2.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/color2.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/color2.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating color2.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/color.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/color.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/color.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating color.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/beepdown.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/beepdown.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating beepdown.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/noodle.hrb
  COMMAND gcc ${BINOPT} -T ${26_day_harib23i_APPLDS} ${root_SOURCE_DIR}/projects/26_day/harib23i/noodle.c ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO} -o ${root_BINARY_DIR}/projects/26_day/harib23i/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${26_day_harib23i_APIO} ${26_day_harib23i_LIBGC} ${26_day_harib23i_RANDO}
  COMMENT "Generating noodle.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23i/hello.nas ${root_BINARY_DIR}/projects/26_day/harib23i/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/hello.nas
  COMMENT "Generating hello.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23i/hello2.nas ${root_BINARY_DIR}/projects/26_day/harib23i/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/hello2.nas
  COMMENT "Generating hello2.hrb for 26_day_harib23i"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/26_day/harib23i/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/26_day/harib23i/hello5.nas ${root_BINARY_DIR}/projects/26_day/harib23i/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/26_day/harib23i/hello5.nas
  COMMENT "Generating hello5.hrb for 26_day_harib23i"
)
