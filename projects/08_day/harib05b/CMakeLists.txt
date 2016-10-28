#----------------------------------------------------------
message(STATUS "Entering directory projects/08_day/harib05b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(08_day_harib05b_OS    ${root_BINARY_DIR}/projects/08_day/harib05b/os.img)
set(08_day_harib05b_SYS	  ${root_BINARY_DIR}/projects/08_day/harib05b/os.sys)
set(08_day_harib05b_IPLB  ${root_BINARY_DIR}/projects/08_day/harib05b/ipl.bin)
set(08_day_harib05b_IPLS  ${root_SOURCE_DIR}/projects/08_day/harib05b/ipl10.nas)
set(08_day_harib05b_HEADB ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead.bin)
set(08_day_harib05b_HEADS ${root_SOURCE_DIR}/projects/08_day/harib05b/asmhead.nas)
set(08_day_harib05b_BOOTB ${root_BINARY_DIR}/projects/08_day/harib05b/boot.bin)
set(08_day_harib05b_CCO	  ${root_BINARY_DIR}/projects/08_day/harib05b/boot.o)
set(08_day_harib05b_CCS	  ${root_SOURCE_DIR}/projects/08_day/harib05b/*.c)

add_custom_target(08_day_harib05b_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${08_day_harib05b_OS}				
)											
add_custom_target(08_day_harib05b_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${08_day_harib05b_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(08_day_harib05b_ipl						
  COMMAND ${NASK} ${08_day_harib05b_IPLS} ${08_day_harib05b_IPLB}		
)											
add_custom_target(08_day_harib05b_sys						
  COMMAND ${NASK} ${08_day_harib05b_HEADS} ${08_day_harib05b_HEADB}		
  COMMAND gcc ${08_day_harib05b_CCS}  -c -o ${08_day_harib05b_CCO}	
  COMMAND ld -o ${08_day_harib05b_BOOTB} -e _HariMain ${08_day_harib05b_CCO} ${08_day_harib05b_FUNCO}	
  COMMAND cat ${08_day_harib05b_HEADB} ${08_day_harib05b_BOOTB} > ${08_day_harib05b_SYS}			
)											
add_custom_target(08_day_harib05b_img						
  COMMAND rm -f ${08_day_harib05b_OS}
  COMMAND mformat -f 1440 -C -B ${08_day_harib05b_IPLB} -i ${08_day_harib05b_OS}	
  COMMAND mcopy -n ${08_day_harib05b_SYS} ${08_day_harib05b_OS}			
)											
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/asmhead_wine.img)

add_custom_target(08_day_harib05b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(08_day_harib05b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(08_day_harib05b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05b_asmhead)
add_dependencies(wine 08_day_harib05b_asmhead_wine)
add_dependencies(od 08_day_harib05b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b/ipl10_wine.img)

add_custom_target(08_day_harib05b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(08_day_harib05b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(08_day_harib05b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05b_ipl10)
add_dependencies(wine 08_day_harib05b_ipl10_wine)
add_dependencies(od 08_day_harib05b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/08_day/harib05b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/08_day/harib05b_wine.imgkfunc_wine.img)

add_custom_target(08_day_harib05bkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(08_day_harib05bkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(08_day_harib05bkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 08_day_harib05bkfunc)
add_dependencies(wine 08_day_harib05bkfunc_wine)
add_dependencies(od 08_day_harib05bkfunc_od)

#----------------------------------------------------------
