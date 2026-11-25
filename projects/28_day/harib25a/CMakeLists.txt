#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25a/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25a_OS    ${root_BINARY_DIR}/projects/28_day/harib25a/os.img)
set(28_day_harib25a_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25a/os.sys)
set(28_day_harib25a_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25a/ipl.bin)
set(28_day_harib25a_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/ipl10.nas)
set(28_day_harib25a_HEADB ${root_BINARY_DIR}/projects/28_day/harib25a/asmhead.bin)
set(28_day_harib25a_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/asmhead.nas)
set(28_day_harib25a_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25a/boot.bin)
set(28_day_harib25a_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25a/boot.o)
file(GLOB 28_day_harib25a_CCS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/*.c)
set(28_day_harib25a_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25a_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(28_day_harib25a_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/naskfunc.nas)
set(28_day_harib25a_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25a/naskfunc.o)
set(28_day_harib25a_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25a/haribote/hankaku.txt)
set(28_day_harib25a_FONTB ${root_BINARY_DIR}/projects/28_day/harib25a/hankaku.bin)
set(28_day_harib25a_FONTO ${root_BINARY_DIR}/projects/28_day/harib25a/hankaku.o)
set(28_day_harib25a_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25a_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25a/libgo.o)
set(28_day_harib25a_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25a/*.o)


add_custom_target(28_day_harib25a_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25a_OS}
  DEPENDS 28_day_harib25a_img
)
add_custom_target(28_day_harib25a_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25a_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 28_day_harib25a_img
)
add_custom_target(28_day_harib25a_clean
  COMMAND rm -f ${28_day_harib25a_OS}
  COMMAND rm -f ${28_day_harib25a_SYS}
  COMMAND rm -f ${28_day_harib25a_IPLB}
  COMMAND rm -f ${28_day_harib25a_HEADB}
  COMMAND rm -f ${28_day_harib25a_BOOTB}
  COMMAND rm -f ${28_day_harib25a_WILDOBJ}
)
add_custom_target(28_day_harib25a_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25a_IPLS} ${28_day_harib25a_IPLB}
  DEPENDS ${28_day_harib25a_IPLS}
)
add_custom_command(
  OUTPUT ${28_day_harib25a_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25a_HEADS} ${28_day_harib25a_HEADB}
  DEPENDS ${28_day_harib25a_HEADS}
  COMMENT "Generating asmhead.bin for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${28_day_harib25a_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25a_FUNCS} ${28_day_harib25a_FUNCO}
  DEPENDS ${28_day_harib25a_FUNCS}
  COMMENT "Generating naskfunc.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${28_day_harib25a_FONTO}
  COMMAND ${FONT} ${28_day_harib25a_FONTS} ${28_day_harib25a_FONTB}
  COMMAND ${B2O}  ${28_day_harib25a_FONTB} ${28_day_harib25a_FONTO} _hankaku
  DEPENDS ${28_day_harib25a_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${28_day_harib25a_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25a_LIBGE} ${28_day_harib25a_LIBGC}
  DEPENDS ${28_day_harib25a_LIBGE}
  COMMENT "Converting golibc for 28_day_harib25a"
)

add_custom_target(28_day_harib25a_sys
  COMMAND gcc ${BINOPT} -T ${28_day_harib25a_LDS} ${28_day_harib25a_CCS} ${28_day_harib25a_WILDOBJ} -o ${28_day_harib25a_BOOTB}
  COMMAND cat ${28_day_harib25a_HEADB} ${28_day_harib25a_BOOTB} > ${28_day_harib25a_SYS}
  DEPENDS ${28_day_harib25a_HEADB} 28_day_harib25a_ipl
  DEPENDS ${28_day_harib25a_FUNCO}
  DEPENDS ${28_day_harib25a_FONTO} ${28_day_harib25a_LIBGC}
)
add_custom_target(28_day_harib25a_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25a_IPLB} -i ${28_day_harib25a_OS}
  COMMAND mcopy -i ${28_day_harib25a_OS} ${28_day_harib25a_SYS} ::
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/a.hrb ::a.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/color.hrb ::color.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${28_day_harib25a_OS} ${root_BINARY_DIR}/projects/28_day/harib25a/hello5.hrb ::hello5.hrb
  DEPENDS 28_day_harib25a_sys ${root_BINARY_DIR}/projects/28_day/harib25a/a.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/hello3.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/hello4.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo2.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo3.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/star1.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/stars.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/stars2.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/lines.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/walk.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/sosu3.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/sosu2.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/sosu.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/noodle.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/beepdown.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/color.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/color2.hrb ${root_BINARY_DIR}/projects/28_day/harib25a/hello5.hrb
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(28_day_harib25a_APIO
  ${root_BINARY_DIR}/projects/28_day/harib25a/api001.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api002.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api003.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api004.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api005.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api006.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api007.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api008.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api009.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api010.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api011.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api012.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api013.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api014.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api015.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api016.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api017.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api018.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api019.o
  ${root_BINARY_DIR}/projects/28_day/harib25a/api020.o)
set(28_day_harib25a_RANDO ${root_BINARY_DIR}/projects/28_day/harib25a/rand.o)
add_custom_command(
  OUTPUT ${28_day_harib25a_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${28_day_harib25a_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api001.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api001.nas
  COMMENT "Generating api001.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api002.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api002.nas
  COMMENT "Generating api002.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api003.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api003.nas
  COMMENT "Generating api003.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api004.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api004.nas
  COMMENT "Generating api004.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api005.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api005.nas
  COMMENT "Generating api005.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api006.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api006.nas
  COMMENT "Generating api006.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api007.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api007.nas
  COMMENT "Generating api007.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api008.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api008.nas
  COMMENT "Generating api008.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api009.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api009.nas
  COMMENT "Generating api009.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api010.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api010.nas
  COMMENT "Generating api010.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api011.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api011.nas
  COMMENT "Generating api011.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api012.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api012.nas
  COMMENT "Generating api012.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api013.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api013.nas
  COMMENT "Generating api013.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api014.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api014.nas
  COMMENT "Generating api014.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api015.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api015.nas
  COMMENT "Generating api015.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api016.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api016.nas
  COMMENT "Generating api016.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api017.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api017.nas
  COMMENT "Generating api017.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api018.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api018.nas
  COMMENT "Generating api018.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api019.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api019.nas
  COMMENT "Generating api019.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api020.nas ${root_BINARY_DIR}/projects/28_day/harib25a/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/apilib/api020.nas
  COMMENT "Generating api020.o for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/a/a.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating a.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/hello3/hello3.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating hello3.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/hello4/hello4.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating hello4.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/winhelo/winhelo.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating winhelo.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/winhelo2/winhelo2.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating winhelo2.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/winhelo3/winhelo3.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating winhelo3.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/star1/star1.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating star1.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/stars/stars.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating stars.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/stars2/stars2.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating stars2.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/lines/lines.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating lines.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/walk/walk.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating walk.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/sosu3/sosu3.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating sosu3.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/sosu2/sosu2.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating sosu2.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/sosu/sosu.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating sosu.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/color2/color2.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating color2.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/color/color.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating color.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/beepdown/beepdown.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating beepdown.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25a -T ${28_day_harib25a_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25a/noodle/noodle.c ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25a/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25a_APIO} ${28_day_harib25a_LIBGC} ${28_day_harib25a_RANDO}
  COMMENT "Generating noodle.hrb for 28_day_harib25a"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25a/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25a/hello5/hello5.nas ${root_BINARY_DIR}/projects/28_day/harib25a/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25a/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 28_day_harib25a"
)
