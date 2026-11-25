#----------------------------------------------------------
message(STATUS "Entering directory projects/29_day/harib26c/")

set(FONT ${root_BINARY_DIR}/src/makefont)
set(B2O  ${root_BINARY_DIR}/src/bin2obj)
set(CONV ${root_BINARY_DIR}/objconv/objconv)
set(29_day_harib26c_OS    ${root_BINARY_DIR}/projects/29_day/harib26c/os.img)
set(29_day_harib26c_SYS	  ${root_BINARY_DIR}/projects/29_day/harib26c/os.sys)
set(29_day_harib26c_IPLB  ${root_BINARY_DIR}/projects/29_day/harib26c/ipl.bin)
set(29_day_harib26c_IPLS  ${root_SOURCE_DIR}/projects/29_day/harib26c/haribote/ipl20.nas)
set(29_day_harib26c_HEADB ${root_BINARY_DIR}/projects/29_day/harib26c/asmhead.bin)
set(29_day_harib26c_HEADS ${root_SOURCE_DIR}/projects/29_day/harib26c/haribote/asmhead.nas)
set(29_day_harib26c_BOOTB ${root_BINARY_DIR}/projects/29_day/harib26c/boot.bin)
set(29_day_harib26c_CCO	  ${root_BINARY_DIR}/projects/29_day/harib26c/boot.o)
file(GLOB 29_day_harib26c_CCS ${root_SOURCE_DIR}/projects/29_day/harib26c/haribote/*.c)
set(29_day_harib26c_LDS	  ${root_SOURCE_DIR}/projects/os.lds)
set(29_day_harib26c_APPLDS  ${root_SOURCE_DIR}/projects/app.lds)
set(29_day_harib26c_FUNCS ${root_SOURCE_DIR}/projects/29_day/harib26c/haribote/naskfunc.nas)
set(29_day_harib26c_FUNCO ${root_BINARY_DIR}/projects/29_day/harib26c/naskfunc.o)
set(29_day_harib26c_FONTS ${root_SOURCE_DIR}/projects/29_day/harib26c/haribote/hankaku.txt)
set(29_day_harib26c_FONTB ${root_BINARY_DIR}/projects/29_day/harib26c/hankaku.bin)
set(29_day_harib26c_FONTO ${root_BINARY_DIR}/projects/29_day/harib26c/hankaku.o)
set(29_day_harib26c_LIBGE ${root_BINARY_DIR}/golibc/libgo.a)
set(29_day_harib26c_LIBGC ${root_BINARY_DIR}/projects/29_day/harib26c/libgo.o)
set(29_day_harib26c_WILDOBJ ${root_BINARY_DIR}/projects/29_day/harib26c/*.o)


add_custom_target(29_day_harib26c_run
  COMMAND ${QEMU} ${QEMUOPT} ${29_day_harib26c_OS}
  DEPENDS 29_day_harib26c_img
)
add_custom_target(29_day_harib26c_debug
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${29_day_harib26c_OS} ${QEMU_DEBUG_OPT}
  DEPENDS 29_day_harib26c_img
)
add_custom_target(29_day_harib26c_clean
  COMMAND rm -f ${29_day_harib26c_OS}
  COMMAND rm -f ${29_day_harib26c_SYS}
  COMMAND rm -f ${29_day_harib26c_IPLB}
  COMMAND rm -f ${29_day_harib26c_HEADB}
  COMMAND rm -f ${29_day_harib26c_BOOTB}
  COMMAND rm -f ${29_day_harib26c_WILDOBJ}
)
add_custom_target(29_day_harib26c_ipl
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26c_IPLS} ${29_day_harib26c_IPLB}
  DEPENDS ${29_day_harib26c_IPLS}
)
add_custom_command(
  OUTPUT ${29_day_harib26c_HEADB}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26c_HEADS} ${29_day_harib26c_HEADB}
  DEPENDS ${29_day_harib26c_HEADS}
  COMMENT "Generating asmhead.bin for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${29_day_harib26c_FUNCO}
  COMMAND ${RAKUSK_EXECUTABLE} ${29_day_harib26c_FUNCS} ${29_day_harib26c_FUNCO}
  DEPENDS ${29_day_harib26c_FUNCS}
  COMMENT "Generating naskfunc.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${29_day_harib26c_FONTO}
  COMMAND ${FONT} ${29_day_harib26c_FONTS} ${29_day_harib26c_FONTB}
  COMMAND ${B2O}  ${29_day_harib26c_FONTB} ${29_day_harib26c_FONTO} _hankaku
  DEPENDS ${29_day_harib26c_FONTS} makefont bin2obj
  COMMENT "Generating hankaku.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${29_day_harib26c_LIBGC}
  COMMAND ${CONV} -fcoff32 -nu ${29_day_harib26c_LIBGE} ${29_day_harib26c_LIBGC}
  DEPENDS ${29_day_harib26c_LIBGE}
  COMMENT "Converting golibc for 29_day_harib26c"
)

add_custom_target(29_day_harib26c_sys
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -T ${29_day_harib26c_LDS} ${29_day_harib26c_CCS} ${29_day_harib26c_WILDOBJ} -o ${29_day_harib26c_BOOTB}
  COMMAND cat ${29_day_harib26c_HEADB} ${29_day_harib26c_BOOTB} > ${29_day_harib26c_SYS}
  DEPENDS ${29_day_harib26c_HEADB} 29_day_harib26c_ipl
  DEPENDS ${29_day_harib26c_FUNCO}
  DEPENDS ${29_day_harib26c_FONTO} ${29_day_harib26c_LIBGC}
)
add_custom_target(29_day_harib26c_img
  COMMAND mformat -f 1440 -l HARIBOTEOS -N 0xffffffff -C -B ${29_day_harib26c_IPLB} -i ${29_day_harib26c_OS}
  COMMAND mcopy -i ${29_day_harib26c_OS} ${29_day_harib26c_SYS} ::
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/a.hrb ::a.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/hello3.hrb ::hello3.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/hello4.hrb ::hello4.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo.hrb ::winhelo.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo2.hrb ::winhelo2.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo3.hrb ::winhelo3.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/star1.hrb ::star1.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/stars.hrb ::stars.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/stars2.hrb ::stars2.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/lines.hrb ::lines.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/walk.hrb ::walk.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/notrec.hrb ::notrec.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/chklang.hrb ::chklang.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/iroha.hrb ::iroha.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/type.hrb ::type.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/sosu3.hrb ::sosu3.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/sosu2.hrb ::sosu2.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/sosu.hrb ::sosu.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/color2.hrb ::color2.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/color.hrb ::color.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/beepdown.hrb ::beepdown.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/noodle.hrb ::noodle.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_BINARY_DIR}/projects/29_day/harib26c/hello5.hrb ::hello5.hrb
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_SOURCE_DIR}/projects/29_day/harib26c/euc.txt ::euc.txt
  COMMAND mcopy -i ${29_day_harib26c_OS} ${root_SOURCE_DIR}/projects/29_day/harib26c/nihongo/nihongo.fnt ::nihongo.fnt
  DEPENDS 29_day_harib26c_sys ${root_BINARY_DIR}/projects/29_day/harib26c/a.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/hello3.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/hello4.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo2.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo3.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/star1.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/stars.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/stars2.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/lines.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/walk.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/notrec.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/chklang.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/iroha.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/type.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/sosu3.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/sosu2.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/sosu.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/noodle.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/beepdown.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/color.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/color2.hrb ${root_BINARY_DIR}/projects/29_day/harib26c/hello5.hrb ${root_SOURCE_DIR}/projects/29_day/harib26c/nihongo/nihongo.fnt ${root_SOURCE_DIR}/projects/29_day/harib26c/euc.txt
)

#----------------------------------------------------------
########### application .hrb files (copied into os.img) ###############
set(29_day_harib26c_APIO
  ${root_BINARY_DIR}/projects/29_day/harib26c/api001.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api002.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api003.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api004.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api005.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api006.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api007.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api008.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api009.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api010.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api011.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api012.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api013.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api014.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api015.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api016.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api017.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api018.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api019.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api020.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api021.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api022.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api023.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api024.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api025.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api026.o
  ${root_BINARY_DIR}/projects/29_day/harib26c/api027.o)
set(29_day_harib26c_RANDO ${root_BINARY_DIR}/projects/29_day/harib26c/rand.o)
add_custom_command(
  OUTPUT ${29_day_harib26c_RANDO}
  COMMAND gcc -fleading-underscore -m32 -nostdlib -fno-pie -x c -c ${root_SOURCE_DIR}/golibc/rand.c_ -o ${29_day_harib26c_RANDO}
  DEPENDS ${root_SOURCE_DIR}/golibc/rand.c_
  COMMENT "Generating rand.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api001.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api001.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api001.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api001.nas
  COMMENT "Generating api001.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api002.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api002.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api002.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api002.nas
  COMMENT "Generating api002.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api003.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api003.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api003.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api003.nas
  COMMENT "Generating api003.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api004.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api004.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api004.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api004.nas
  COMMENT "Generating api004.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api005.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api005.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api005.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api005.nas
  COMMENT "Generating api005.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api006.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api006.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api006.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api006.nas
  COMMENT "Generating api006.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api007.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api007.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api007.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api007.nas
  COMMENT "Generating api007.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api008.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api008.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api008.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api008.nas
  COMMENT "Generating api008.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api009.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api009.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api009.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api009.nas
  COMMENT "Generating api009.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api010.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api010.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api010.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api010.nas
  COMMENT "Generating api010.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api011.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api011.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api011.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api011.nas
  COMMENT "Generating api011.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api012.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api012.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api012.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api012.nas
  COMMENT "Generating api012.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api013.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api013.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api013.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api013.nas
  COMMENT "Generating api013.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api014.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api014.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api014.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api014.nas
  COMMENT "Generating api014.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api015.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api015.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api015.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api015.nas
  COMMENT "Generating api015.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api016.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api016.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api016.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api016.nas
  COMMENT "Generating api016.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api017.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api017.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api017.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api017.nas
  COMMENT "Generating api017.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api018.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api018.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api018.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api018.nas
  COMMENT "Generating api018.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api019.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api019.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api019.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api019.nas
  COMMENT "Generating api019.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api020.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api020.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api020.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api020.nas
  COMMENT "Generating api020.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api021.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api021.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api021.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api021.nas
  COMMENT "Generating api021.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api022.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api022.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api022.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api022.nas
  COMMENT "Generating api022.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api023.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api023.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api023.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api023.nas
  COMMENT "Generating api023.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api024.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api024.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api024.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api024.nas
  COMMENT "Generating api024.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api025.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api025.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api025.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api025.nas
  COMMENT "Generating api025.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api026.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api026.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api026.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api026.nas
  COMMENT "Generating api026.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/api027.o
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api027.nas ${root_BINARY_DIR}/projects/29_day/harib26c/api027.o
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/apilib/api027.nas
  COMMENT "Generating api027.o for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/a.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/a/a.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/a.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/a/a.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating a.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/hello3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/hello3/hello3.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/hello3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/hello3/hello3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating hello3.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/hello4.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/hello4/hello4.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/hello4.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/hello4/hello4.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating hello4.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/winhelo/winhelo.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/winhelo/winhelo.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating winhelo.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/winhelo2/winhelo2.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/winhelo2/winhelo2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating winhelo2.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/winhelo3/winhelo3.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/winhelo3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/winhelo3/winhelo3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating winhelo3.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/star1.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/star1/star1.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/star1.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/star1/star1.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating star1.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/stars.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/stars/stars.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/stars.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/stars/stars.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating stars.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/stars2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/stars2/stars2.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/stars2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/stars2/stars2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating stars2.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/lines.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/lines/lines.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/lines.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/lines/lines.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating lines.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/walk.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/walk/walk.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/walk.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/walk/walk.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating walk.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/notrec.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/notrec/notrec.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/notrec.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/notrec/notrec.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating notrec.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/chklang.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/chklang/chklang.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/chklang.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/chklang/chklang.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating chklang.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/iroha.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/iroha/iroha.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/iroha.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/iroha/iroha.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating iroha.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/type.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/type/type.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/type.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/type/type.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating type.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/sosu3.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/sosu3/sosu3.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/sosu3.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/sosu3/sosu3.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating sosu3.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/sosu2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/sosu2/sosu2.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/sosu2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/sosu2/sosu2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating sosu2.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/sosu.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/sosu/sosu.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/sosu.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/sosu/sosu.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating sosu.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/color2.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/color2/color2.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/color2.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/color2/color2.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating color2.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/color.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/color/color.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/color.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/color/color.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating color.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/beepdown.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/beepdown/beepdown.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/beepdown.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/beepdown/beepdown.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating beepdown.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/noodle.hrb
  COMMAND gcc ${BINOPT} -I ${root_SOURCE_DIR}/golibc -I ${root_SOURCE_DIR}/projects/29_day/harib26c -T ${29_day_harib26c_APPLDS} ${root_SOURCE_DIR}/projects/29_day/harib26c/noodle/noodle.c ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO} -o ${root_BINARY_DIR}/projects/29_day/harib26c/noodle.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/noodle/noodle.c ${root_SOURCE_DIR}/projects/app.lds ${29_day_harib26c_APIO} ${29_day_harib26c_LIBGC} ${29_day_harib26c_RANDO}
  COMMENT "Generating noodle.hrb for 29_day_harib26c"
)
add_custom_command(
  OUTPUT ${root_BINARY_DIR}/projects/29_day/harib26c/hello5.hrb
  COMMAND ${RAKUSK_EXECUTABLE} ${root_SOURCE_DIR}/projects/29_day/harib26c/hello5/hello5.nas ${root_BINARY_DIR}/projects/29_day/harib26c/hello5.hrb
  DEPENDS ${root_SOURCE_DIR}/projects/29_day/harib26c/hello5/hello5.nas
  COMMENT "Generating hello5.hrb for 29_day_harib26c"
)
