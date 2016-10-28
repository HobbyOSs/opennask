#----------------------------------------------------------
message(STATUS "Entering directory projects/10_day/harib07b/")

set(NASK ${root_BINARY_DIR}/src/opennask)
set(10_day_harib07b_OS    ${root_BINARY_DIR}/projects/10_day/harib07b/os.img)
set(10_day_harib07b_SYS	  ${root_BINARY_DIR}/projects/10_day/harib07b/os.sys)
set(10_day_harib07b_IPLB  ${root_BINARY_DIR}/projects/10_day/harib07b/ipl.bin)
set(10_day_harib07b_IPLS  ${root_SOURCE_DIR}/projects/10_day/harib07b/ipl10.nas)
set(10_day_harib07b_HEADB ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead.bin)
set(10_day_harib07b_HEADS ${root_SOURCE_DIR}/projects/10_day/harib07b/asmhead.nas)
set(10_day_harib07b_BOOTB ${root_BINARY_DIR}/projects/10_day/harib07b/boot.bin)
set(10_day_harib07b_CCO	  ${root_BINARY_DIR}/projects/10_day/harib07b/boot.o)
set(10_day_harib07b_CCS	  ${root_SOURCE_DIR}/projects/10_day/harib07b/*.c)

add_custom_target(10_day_harib07b_run                                           
  COMMAND ${QEMU} ${QEMUOPT} ${10_day_harib07b_OS}				
)											
add_custom_target(10_day_harib07b_debug						
  COMMAND ${QEMU} -s -S ${QEMUOPT} ${10_day_harib07b_OS} -redir tcp:5555:127.0.0.1:1234 &
)											
add_custom_target(10_day_harib07b_ipl						
  COMMAND ${NASK} ${10_day_harib07b_IPLS} ${10_day_harib07b_IPLB}		
)											
add_custom_target(10_day_harib07b_sys						
  COMMAND ${NASK} ${10_day_harib07b_HEADS} ${10_day_harib07b_HEADB}		
  COMMAND gcc ${10_day_harib07b_CCS}  -c -o ${10_day_harib07b_CCO}	
  COMMAND ld -o ${10_day_harib07b_BOOTB} -e _HariMain ${10_day_harib07b_CCO} ${10_day_harib07b_FUNCO}	
  COMMAND cat ${10_day_harib07b_HEADB} ${10_day_harib07b_BOOTB} > ${10_day_harib07b_SYS}			
)											
add_custom_target(10_day_harib07b_img						
  COMMAND rm -f ${10_day_harib07b_OS}
  COMMAND mformat -f 1440 -C -B ${10_day_harib07b_IPLB} -i ${10_day_harib07b_OS}	
  COMMAND mcopy -n ${10_day_harib07b_SYS} ${10_day_harib07b_OS}			
)											
########### next target ###############
set(asmhead.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/asmhead.nas)
set(asmhead.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead.img)
set(asmhead_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/asmhead_wine.img)

add_custom_target(10_day_harib07b_asmhead
  COMMAND ${root_BINARY_DIR}/src/opennask ${asmhead.img_SRCS} ${asmhead.img_OUTS}
)
add_custom_target(10_day_harib07b_asmhead_wine
  COMMAND ${WINE} ${WINE_NASK} ${asmhead.img_SRCS} ${asmhead_wine.img_OUTS}
)
add_custom_target(10_day_harib07b_asmhead_od
  COMMAND ${OD} -t x1 ${asmhead.img_OUTS}      > ${asmhead.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${asmhead_wine.img_OUTS} > ${asmhead_wine.img_OUTS}_t.txt
)

add_dependencies(images 10_day_harib07b_asmhead)
add_dependencies(wine 10_day_harib07b_asmhead_wine)
add_dependencies(od 10_day_harib07b_asmhead_od)

#----------------------------------------------------------
########### next target ###############
set(ipl10.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/ipl10.nas)
set(ipl10.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/ipl10.img)
set(ipl10_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b/ipl10_wine.img)

add_custom_target(10_day_harib07b_ipl10
  COMMAND ${root_BINARY_DIR}/src/opennask ${ipl10.img_SRCS} ${ipl10.img_OUTS}
)
add_custom_target(10_day_harib07b_ipl10_wine
  COMMAND ${WINE} ${WINE_NASK} ${ipl10.img_SRCS} ${ipl10_wine.img_OUTS}
)
add_custom_target(10_day_harib07b_ipl10_od
  COMMAND ${OD} -t x1 ${ipl10.img_OUTS}      > ${ipl10.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${ipl10_wine.img_OUTS} > ${ipl10_wine.img_OUTS}_t.txt
)

add_dependencies(images 10_day_harib07b_ipl10)
add_dependencies(wine 10_day_harib07b_ipl10_wine)
add_dependencies(od 10_day_harib07b_ipl10_od)

#----------------------------------------------------------
########### next target ###############
set(naskfunc.img_SRCS ${root_SOURCE_DIR}/projects/10_day/harib07b/naskfunc.nas)
set(naskfunc.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b.imgkfunc.img)
set(naskfunc_wine.img_OUTS ${root_BINARY_DIR}/projects/10_day/harib07b_wine.imgkfunc_wine.img)

add_custom_target(10_day_harib07bkfunc
  COMMAND ${root_BINARY_DIR}/src/opennask ${naskfunc.img_SRCS} ${naskfunc.img_OUTS}
)
add_custom_target(10_day_harib07bkfunc_wine
  COMMAND ${WINE} ${WINE_NASK} ${naskfunc.img_SRCS} ${naskfunc_wine.img_OUTS}
)
add_custom_target(10_day_harib07bkfunc_od
  COMMAND ${OD} -t x1 ${naskfunc.img_OUTS}      > ${naskfunc.img_OUTS}_f.txt
  COMMAND ${OD} -t x1 ${naskfunc_wine.img_OUTS} > ${naskfunc_wine.img_OUTS}_t.txt
)

add_dependencies(images 10_day_harib07bkfunc)
add_dependencies(wine 10_day_harib07bkfunc_wine)
add_dependencies(od 10_day_harib07bkfunc_od)

#----------------------------------------------------------
