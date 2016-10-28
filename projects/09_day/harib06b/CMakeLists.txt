#----------------------------------------------------------
message(STATUS "Entering directory projects/09_day/harib06b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(09_day_harib06b_OS    ${root_BINARY_DIR}/projects/09_day/harib06b/os.img)
set(09_day_harib06b_SYS	  ${root_BINARY_DIR}/projects/09_day/harib06b/os.sys)
set(09_day_harib06b_IPLB  ${root_BINARY_DIR}/projects/09_day/harib06b/ipl.bin)
set(09_day_harib06b_IPLS  ${root_SOURCE_DIR}/projects/09_day/harib06b/ipl10.nas)
set(09_day_harib06b_HEADB ${root_BINARY_DIR}/projects/09_day/harib06b/asmhead.bin)
set(09_day_harib06b_HEADS ${root_SOURCE_DIR}/projects/09_day/harib06b/asmhead.nas)
set(09_day_harib06b_BOOTB ${root_BINARY_DIR}/projects/09_day/harib06b/boot.bin)
set(09_day_harib06b_CCO	  ${root_BINARY_DIR}/projects/09_day/harib06b/boot.o)
set(09_day_harib06b_CCS	  ${root_SOURCE_DIR}/projects/09_day/harib06b/*.c)

add_custom_target(09_day_harib06b_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${09_day_harib06b_OS}				
)											
add_custom_target(09_day_harib06b_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${09_day_harib06b_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(09_day_harib06b_ipl						
  COMMAND ${NASK} ${09_day_harib06b_IPLS} ${09_day_harib06b_IPLB}		
)											
add_custom_target(09_day_harib06b_sys						
  COMMAND ${NASK} ${09_day_harib06b_HEADS} ${09_day_harib06b_HEADB}		
  COMMAND gcc ${09_day_harib06b_CCS}  -c -o ${09_day_harib06b_CCO}	
  COMMAND ld -o ${09_day_harib06b_BOOTB} -e _HariMain ${09_day_harib06b_CCO} ${09_day_harib06b_FUNCO}	
  COMMAND cat ${09_day_harib06b_HEADB} ${09_day_harib06b_BOOTB} > ${09_day_harib06b_SYS}			
)											
add_custom_target(09_day_harib06b_img						
  COMMAND mformat -f 1440 -C -B ${09_day_harib06b_IPLB} -i ${09_day_harib06b_OS}	
  COMMAND mcopy ${09_day_harib06b_SYS} -i ${09_day_harib06b_OS}			
)											
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/asmhead_wine.img)

add_custom_target(09_day_harib06b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(09_day_harib06b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(09_day_harib06b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06b_asmhead)
add_dependencies(wine 09_day_harib06b_asmhead_wine)
add_dependencies(od 09_day_harib06b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b/ipl10_wine.img)

add_custom_target(09_day_harib06b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(09_day_harib06b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(09_day_harib06b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06b_ipl10)
add_dependencies(wine 09_day_harib06b_ipl10_wine)
add_dependencies(od 09_day_harib06b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/09_day/harib06b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/09_day/harib06b_wine.imgkfunc_wine.img)

add_custom_target(09_day_harib06bkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(09_day_harib06bkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(09_day_harib06bkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 09_day_harib06bkfunc)
add_dependencies(wine 09_day_harib06bkfunc_wine)
add_dependencies(od 09_day_harib06bkfunc_od)

#----------------------------------------------------------
