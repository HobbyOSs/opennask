#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25g/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25g_OS    ${root_BINARY_DIR}/projects/28_day/harib25g/os.img)
set(28_day_harib25g_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25g/os.sys)
set(28_day_harib25g_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25g/ipl.bin)
set(28_day_harib25g_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25g/haribote/ipl20.nas)
set(28_day_harib25g_HEADB ${root_BINARY_DIR}/projects/28_day/harib25g/asmhead.bin)
set(28_day_harib25g_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25g/haribote/asmhead.nas)
set(28_day_harib25g_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25g/boot.bin)
set(28_day_harib25g_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25g/boot.o)
file(GLOB 28_day_harib25g_CCS ${root_SOURCE_DIR}/projects/28_day/harib25g/haribote/*.c)
set(28_day_harib25g_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25g_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(28_day_harib25g_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25g/haribote/naskfunc.nas)
set(28_day_harib25g_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25g/naskfunc.o)
set(28_day_harib25g_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25g/haribote/hankaku.txt)
set(28_day_harib25g_FONTB ${root_BINARY_DIR}/projects/28_day/harib25g/hankaku.bin)
set(28_day_harib25g_FONTO ${root_BINARY_DIR}/projects/28_day/harib25g/hankaku.o)
set(28_day_harib25g_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25g_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25g/libgo.o)
set(28_day_harib25g_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25g/*.o)


add_custom_target(28_day_harib25g_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25g_OS}
  DEPENDS 28_day_harib25g_img
)
add_custom_target(28_day_harib25g_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25g_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 28_day_harib25g_img
)
add_custom_target(28_day_harib25g_clean
  COMMAND rm -f ${28_day_harib25g_OS}
  COMMAND rm -f ${28_day_harib25g_SYS}
  COMMAND rm -f ${28_day_harib25g_IPLB}
  COMMAND rm -f ${28_day_harib25g_HEADB}
  COMMAND rm -f ${28_day_harib25g_BOOTB}
  COMMAND rm -f ${28_day_harib25g_WILDOBJ}
)
add_custom_target(28_day_harib25g_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25g_IPLS} ${28_day_harib25g_IPLB}
  DEPENDS ${28_day_harib25g_IPLS}
)
add_custom_command(
  OUTPUT ${28_day_harib25g_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25g_HEADS} ${28_day_harib25g_HEADB}
  DEPENDS ${28_day_harib25g_HEADS}
  COMMENT "Generating asmhead.bin for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${28_day_harib25g_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25g_FUNCS} ${28_day_harib25g_FUNCO}
  DEPENDS ${28_day_harib25g_FUNCS}
  COMMENT "Generating naskfunc.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${28_day_harib25g_FONTO}
  COMMAND ${FONT} ${28_day_harib25g_FONTS} ${28_day_harib25g_FONTB}
  COMMAND ${B2O}  ${28_day_harib25g_FONTB} ${28_day_harib25g_FONTO} _hankaku
  DEPENDS ${28_day_harib25g_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${28_day_harib25g_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25g_LIBGE} ${28_day_harib25g_LIBGC}
  DEPENDS ${28_day_harib25g_LIBGE}
  COMMENT "Converting golibc for 28_day_harib25g"
)

add_custom_target(28_day_harib25g_sys
  COMMAND gcc ${BINOPT} -T ${28_day_harib25g_LDS} ${28_day_harib25g_CCS} ${28_day_harib25g_WILDOBJ} -o ${28_day_harib25g_BOOTB}
  COMMAND cat ${28_day_harib25g_HEADB} ${28_day_harib25g_BOOTB} > ${28_day_harib25g_SYS}
  DEPENDS ${28_day_harib25g_HEADB} 28_day_harib25g_ipl
  DEPENDS ${28_day_harib25g_FUNCO}
  DEPENDS ${28_day_harib25g_FONTO} ${28_day_harib25g_LIBGC}
)
add_custom_target(28_day_harib25g_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25g_IPLB} -i ${28_day_harib25g_OS}
  COMMAND mcopy -i ${28_day_harib25g_OS} ${28_day_harib25g_SYS} ::
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/a.hrb ::a.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/type.hrb ::type.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/typeipl.hrb ::typeipl.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/color.hrb ::color.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_BINARY_DIR}/projects/28_day/harib25g/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_SOURCE_DIR}/projects/28_day/harib25g/euc.txt ::euc.txt
  COMMAND mcopy -i ${28_day_harib25g_OS} ${root_SOURCE_DIR}/projects/28_day/harib25g/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 28_day_harib25g_sys ${root_BINARY_DIR}/projects/28_day/harib25g/a.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/hello3.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/hello4.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo2.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo3.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/star1.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/stars.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/stars2.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/lines.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/walk.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/chklang.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/iroha.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/type.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/typeipl.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/sosu3.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/sosu2.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/sosu.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/noodle.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/beepdown.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/color.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/color2.hrb ${root_BINARY_DIR}/projects/28_day/harib25g/hello5.hrb ${root_SOURCE_DIR}/projects/28_day/harib25g/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/28_day/harib25g/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(28_day_harib25g_APIO
  ${root_BINARY_DIR}/projects/28_day/harib25g/api001.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api002.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api003.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api004.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api005.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api006.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api007.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api008.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api009.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api010.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api011.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api012.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api013.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api014.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api015.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api016.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api017.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api018.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api019.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api020.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api021.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api022.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api023.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api024.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api025.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api026.o
  ${root_BINARY_DIR}/projects/28_day/harib25g/api027.o)
set(28_day_harib25g_RANDO ${root_BINARY_DIR}/projects/28_day/harib25g/rand.o)
add_custom_command(
  OUTPUT ${28_day_harib25g_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${28_day_harib25g_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api001.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api001.nas
  COMMENT "Generating api001.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api002.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api002.nas
  COMMENT "Generating api002.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api003.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api003.nas
  COMMENT "Generating api003.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api004.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api004.nas
  COMMENT "Generating api004.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api005.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api005.nas
  COMMENT "Generating api005.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api006.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api006.nas
  COMMENT "Generating api006.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api007.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api007.nas
  COMMENT "Generating api007.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api008.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api008.nas
  COMMENT "Generating api008.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api009.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api009.nas
  COMMENT "Generating api009.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api010.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api010.nas
  COMMENT "Generating api010.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api011.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api011.nas
  COMMENT "Generating api011.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api012.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api012.nas
  COMMENT "Generating api012.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api013.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api013.nas
  COMMENT "Generating api013.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api014.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api014.nas
  COMMENT "Generating api014.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api015.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api015.nas
  COMMENT "Generating api015.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api016.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api016.nas
  COMMENT "Generating api016.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api017.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api017.nas
  COMMENT "Generating api017.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api018.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api018.nas
  COMMENT "Generating api018.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api019.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api019.nas
  COMMENT "Generating api019.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api020.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api020.nas
  COMMENT "Generating api020.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api021.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api021.nas
  COMMENT "Generating api021.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api022.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api022.nas
  COMMENT "Generating api022.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api023.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api023.nas
  COMMENT "Generating api023.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api024.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api024.nas
  COMMENT "Generating api024.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api025.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api025.nas
  COMMENT "Generating api025.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api026.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api026.nas
  COMMENT "Generating api026.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api027.nas ${root_BINARY_DIR}/projects/28_day/harib25g/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/apilib/api027.nas
  COMMENT "Generating api027.o for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/a/a.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating a.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/hello3/hello3.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating hello3.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/hello4/hello4.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating hello4.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/winhelo/winhelo.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating winhelo.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/winhelo2/winhelo2.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating winhelo2.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/winhelo3/winhelo3.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating winhelo3.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/star1/star1.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating star1.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/stars/stars.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating stars.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/stars2/stars2.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating stars2.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/lines/lines.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating lines.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/walk/walk.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating walk.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/chklang/chklang.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating chklang.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/iroha/iroha.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating iroha.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/type/type.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating type.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/typeipl.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/typeipl/typeipl.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/typeipl.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/typeipl/typeipl.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating typeipl.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/sosu3/sosu3.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating sosu3.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/sosu2/sosu2.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating sosu2.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/sosu/sosu.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating sosu.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/color2/color2.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating color2.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/color/color.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating color.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/beepdown/beepdown.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating beepdown.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25g -T ${28_day_harib25g_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25g/noodle/noodle.c ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25g/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25g_APIO} ${28_day_harib25g_LIBGC} ${28_day_harib25g_RANDO}
  COMMENT "Generating noodle.hrb for 28_day_harib25g"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25g/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25g/hello5/hello5.nas ${root_BINARY_DIR}/projects/28_day/harib25g/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25g/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 28_day_harib25g"
)
