OBJ_S390X=bp_s390x.o

STATIC_OBJ+=${OBJ_S390X}
TARGET_S390X=bp_s390x.${EXT_SO}

ALL_TARGETS+=${TARGET_S390X}

${TARGET_S390X}: ${OBJ_S390X}
	${CC} $(call libname,bp_s390x) ${CFLAGS} -o ${TARGET_S390X} ${OBJ_S390X}
