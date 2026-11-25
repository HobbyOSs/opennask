#----------------------------------------------------------
message(STATUS "Entering directory projects/28_day/harib25f/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(28_day_harib25f_OS    ${root_BINARY_DIR}/projects/28_day/harib25f/os.img)
set(28_day_harib25f_SYS	  ${root_BINARY_DIR}/projects/28_day/harib25f/os.sys)
set(28_day_harib25f_IPLB  ${root_BINARY_DIR}/projects/28_day/harib25f/ipl.bin)
set(28_day_harib25f_IPLS  ${root_SOURCE_DIR}/projects/28_day/harib25f/haribote/ipl10.nas)
set(28_day_harib25f_HEADB ${root_BINARY_DIR}/projects/28_day/harib25f/asmhead.bin)
set(28_day_harib25f_HEADS ${root_SOURCE_DIR}/projects/28_day/harib25f/haribote/asmhead.nas)
set(28_day_harib25f_BOOTB ${root_BINARY_DIR}/projects/28_day/harib25f/boot.bin)
set(28_day_harib25f_CCO	  ${root_BINARY_DIR}/projects/28_day/harib25f/boot.o)
file(GLOB 28_day_harib25f_CCS ${root_SOURCE_DIR}/projects/28_day/harib25f/haribote/*.c)
set(28_day_harib25f_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(28_day_harib25f_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(28_day_harib25f_FUNCS ${root_SOURCE_DIR}/projects/28_day/harib25f/haribote/naskfunc.nas)
set(28_day_harib25f_FUNCO ${root_BINARY_DIR}/projects/28_day/harib25f/naskfunc.o)
set(28_day_harib25f_FONTS ${root_SOURCE_DIR}/projects/28_day/harib25f/haribote/hankaku.txt)
set(28_day_harib25f_FONTB ${root_BINARY_DIR}/projects/28_day/harib25f/hankaku.bin)
set(28_day_harib25f_FONTO ${root_BINARY_DIR}/projects/28_day/harib25f/hankaku.o)
set(28_day_harib25f_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(28_day_harib25f_LIBGC ${root_BINARY_DIR}/projects/28_day/harib25f/libgo.o)
set(28_day_harib25f_WILDOBJ ${root_BINARY_DIR}/projects/28_day/harib25f/*.o)


add_custom_target(28_day_harib25f_run
  COMMAND ${QEMU} ${QEMUOPT} ${28_day_harib25f_OS}
  DEPENDS 28_day_harib25f_img
)
add_custom_target(28_day_harib25f_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${28_day_harib25f_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 28_day_harib25f_img
)
add_custom_target(28_day_harib25f_clean
  COMMAND rm -f ${28_day_harib25f_OS}
  COMMAND rm -f ${28_day_harib25f_SYS}
  COMMAND rm -f ${28_day_harib25f_IPLB}
  COMMAND rm -f ${28_day_harib25f_HEADB}
  COMMAND rm -f ${28_day_harib25f_BOOTB}
  COMMAND rm -f ${28_day_harib25f_WILDOBJ}
)
add_custom_target(28_day_harib25f_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25f_IPLS} ${28_day_harib25f_IPLB}
  DEPENDS ${28_day_harib25f_IPLS}
)
add_custom_command(
  OUTPUT ${28_day_harib25f_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25f_HEADS} ${28_day_harib25f_HEADB}
  DEPENDS ${28_day_harib25f_HEADS}
  COMMENT "Generating asmhead.bin for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${28_day_harib25f_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${28_day_harib25f_FUNCS} ${28_day_harib25f_FUNCO}
  DEPENDS ${28_day_harib25f_FUNCS}
  COMMENT "Generating naskfunc.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${28_day_harib25f_FONTO}
  COMMAND ${FONT} ${28_day_harib25f_FONTS} ${28_day_harib25f_FONTB}
  COMMAND ${B2O}  ${28_day_harib25f_FONTB} ${28_day_harib25f_FONTO} _hankaku
  DEPENDS ${28_day_harib25f_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${28_day_harib25f_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${28_day_harib25f_LIBGE} ${28_day_harib25f_LIBGC}
  DEPENDS ${28_day_harib25f_LIBGE}
  COMMENT "Converting golibc for 28_day_harib25f"
)

add_custom_target(28_day_harib25f_sys
  COMMAND gcc ${BINOPT} -T ${28_day_harib25f_LDS} ${28_day_harib25f_CCS} ${28_day_harib25f_WILDOBJ} -o ${28_day_harib25f_BOOTB}
  COMMAND cat ${28_day_harib25f_HEADB} ${28_day_harib25f_BOOTB} > ${28_day_harib25f_SYS}
  DEPENDS ${28_day_harib25f_HEADB} 28_day_harib25f_ipl
  DEPENDS ${28_day_harib25f_FUNCO}
  DEPENDS ${28_day_harib25f_FONTO} ${28_day_harib25f_LIBGC}
)
add_custom_target(28_day_harib25f_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${28_day_harib25f_IPLB} -i ${28_day_harib25f_OS}
  COMMAND mcopy -i ${28_day_harib25f_OS} ${28_day_harib25f_SYS} ::
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/a.hrb ::a.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/type.hrb ::type.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/typeipl.hrb ::typeipl.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/color.hrb ::color.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_BINARY_DIR}/projects/28_day/harib25f/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${28_day_harib25f_OS} ${root_SOURCE_DIR}/projects/28_day/harib25f/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 28_day_harib25f_sys ${root_BINARY_DIR}/projects/28_day/harib25f/a.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/hello3.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/hello4.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo2.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo3.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/star1.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/stars.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/stars2.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/lines.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/walk.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/iroha.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/type.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/typeipl.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/sosu3.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/sosu2.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/sosu.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/noodle.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/beepdown.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/color.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/color2.hrb ${root_BINARY_DIR}/projects/28_day/harib25f/hello5.hrb ${root_SOURCE_DIR}/projects/28_day/harib25f/nihongo/nihongo.fnt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(28_day_harib25f_APIO
  ${root_BINARY_DIR}/projects/28_day/harib25f/api001.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api002.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api003.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api004.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api005.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api006.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api007.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api008.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api009.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api010.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api011.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api012.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api013.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api014.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api015.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api016.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api017.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api018.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api019.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api020.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api021.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api022.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api023.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api024.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api025.o
  ${root_BINARY_DIR}/projects/28_day/harib25f/api026.o)
set(28_day_harib25f_RANDO ${root_BINARY_DIR}/projects/28_day/harib25f/rand.o)
add_custom_command(
  OUTPUT ${28_day_harib25f_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${28_day_harib25f_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api001.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api001.nas
  COMMENT "Generating api001.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api002.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api002.nas
  COMMENT "Generating api002.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api003.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api003.nas
  COMMENT "Generating api003.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api004.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api004.nas
  COMMENT "Generating api004.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api005.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api005.nas
  COMMENT "Generating api005.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api006.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api006.nas
  COMMENT "Generating api006.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api007.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api007.nas
  COMMENT "Generating api007.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api008.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api008.nas
  COMMENT "Generating api008.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api009.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api009.nas
  COMMENT "Generating api009.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api010.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api010.nas
  COMMENT "Generating api010.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api011.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api011.nas
  COMMENT "Generating api011.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api012.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api012.nas
  COMMENT "Generating api012.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api013.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api013.nas
  COMMENT "Generating api013.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api014.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api014.nas
  COMMENT "Generating api014.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api015.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api015.nas
  COMMENT "Generating api015.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api016.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api016.nas
  COMMENT "Generating api016.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api017.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api017.nas
  COMMENT "Generating api017.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api018.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api018.nas
  COMMENT "Generating api018.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api019.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api019.nas
  COMMENT "Generating api019.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api020.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api020.nas
  COMMENT "Generating api020.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api021.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api021.nas
  COMMENT "Generating api021.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api022.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api022.nas
  COMMENT "Generating api022.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api023.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api023.nas
  COMMENT "Generating api023.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api024.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api024.nas
  COMMENT "Generating api024.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api025.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api025.nas
  COMMENT "Generating api025.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api026.nas ${root_BINARY_DIR}/projects/28_day/harib25f/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/apilib/api026.nas
  COMMENT "Generating api026.o for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/a/a.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating a.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/hello3/hello3.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating hello3.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/hello4/hello4.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating hello4.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/winhelo/winhelo.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating winhelo.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/winhelo2/winhelo2.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating winhelo2.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/winhelo3/winhelo3.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating winhelo3.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/star1/star1.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating star1.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/stars/stars.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating stars.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/stars2/stars2.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating stars2.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/lines/lines.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating lines.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/walk/walk.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating walk.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/iroha/iroha.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating iroha.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/type/type.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating type.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/typeipl.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/typeipl/typeipl.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/typeipl.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/typeipl/typeipl.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating typeipl.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/sosu3/sosu3.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating sosu3.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/sosu2/sosu2.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating sosu2.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/sosu/sosu.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating sosu.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/color2/color2.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating color2.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/color/color.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating color.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/beepdown/beepdown.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating beepdown.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/projects/28_day/harib25f -T ${28_day_harib25f_APPLDS} ${root_SOURCE_DIR}/projects/28_day/harib25f/noodle/noodle.c ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO} -o ${root_BINARY_DIR}/projects/28_day/harib25f/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${28_day_harib25f_APIO} ${28_day_harib25f_LIBGC} ${28_day_harib25f_RANDO}
  COMMENT "Generating noodle.hrb for 28_day_harib25f"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/28_day/harib25f/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/28_day/harib25f/hello5/hello5.nas ${root_BINARY_DIR}/projects/28_day/harib25f/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/28_day/harib25f/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 28_day_harib25f"
)
