#----------------------------------------------------------
message(STATUS "Entering directory projects/23_day/harib20b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(23_day_harib20b_OS    ${root_BINARY_DIR}/projects/23_day/harib20b/os.img)
set(23_day_harib20b_SYS	  ${root_BINARY_DIR}/projects/23_day/harib20b/os.sys)
set(23_day_harib20b_IPLB  ${root_BINARY_DIR}/projects/23_day/harib20b/ipl.bin)
set(23_day_harib20b_IPLS  ${root_SOURCE_DIR}/projects/23_day/harib20b/ipl10.nas)
set(23_day_harib20b_HEADB ${root_BINARY_DIR}/projects/23_day/harib20b/asmhead.bin)
set(23_day_harib20b_HEADS ${root_SOURCE_DIR}/projects/23_day/harib20b/asmhead.nas)
set(23_day_harib20b_BOOTB ${root_BINARY_DIR}/projects/23_day/harib20b/boot.bin)
set(23_day_harib20b_CCO	  ${root_BINARY_DIR}/projects/23_day/harib20b/boot.o)
file(GLOB 23_day_harib20b_CCS ${root_SOURCE_DIR}/projects/23_day/harib20b/*.c)
list(REMOVE_ITEM 23_day_harib20b_CCS
  ${root_SOURCE_DIR}/projects/23_day/harib20b/a.c
  ${root_SOURCE_DIR}/projects/23_day/harib20b/hello3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20b/hello4.c
  ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo.c
  ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo2.c
  ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo3.c
  ${root_SOURCE_DIR}/projects/23_day/harib20b/star1.c
  ${root_SOURCE_DIR}/projects/23_day/harib20b/stars.c)
set(23_day_harib20b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(23_day_harib20b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(23_day_harib20b_FUNCS ${root_SOURCE_DIR}/projects/23_day/harib20b/naskfunc.nas)
set(23_day_harib20b_FUNCO ${root_BINARY_DIR}/projects/23_day/harib20b/naskfunc.o)
set(23_day_harib20b_FONTS ${root_SOURCE_DIR}/projects/23_day/harib20b/hankaku.txt)
set(23_day_harib20b_FONTB ${root_BINARY_DIR}/projects/23_day/harib20b/hankaku.bin)
set(23_day_harib20b_FONTO ${root_BINARY_DIR}/projects/23_day/harib20b/hankaku.o)
set(23_day_harib20b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(23_day_harib20b_LIBGC ${root_BINARY_DIR}/projects/23_day/harib20b/libgo.o)
set(23_day_harib20b_WILDOBJ ${root_BINARY_DIR}/projects/23_day/harib20b/*.o)


add_custom_target(23_day_harib20b_run
  COMMAND ${QEMU} ${QEMUOPT} ${23_day_harib20b_OS}
  DEPENDS 23_day_harib20b_img
)
add_custom_target(23_day_harib20b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${23_day_harib20b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 23_day_harib20b_img
)
add_custom_target(23_day_harib20b_clean
  COMMAND rm -f ${23_day_harib20b_OS}
  COMMAND rm -f ${23_day_harib20b_SYS}
  COMMAND rm -f ${23_day_harib20b_IPLB}
  COMMAND rm -f ${23_day_harib20b_HEADB}
  COMMAND rm -f ${23_day_harib20b_BOOTB}
  COMMAND rm -f ${23_day_harib20b_WILDOBJ}
)
add_custom_target(23_day_harib20b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20b_IPLS} ${23_day_harib20b_IPLB}
  DEPENDS ${23_day_harib20b_IPLS}
)
add_custom_command(
  OUTPUT ${23_day_harib20b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20b_HEADS} ${23_day_harib20b_HEADB}
  DEPENDS ${23_day_harib20b_HEADS}
  COMMENT "Generating asmhead.bin for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${23_day_harib20b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${23_day_harib20b_FUNCS} ${23_day_harib20b_FUNCO}
  DEPENDS ${23_day_harib20b_FUNCS}
  COMMENT "Generating naskfunc.o for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${23_day_harib20b_FONTO}
  COMMAND ${FONT} ${23_day_harib20b_FONTS} ${23_day_harib20b_FONTB}
  COMMAND ${B2O}  ${23_day_harib20b_FONTB} ${23_day_harib20b_FONTO} _hankaku
  DEPENDS ${23_day_harib20b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${23_day_harib20b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${23_day_harib20b_LIBGE} ${23_day_harib20b_LIBGC}
  DEPENDS ${23_day_harib20b_LIBGE}
  COMMENT "Converting golibc for 23_day_harib20b"
)

add_custom_target(23_day_harib20b_sys
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_LDS} ${23_day_harib20b_CCS} ${23_day_harib20b_WILDOBJ} -o ${23_day_harib20b_BOOTB}
  COMMAND cat ${23_day_harib20b_HEADB} ${23_day_harib20b_BOOTB} > ${23_day_harib20b_SYS}
  DEPENDS ${23_day_harib20b_HEADB} 23_day_harib20b_ipl
  DEPENDS ${23_day_harib20b_FUNCO}
  DEPENDS ${23_day_harib20b_FONTO} ${23_day_harib20b_LIBGC}
)
add_custom_target(23_day_harib20b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${23_day_harib20b_IPLB} -i ${23_day_harib20b_OS}
  COMMAND mcopy -i ${23_day_harib20b_OS} ${23_day_harib20b_SYS} ::
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/a.hrb ::a.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/hello.hrb ::hello.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/hello2.hrb ::hello2.hrb
  COMMAND mcopy -i ${23_day_harib20b_OS} ${root_BINARY_DIR}/projects/23_day/harib20b/hello5.hrb ::hello5.hrb
  DEPENDS 23_day_harib20b_sys ${root_BINARY_DIR}/projects/23_day/harib20b/a.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/hello3.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/hello4.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo2.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo3.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/star1.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/stars.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/hello.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/hello2.hrb ${root_BINARY_DIR}/projects/23_day/harib20b/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(23_day_harib20b_APIO ${root_BINARY_DIR}/projects/23_day/harib20b/a_nask.o)
set(23_day_harib20b_RANDO ${root_BINARY_DIR}/projects/23_day/harib20b/rand.o)
add_custom_command(
  OUTPUT ${23_day_harib20b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${23_day_harib20b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${23_day_harib20b_APIO}
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20b/a_nask.nas ${23_day_harib20b_APIO}
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/a_nask.nas
  COMMENT "Generating a_nask.o for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/a.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/a.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/a.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating a.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/hello3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/hello3.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating hello3.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/hello4.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/hello4.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating hello4.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating winhelo.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo2.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating winhelo2.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo3.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating winhelo3.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/star1.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/star1.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/star1.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating star1.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/stars.hrb
  COMMAND gcc ${BINOPT} -T ${23_day_harib20b_APPLDS} ${root_SOURCE_DIR}/projects/23_day/harib20b/stars.c ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO} -o ${root_BINARY_DIR}/projects/23_day/harib20b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/stars.c ${root_SOURCE_DIR}/projects/app.lds ${23_day_harib20b_APIO} ${23_day_harib20b_LIBGC} ${23_day_harib20b_RANDO}
  COMMENT "Generating stars.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/hello.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20b/hello.nas ${root_BINARY_DIR}/projects/23_day/harib20b/hello.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/hello.nas
  COMMENT "Generating hello.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/hello2.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20b/hello2.nas ${root_BINARY_DIR}/projects/23_day/harib20b/hello2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/hello2.nas
  COMMENT "Generating hello2.hrb for 23_day_harib20b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/23_day/harib20b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/23_day/harib20b/hello5.nas ${root_BINARY_DIR}/projects/23_day/harib20b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/23_day/harib20b/hello5.nas
  COMMENT "Generating hello5.hrb for 23_day_harib20b"
)
