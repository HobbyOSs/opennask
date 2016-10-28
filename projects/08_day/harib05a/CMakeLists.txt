#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05a/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(08_day_harib05a_OS    ${root_BINARY_DIR}/projects/08_day/harib05a/os.img)
set(08_day_harib05a_SYS	  ${root_BINARY_DIR}/projects/08_day/harib05a/os.sys)
set(08_day_harib05a_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05a/ipl.bin)
set(08_day_harib05a_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05a/ipl10.nas)
set(08_day_harib05a_HEADB ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead.bin)
set(08_day_harib05a_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05a/asmhead.nas)
set(08_day_harib05a_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05a/boot.bin)
set(08_day_harib05a_CCO	  ${root_BINARY_DIR}/projects/08_day/harib05a/boot.o)
set(08_day_harib05a_CCS	  ${root_SOURCE_DIR}/projects/08_day/harib05a/*.c)

add_custom_target(08_day_harib05a_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05a_OS}				
)											
add_custom_target(08_day_harib05a_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05a_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(08_day_harib05a_ipl						
  COMMAND ${NASK} ${08_day_harib05a_IPLS} ${08_day_harib05a_IPLB}		
)											
add_custom_target(08_day_harib05a_sys						
  COMMAND ${NASK} ${08_day_harib05a_HEADS} ${08_day_harib05a_HEADB}		
  COMMAND gcc ${08_day_harib05a_CCS}  -c -o ${08_day_harib05a_CCO}	
  COMMAND ld -o ${08_day_harib05a_BOOTB} -e _HariMain ${08_day_harib05a_CCO} ${08_day_harib05a_FUNCO}	
  COMMAND cat ${08_day_harib05a_HEADB} ${08_day_harib05a_BOOTB} > ${08_day_harib05a_SYS}			
)											
add_custom_target(08_day_harib05a_img						
  COMMAND mformat -f 1440 -C -B ${08_day_harib05a_IPLB} -i ${08_day_harib05a_OS}	
  COMMAND mcopy ${08_day_harib05a_SYS} -i ${08_day_harib05a_OS}			
)											
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/asmhead_wine.img)

add_custom_target(08_day_harib05a_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(08_day_harib05a_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(08_day_harib05a_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05a_asmhead)
add_dependencies(wine 08_day_harib05a_asmhead_wine)
add_dependencies(od 08_day_harib05a_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a/ipl10_wine.img)

add_custom_target(08_day_harib05a_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(08_day_harib05a_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(08_day_harib05a_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05a_ipl10)
add_dependencies(wine 08_day_harib05a_ipl10_wine)
add_dependencies(od 08_day_harib05a_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05a/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05a_wine.imgkfunc_wine.img)

add_custom_target(08_day_harib05akfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(08_day_harib05akfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(08_day_harib05akfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05akfunc)
add_dependencies(wine 08_day_harib05akfunc_wine)
add_dependencies(od 08_day_harib05akfunc_od)

#----------------------------------------------------------
