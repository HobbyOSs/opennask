#----------------------------------------------------------
message(STATUS "Entering directory projects/24_day/harib21d/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(24_day_harib21d_OS    ${root_BINARY_DIR}/projects/24_day/harib21d/os.img)
set(24_day_harib21d_SYS	  ${root_BINARY_DIR}/projects/24_day/harib21d/os.sys)
set(24_day_harib21d_IPLB  ${root_BINARY_DIR}/projects/24_day/harib21d/ipl.bin)
set(24_day_harib21d_IPLS  ${root_SOURCE_DIR}/projects/24_day/harib21d/ipl10.nas)
set(24_day_harib21d_HEADB ${root_BINARY_DIR}/projects/24_day/harib21d/asmhead.bin)
set(24_day_harib21d_HEADS ${root_SOURCE_DIR}/projects/24_day/harib21d/asmhead.nas)
set(24_day_harib21d_BOOTB ${root_BINARY_DIR}/projects/24_day/harib21d/boot.bin)
set(24_day_harib21d_CCO	  ${root_BINARY_DIR}/projects/24_day/harib21d/boot.o)
file(GLOB 24_day_harib21d_CCS ${root_SOURCE_DIR}/projects/24_day/harib21d/*.c)
list(REMOVE_ITEM 24_day_harib21d_CCS
  ${root_SOURCE_DIR}/projects/24_day/harib21d/a.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/hello3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/hello4.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo3.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/star1.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/stars.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/stars2.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/lines.c
  ${root_SOURCE_DIR}/projects/24_day/harib21d/walk.c)
set(24_day_harib21d_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(24_day_harib21d_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(24_day_harib21d_FUNCS ${root_SOURCE_DIR}/projects/24_day/harib21d/naskfunc.nas)
set(24_day_harib21d_FUNCO ${root_BINARY_DIR}/projects/24_day/harib21d/naskfunc.o)
set(24_day_harib21d_FONTS ${root_SOURCE_DIR}/projects/24_day/harib21d/hankaku.txt)
set(24_day_harib21d_FONTB ${root_BINARY_DIR}/projects/24_day/harib21d/hankaku.bin)
set(24_day_harib21d_FONTO ${root_BINARY_DIR}/projects/24_day/harib21d/hankaku.o)
set(24_day_harib21d_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(24_day_harib21d_LIBGC ${root_BINARY_DIR}/projects/24_day/harib21d/libgo.o)
set(24_day_harib21d_WILDOBJ ${root_BINARY_DIR}/projects/24_day/harib21d/*.o)


add_custom_target(24_day_harib21d_run
  COMMAND ${QEMU} ${QEMUOPT} ${24_day_harib21d_OS}
  DEPENDS 24_day_harib21d_img
)
add_custom_target(24_day_harib21d_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${24_day_harib21d_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 24_day_harib21d_img
)
add_custom_target(24_day_harib21d_clean
  COMMAND rm -f ${24_day_harib21d_OS}
  COMMAND rm -f ${24_day_harib21d_SYS}
  COMMAND rm -f ${24_day_harib21d_IPLB}
  COMMAND rm -f ${24_day_harib21d_HEADB}
  COMMAND rm -f ${24_day_harib21d_BOOTB}
  COMMAND rm -f ${24_day_harib21d_WILDOBJ}
)
add_custom_target(24_day_harib21d_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21d_IPLS} ${24_day_harib21d_IPLB}
  DEPENDS ${24_day_harib21d_IPLS}
)
add_custom_command(
  OUTPUT ${24_day_harib21d_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21d_HEADS} ${24_day_harib21d_HEADB}
  DEPENDS ${24_day_harib21d_HEADS}
  COMMENT "Generating asmhead.bin for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${24_day_harib21d_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${24_day_harib21d_FUNCS} ${24_day_harib21d_FUNCO}
  DEPENDS ${24_day_harib21d_FUNCS}
  COMMENT "Generating naskfunc.o for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${24_day_harib21d_FONTO}
  COMMAND ${FONT} ${24_day_harib21d_FONTS} ${24_day_harib21d_FONTB}
  COMMAND ${B2O}  ${24_day_harib21d_FONTB} ${24_day_harib21d_FONTO} _hankaku
  DEPENDS ${24_day_harib21d_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${24_day_harib21d_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${24_day_harib21d_LIBGE} ${24_day_harib21d_LIBGC}
  DEPENDS ${24_day_harib21d_LIBGE}
  COMMENT "Converting golibc for 24_day_harib21d"
)

add_custom_target(24_day_harib21d_sys
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_LDS} ${24_day_harib21d_CCS} ${24_day_harib21d_WILDOBJ} -o ${24_day_harib21d_BOOTB}
  COMMAND cat ${24_day_harib21d_HEADB} ${24_day_harib21d_BOOTB} > ${24_day_harib21d_SYS}
  DEPENDS ${24_day_harib21d_HEADB} 24_day_harib21d_ipl
  DEPENDS ${24_day_harib21d_FUNCO}
  DEPENDS ${24_day_harib21d_FONTO} ${24_day_harib21d_LIBGC}
)
add_custom_target(24_day_harib21d_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${24_day_harib21d_IPLB} -i ${24_day_harib21d_OS}
  COMMAND mcopy -i ${24_day_harib21d_OS} ${24_day_harib21d_SYS} ::
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/a.hrb ::a.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${24_day_harib21d_OS} ${root_BINARY_DIR}/projects/24_day/harib21d/hello5.hrb ::hello5.hrb
  DEPENDS 24_day_harib21d_sys ${root_BINARY_DIR}/projects/24_day/harib21d/a.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/hello3.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/hello4.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo2.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo3.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/star1.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/stars.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/stars2.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/lines.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/walk.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/hello.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/hello2.hrb ${root_BINARY_DIR}/projects/24_day/harib21d/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(24_day_harib21d_APIO ${root_BINARY_DIR}/projects/24_day/harib21d/a_nask.o)
set(24_day_harib21d_RANDO ${root_BINARY_DIR}/projects/24_day/harib21d/rand.o)
add_custom_command(
  OUTPUT ${24_day_harib21d_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${24_day_harib21d_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${24_day_harib21d_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21d/a_nask.nas ${24_day_harib21d_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/a_nask.nas
  COMMENT "Generating a_nask.o for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/a.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/a.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/a.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating a.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/hello3.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating hello3.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/hello4.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating hello4.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating winhelo.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo2.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating winhelo2.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo3.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating winhelo3.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/star1.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/star1.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/star1.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating star1.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/stars.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/stars.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/stars.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating stars.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/stars2.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/stars2.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating stars2.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/lines.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/lines.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/lines.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating lines.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/walk.hrb
  COMMAND gcc ${BINOPT} -T ${24_day_harib21d_APPLDS} ${root_SOURCE_DIR}/projects/24_day/harib21d/walk.c ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO} -o ${root_BINARY_DIR}/projects/24_day/harib21d/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/walk.c ${root_SOURCE_DIR}/projects/app.lds ${24_day_harib21d_APIO} ${24_day_harib21d_LIBGC} ${24_day_harib21d_RANDO}
  COMMENT "Generating walk.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21d/hello.nas ${root_BINARY_DIR}/projects/24_day/harib21d/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/hello.nas
  COMMENT "Generating hello.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21d/hello2.nas ${root_BINARY_DIR}/projects/24_day/harib21d/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/hello2.nas
  COMMENT "Generating hello2.hrb for 24_day_harib21d"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/24_day/harib21d/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/24_day/harib21d/hello5.nas ${root_BINARY_DIR}/projects/24_day/harib21d/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/24_day/harib21d/hello5.nas
  COMMENT "Generating hello5.hrb for 24_day_harib21d"
)
