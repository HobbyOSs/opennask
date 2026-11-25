#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21e/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21e_OS    ${root_BINARY_DIR}/projects/24_day/harib21e/os.img)
set(24_day_harib21e_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21e/os.sys)
set(24_day_harib21e_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21e/ipl.bin)
set(24_day_harib21e_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21e/ipl10.nas)
set(24_day_harib21e_HEADB ${root_BINARY_DIR}/projects/24_day/harib21e/asmhead.bin)
set(24_day_harib21e_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21e/asmhead.nas)
set(24_day_harib21e_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21e/boot.bin)
set(24_day_harib21e_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21e/boot.o)
file(GLOB 24_day_harib21e_CCS ${root_SOURCE_DIR}/projects/24_day/harib21e/*.c)
list(REMOVE_ITEM 24_day_harib21e_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21e/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21e/walk.c)
set(24_day_harib21e_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21e_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21e_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21e/naskfunc.nas)
set(24_day_harib21e_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21e/naskfunc.o)
set(24_day_harib21e_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21e/hankaku.txt)
set(24_day_harib21e_FONTB ${root_BINARY_DIR}/projects/24_day/harib21e/hankaku.bin)
set(24_day_harib21e_FONTO ${root_BINARY_DIR}/projects/24_day/harib21e/hankaku.o)
set(24_day_harib21e_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21e_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21e/libgo.o)
set(24_day_harib21e_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21e/*.o)


add_custom_target(24_day_harib21e_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21e_OS}
  DEPENDS 24_day_harib21e_img
)
add_custom_target(24_day_harib21e_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21e_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21e_img
)
add_custom_target(24_day_harib21e_clean
  COMMAND rm -f ${24_day_harib21e_OS}
  COMMAND rm -f ${24_day_harib21e_SYS}
  COMMAND rm -f ${24_day_harib21e_IPLB}
  COMMAND rm -f ${24_day_harib21e_HEADB}
  COMMAND rm -f ${24_day_harib21e_BOOTB}
  COMMAND rm -f ${24_day_harib21e_WILDOBJ}
)
add_custom_target(24_day_harib21e_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21e_IPLS} ${24_day_harib21e_IPLB}
  DEPENDS ${24_day_harib21e_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21e_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21e_HEADS} ${24_day_harib21e_HEADB}
  DEPENDS ${24_day_harib21e_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${24_day_harib21e_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21e_FUNCS} ${24_day_harib21e_FUNCO}
  DEPENDS ${24_day_harib21e_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${24_day_harib21e_FONTO}
  COMMAND ${FONT} ${24_day_harib21e_FONTS} ${24_day_harib21e_FONTB}
  COMMAND ${B2O}  ${24_day_harib21e_FONTB} ${24_day_harib21e_FONTO} _hankaku
  DEPENDS ${24_day_harib21e_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${24_day_harib21e_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21e_LIBGE} ${24_day_harib21e_LIBGC}
  DEPENDS ${24_day_harib21e_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21e"
)

add_custom_target(24_day_harib21e_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_LDS} ${24_day_harib21e_CCS} ${24_day_harib21e_WILDOBJ} -o ${24_day_harib21e_BOOTB}
  COMMAND cat ${24_day_harib21e_HEADB} ${24_day_harib21e_BOOTB} > ${24_day_harib21e_SYS}
  DEPENDS ${24_day_harib21e_HEADB} 24_day_harib21e_ipl
  DEPENDS ${24_day_harib21e_FUNCO}
  DEPENDS ${24_day_harib21e_FONTO} ${24_day_harib21e_LIBGC}
)
add_custom_target(24_day_harib21e_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21e_IPLB} -i ${24_day_harib21e_OS}
  COMMAND mcopy -i ${24_day_harib21e_OS} ${24_day_harib21e_SYS} ::
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21e_OS} ${root_BINARY_DIR}/projects/24_day/harib21e/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21e_sys ${root_BINARY_DIR}/projects/24_day/harib21e/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21e/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21e_APIO ${root_BINARY_DIR}/projects/24_day/harib21e/a_nask.o)
set(24_day_harib21e_RANDO ${root_BINARY_DIR}/projects/24_day/harib21e/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21e_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21e_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${24_day_harib21e_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21e/a_nask.nas ${24_day_harib21e_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/a.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/hello3.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/hello4.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo2.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo3.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/star1.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/stars.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/stars2.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/lines.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21e_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21e/walk.c ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21e/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21e_APIO} ${24_day_harib21e_LIBGC} ${24_day_harib21e_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21e/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21e/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21e/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21e/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21e"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21e/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21e/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21e/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21e/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21e"
)
