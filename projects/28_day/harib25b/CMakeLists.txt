#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25b/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25b_OS    ${root_BINARY_DIR}/projects/28_day/harib25b/os.img)
set(28_day_harib25b_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25b/os.sys)
set(28_day_harib25b_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25b/ipl.bin)
set(28_day_harib25b_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25b/haribote/ipl10.nas)
set(28_day_harib25b_HEADB ${root_BINARY_DIR}/projects/28_day/harib25b/asmhead.bin)
set(28_day_harib25b_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25b/haribote/asmhead.nas)
set(28_day_harib25b_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25b/boot.bin)
set(28_day_harib25b_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25b/boot.o)
file(GLOB 28_day_harib25b_CCS ${root_SOURCE_DIR}/projects/28_day/harib25b/haribote/*.c)
set(28_day_harib25b_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25b_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(28_day_harib25b_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25b/haribote/naskfunc.nas)
set(28_day_harib25b_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25b/naskfunc.o)
set(28_day_harib25b_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25b/haribote/hankaku.txt)
set(28_day_harib25b_FONTB ${root_BINARY_DIR}/projects/28_day/harib25b/hankaku.bin)
set(28_day_harib25b_FONTO ${root_BINARY_DIR}/projects/28_day/harib25b/hankaku.o)
set(28_day_harib25b_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25b_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25b/libgo.o)
set(28_day_harib25b_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25b/*.o)


add_custom_target(28_day_harib25b_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25b_OS}
  DEPENDS 28_day_harib25b_img
)
add_custom_target(28_day_harib25b_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25b_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 28_day_harib25b_img
)
add_custom_target(28_day_harib25b_clean
  COMMAND rm -f ${28_day_harib25b_OS}
  COMMAND rm -f ${28_day_harib25b_SYS}
  COMMAND rm -f ${28_day_harib25b_IPLB}
  COMMAND rm -f ${28_day_harib25b_HEADB}
  COMMAND rm -f ${28_day_harib25b_BOOTB}
  COMMAND rm -f ${28_day_harib25b_WILDOBJ}
)
add_custom_target(28_day_harib25b_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25b_IPLS} ${28_day_harib25b_IPLB}
  DEPENDS ${28_day_harib25b_IPLS}
)
add_custom_command(
  OUTPUT ${28_day_harib25b_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25b_HEADS} ${28_day_harib25b_HEADB}
  DEPENDS ${28_day_harib25b_HEADS}
  COMMENT "Generating asmhead.bin for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${28_day_harib25b_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25b_FUNCS} ${28_day_harib25b_FUNCO}
  DEPENDS ${28_day_harib25b_FUNCS}
  COMMENT "Generating naskfunc.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${28_day_harib25b_FONTO}
  COMMAND ${FONT} ${28_day_harib25b_FONTS} ${28_day_harib25b_FONTB}
  COMMAND ${B2O}  ${28_day_harib25b_FONTB} ${28_day_harib25b_FONTO} _hankaku
  DEPENDS ${28_day_harib25b_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${28_day_harib25b_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25b_LIBGE} ${28_day_harib25b_LIBGC}
  DEPENDS ${28_day_harib25b_LIBGE}
  COMMENT "Converting golibc for 28_day_harib25b"
)

add_custom_target(28_day_harib25b_sys
  COMMAND gcc ${BINOPT} -T ${28_day_harib25b_LDS} ${28_day_harib25b_CCS} ${28_day_harib25b_WILDOBJ} -o ${28_day_harib25b_BOOTB}
  COMMAND cat ${28_day_harib25b_HEADB} ${28_day_harib25b_BOOTB} > ${28_day_harib25b_SYS}
  DEPENDS ${28_day_harib25b_HEADB} 28_day_harib25b_ipl
  DEPENDS ${28_day_harib25b_FUNCO}
  DEPENDS ${28_day_harib25b_FONTO} ${28_day_harib25b_LIBGC}
)
add_custom_target(28_day_harib25b_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25b_IPLB} -i ${28_day_harib25b_OS}
  COMMAND mcopy -i ${28_day_harib25b_OS} ${28_day_harib25b_SYS} ::
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/a.hrb ::a.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/color.hrb ::color.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${28_day_harib25b_OS} ${root_BINARY_DIR}/projects/28_day/harib25b/hello5.hrb ::hello5.hrb
  DEPENDS 28_day_harib25b_sys ${root_BINARY_DIR}/projects/28_day/harib25b/a.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/hello3.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/hello4.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo2.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo3.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/star1.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/stars.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/stars2.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/lines.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/walk.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/sosu3.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/sosu2.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/sosu.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/noodle.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/beepdown.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/color.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/color2.hrb ${root_BINARY_DIR}/projects/28_day/harib25b/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(28_day_harib25b_APIO
  ${root_BINARY_DIR}/projects/28_day/harib25b/api001.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api002.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api003.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api004.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api005.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api006.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api007.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api008.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api009.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api010.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api011.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api012.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api013.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api014.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api015.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api016.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api017.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api018.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api019.o
  ${root_BINARY_DIR}/projects/28_day/harib25b/api020.o)
set(28_day_harib25b_RANDO ${root_BINARY_DIR}/projects/28_day/harib25b/rand.o)
add_custom_command(
  OUTPUT ${28_day_harib25b_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${28_day_harib25b_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api001.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api001.nas
  COMMENT "Generating api001.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api002.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api002.nas
  COMMENT "Generating api002.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api003.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api003.nas
  COMMENT "Generating api003.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api004.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api004.nas
  COMMENT "Generating api004.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api005.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api005.nas
  COMMENT "Generating api005.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api006.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api006.nas
  COMMENT "Generating api006.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api007.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api007.nas
  COMMENT "Generating api007.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api008.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api008.nas
  COMMENT "Generating api008.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api009.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api009.nas
  COMMENT "Generating api009.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api010.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api010.nas
  COMMENT "Generating api010.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api011.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api011.nas
  COMMENT "Generating api011.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api012.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api012.nas
  COMMENT "Generating api012.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api013.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api013.nas
  COMMENT "Generating api013.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api014.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api014.nas
  COMMENT "Generating api014.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api015.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api015.nas
  COMMENT "Generating api015.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api016.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api016.nas
  COMMENT "Generating api016.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api017.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api017.nas
  COMMENT "Generating api017.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api018.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api018.nas
  COMMENT "Generating api018.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api019.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api019.nas
  COMMENT "Generating api019.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api020.nas ${root_BINARY_DIR}/projects/28_day/harib25b/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/apilib/api020.nas
  COMMENT "Generating api020.o for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/a/a.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating a.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/hello3/hello3.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating hello3.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/hello4/hello4.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating hello4.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/winhelo/winhelo.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating winhelo.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/winhelo2/winhelo2.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating winhelo2.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/winhelo3/winhelo3.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating winhelo3.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/star1/star1.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating star1.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/stars/stars.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating stars.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/stars2/stars2.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating stars2.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/lines/lines.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating lines.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/walk/walk.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating walk.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/sosu3/sosu3.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating sosu3.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/sosu2/sosu2.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating sosu2.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/sosu/sosu.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating sosu.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/color2/color2.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating color2.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/color/color.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating color.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/beepdown/beepdown.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating beepdown.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25b -T ${28_day_harib25b_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25b/noodle/noodle.c ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25b/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25b_APIO} ${28_day_harib25b_LIBGC} ${28_day_harib25b_RANDO}
  COMMENT "Generating noodle.hrb for 28_day_harib25b"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25b/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25b/hello5/hello5.nas ${root_BINARY_DIR}/projects/28_day/harib25b/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25b/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 28_day_harib25b"
)
