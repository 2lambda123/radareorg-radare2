/* radare - Apache 2.0 - Copyright 2013 - Adam Pridgen <dso@rice.edu || adam.pridgen@thecoverofnight.com> */
#include <r_anal.h>

#ifndef _INCLUDE_R_ANAL2_H_
#define _INCLUDE_R_ANAL2_H_


enum {
    R_ANAL_EXEC_FMT,
    R_ANAL_DATA_FMT,
    R_ANAL_MIXED_FMT,
};


typedef struct r_anal2_state_type_t {
    ut64 start;
    ut64 end;
    const ut8* buffer;
    ut64 len;
    
    ut64 bytes_consumed;
    ut64 last_addr;

    RList *bbs;
    RHashTable64 *ht;
    ut64 ht_sz;
    
    RAnalFunction *current_fcn;
    RAnalOp *current_op;
    RAnalBlock *current_bb;

    ut8 done;
    void *user_state;
} RAnalInfos;

typedef struct r_anal2_op_to_str_t {
    const char *str;
    ut32 value;
} RAnalOpToStr;


enum {
    R_ANAL2_ILL_OP  =-1,   /* illegal instruction // trap */
    R_ANAL2_NULL_OP = 0,
    R_ANAL2_NOP = 1, /* does nothing */    
    R_ANAL2_STORE_OP  = 0x1 << 20,  // Load or Store memory operation
    R_ANAL2_LOAD_OP   = 0x1 << 21,  // Load or Store memory operation
    R_ANAL2_REG_OP    = 0x1 << 22,  // register operation
    R_ANAL2_OBJ_OP    = 0x1 << 23,  // operates on an object
    R_ANAL2_STACK_OP  = 0x1 << 25,  // stack based operation
    R_ANAL2_BIN_OP    = 0x1 << 26,  // binary operation
    R_ANAL2_CODE_OP   = 0x1 << 27,  // operates on code
    R_ANAL2_DATA_OP   = 0x1 << 28,  // operates on data
    R_ANAL2_UNK_OP  = 0x1 << 29,  /* unknown opcode type */
    R_ANAL2_REP_OP  = 0x1 << 30,  /* repeats next instruction N times */
    R_ANAL2_COND_OP = 0x1 << 31, 
};

enum {
    R_ANAL2_TYPE_REF_NULL  = 0,
    R_ANAL2_TYPE_UNK_REF   = 0x1 << 1, 
    R_ANAL2_TYPE_REF       = 0x1 << 2, 
    R_ANAL2_TYPE_SIGNED    = 0x1 << 3,
    R_ANAL2_TYPE_PRIM      = 0x1 << 4, 
    R_ANAL2_TYPE_CONST     = 0x1 << 5,
    R_ANAL2_TYPE_STATIC    = 0x1 << 6,
    R_ANAL2_TYPE_VOLATILE  = 0x1 << 7,
    R_ANAL2_TYPE_PUBLIC    = 0x1 << 8,

    R_ANAL2_TYPE_BOOL   = 0x1 << 10,
    R_ANAL2_TYPE_BYTE   = 0x1 << 11,
    R_ANAL2_TYPE_SHORT  = 0x1 << 12,
    R_ANAL2_TYPE_INT32  = 0x1 << 13,
    R_ANAL2_TYPE_INT64  = 0x1 << 14, 
    R_ANAL2_TYPE_FLOAT  = 0x1 << 15, 
    R_ANAL2_TYPE_DOUBLE = 0x1 << 16, 
};



// code ops
enum {
    R_ANAL2_CODEOP_JMP    = 0x1 << 1  | R_ANAL2_CODE_OP,/* mandatory jump */
    R_ANAL2_CODEOP_CALL   = 0x1 << 2  | R_ANAL2_CODE_OP,/* call to subroutine (branch+link) */
    R_ANAL2_CODEOP_RET    = 0x1 << 3  | R_ANAL2_CODE_OP,/* returns from subrutine */
    R_ANAL2_CODEOP_TRAP   = 0x1 << 4  | R_ANAL2_CODE_OP,/* it's a trap! */
    R_ANAL2_CODEOP_SWI    = 0x1 << 5  | R_ANAL2_CODE_OP,/* syscall  software interrupt */
    R_ANAL2_CODEOP_IO     = 0x1 << 6  | R_ANAL2_CODE_OP,
    R_ANAL2_CODEOP_LEAVE  = 0x1 << 7  | R_ANAL2_CODE_OP,
    R_ANAL2_CODEOP_SWITCH = 0x1 << 8  | R_ANAL2_CODE_OP,
    R_ANAL2_CODEOP_CJMP   = R_ANAL2_COND_OP | R_ANAL2_CODE_OP | R_ANAL2_CODEOP_JMP,
    R_ANAL2_CODEOP_EOB    = R_ANAL2_CODEOP_JMP | R_ANAL2_CODEOP_RET | R_ANAL2_CODEOP_LEAVE | R_ANAL2_CODEOP_SWITCH,
};

enum {
    // call return types
    R_ANAL2_RET_TYPE_REF_NULL = 0x1 << 10,
    R_ANAL2_RET_TYPE_REF      = 0x1 << 11 ,
    R_ANAL2_RET_TYPE_PRIM     = 0x1 << 12 ,
    R_ANAL2_RET_TYPE_CONST    = 0x1 << 13,
    R_ANAL2_RET_TYPE_STATIC   = 0x1 << 14,
};

// jmp conditionals
enum {
    // TODO these should be mapped to some sort of 
    // flags register
    R_ANAL2_COND_EQ  = 0x1 << 11,
    R_ANAL2_COND_NE  = 0x1 << 12,
    R_ANAL2_COND_GE  = 0x1 << 13,
    R_ANAL2_COND_GT  = 0x1 << 14,
    R_ANAL2_COND_LE  = 0x1 << 15,
    R_ANAL2_COND_LT  = 0x1 << 16,
    R_ANAL2_COND_AL  = 0x1 << 17,
    R_ANAL2_COND_NV  = 0x1 << 18,
    R_ANAL2_COND_NULL  = 0x1 << 19,
};

// bin ops
enum {
    R_ANAL2_BINOP_XCHG = 0x1 << 1 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_CMP  = 0x1 << 2  | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_ADD  = 0x1 << 3  | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_SUB  = 0x1 << 4  | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_MUL  = 0x1 << 6  | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_DIV  = 0x1 << 7  | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_SHR  = 0x1 << 8  | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_SHL  = 0x1 << 9  | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_SAL  = 0x1 << 10 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_SAR  = 0x1 << 11 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_OR   = 0x1 << 12 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_AND  = 0x1 << 14 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_XOR  = 0x1 << 15 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_NOT  = 0x1 << 16 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_MOD  = 0x1 << 17 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_ROR  = 0x1 << 18 | R_ANAL2_BIN_OP,
    R_ANAL2_BINOP_ROL  = 0x1 << 19 | R_ANAL2_BIN_OP,
};


// Object ops
enum {
    R_ANAL2_OBJOP_CAST  = 0x1 << 0 | R_ANAL2_OBJ_OP,
    R_ANAL2_OBJOP_CHECK = 0x1 << 1 | R_ANAL2_OBJ_OP,
    R_ANAL2_OBJOP_NEW   = 0x1 << 2 | R_ANAL2_OBJ_OP,
    R_ANAL2_OBJOP_DEL   = 0x1 << 3 | R_ANAL2_OBJ_OP,
    R_ANAL2_OBJOP_SIZE   = 0x1 << 3 | R_ANAL2_OBJ_OP,
};


// Memory or Data Operations
// Locations of item loaded (base of indirect)
enum {
    R_ANAL2_LDST_FROM_REF   =  0x1 << 1,
    R_ANAL2_LDST_FROM_MEM   =  0x1 << 1,

    R_ANAL2_LDST_FROM_REG   =  0x1 << 2,
    R_ANAL2_LDST_FROM_STACK =  0x1 << 3,
    R_ANAL2_LDST_FROM_CONST =  0x1 << 4,
    R_ANAL2_LDST_FROM_VAR   =  0x1 << 5,

    // If indirect load, where are we getting the indirection,
    R_ANAL2_LDST_INDIRECT_REF  = 0x1 << 6,
    R_ANAL2_LDST_INDIRECT_MEM  = 0x1 << 6,

    R_ANAL2_LDST_INDIRECT_REG   =  0x1 << 7,
    R_ANAL2_LDST_INDIRECT_STACK =  0x1 << 8,
    R_ANAL2_LDST_INDIRECT_IDX   =  0x1 << 9,
    R_ANAL2_LDST_INDIRECT_VAR   =  0x1 << 10,

    // Location to put the item,
    R_ANAL2_LDST_TO_REF  = 0x1 << 11,
    R_ANAL2_LDST_TO_MEM  = 0x1 << 11,

    R_ANAL2_LDST_TO_REG = 0x1 << 12,
    R_ANAL2_LDST_TO_STACK =  0x1 << 13,
    R_ANAL2_LDST_TO_VAR =    0x1 << 14,

    // Stack, Memory, Register, Bss, Data ,
    R_ANAL2_LDST_OP_PUSH  = 0x1 << 15  ,
    R_ANAL2_LDST_OP_POP   = 0x1 << 16,
    R_ANAL2_LDST_OP_MOV   = 0x1 << 17 ,
    R_ANAL2_LDST_OP_EFF_ADDR   = 0x1 << 18,
};

enum {
    R_ANAL2_CODEOP_UCALL = R_ANAL2_UNK_OP | R_ANAL2_CODEOP_CALL,
    R_ANAL2_CODEOP_UJMP  = R_ANAL2_UNK_OP | R_ANAL2_CODEOP_JMP,
    R_ANAL2_LDST_OP_UPUSH = R_ANAL2_UNK_OP | R_ANAL2_LDST_OP_PUSH,
    R_ANAL2_LDST_OP_UPOP  = R_ANAL2_UNK_OP | R_ANAL2_LDST_OP_POP,
};

enum {

	R_ANAL2_LDST_LOAD_FROM_CONST_REF_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_REF |\
                                                     R_ANAL2_LDST_FROM_CONST |\
                                                     R_ANAL2_LDST_TO_STACK |\
                                                     R_ANAL2_TYPE_REF,
                                                     


	R_ANAL2_LDST_LOAD_FROM_CONST_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_CONST |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_LOAD_FROM_CONST_INDIRECT_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_CONST |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_LOAD_FROM_VAR_INDIRECT_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_VAR |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_LOAD_FROM_VAR_INDIRECT_TO_STACK_REF = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_VAR |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_LOAD_FROM_VAR_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_VAR |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_LOAD_FROM_VAR_TO_STACK_REF = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_VAR |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_LOAD_FROM_REF_INDIRECT_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_REF |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_LOAD_FROM_REF_INDIRECT_TO_STACK_REF = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_REF |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_STACK, 

	R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_VAR = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_STORE_OP |\
                                                     R_ANAL2_LDST_FROM_STACK |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_VAR, 

	R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_VAR_REF = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_STORE_OP |\
                                                     R_ANAL2_LDST_FROM_STACK |\
                                                     R_ANAL2_LDST_INDIRECT_IDX |\
                                                     R_ANAL2_LDST_TO_VAR, 

	R_ANAL2_LDST_STORE_FROM_STACK_TO_VAR = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_STORE_OP |\
                                                     R_ANAL2_LDST_FROM_STACK |\
                                                     R_ANAL2_LDST_TO_VAR, 

	R_ANAL2_LDST_STORE_FROM_STACK_TO_VAR_REF = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_STORE_OP |\
                                                     R_ANAL2_LDST_FROM_STACK |\
                                                     R_ANAL2_LDST_TO_VAR, 

	R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_REF = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_STORE_OP |\
                                                     R_ANAL2_LDST_FROM_STACK |\
                                                     R_ANAL2_LDST_TO_REF, 

	R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_REF_REF = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_STORE_OP |\
                                                     R_ANAL2_LDST_FROM_STACK |\
                                                     R_ANAL2_LDST_TO_REF, 

	R_ANAL2_LDST_LOAD_FROM_REF_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                     R_ANAL2_LOAD_OP |\
                                                     R_ANAL2_LDST_FROM_REF |\
                                                     R_ANAL2_LDST_TO_STACK |\
                                                     R_ANAL2_TYPE_PRIM,

	R_ANAL2_LDST_LOAD_FROM_PRIM_VAR_TO_STACK = R_ANAL2_LDST_OP_PUSH |\
                                                   R_ANAL2_LOAD_OP |\
                                                   R_ANAL2_LDST_FROM_VAR |\
                                                   R_ANAL2_TYPE_PRIM,

	R_ANAL2_LDST_LOAD_GET_STATIC = R_ANAL2_LDST_OP_PUSH |\
                                            R_ANAL2_LOAD_OP |\
                                            R_ANAL2_LDST_FROM_REF |\
                                            R_ANAL2_LDST_TO_STACK |\
                                            R_ANAL2_TYPE_REF,

	R_ANAL2_LDST_STORE_PUT_STATIC = R_ANAL2_LDST_OP_POP |\
                                            R_ANAL2_STORE_OP |\
                                            R_ANAL2_LDST_FROM_STACK |\
                                            R_ANAL2_LDST_TO_REF |\
                                            R_ANAL2_TYPE_REF,

	R_ANAL2_LDST_LOAD_GET_FIELD = R_ANAL2_LDST_OP_PUSH |\
                                            R_ANAL2_LOAD_OP |\
                                            R_ANAL2_LDST_FROM_REF |\
                                            R_ANAL2_LDST_TO_STACK |\
                                            R_ANAL2_TYPE_REF,

	R_ANAL2_LDST_STORE_PUT_FIELD = R_ANAL2_LDST_OP_POP |\
                                            R_ANAL2_STORE_OP |\
                                            R_ANAL2_LDST_FROM_STACK |\
                                            R_ANAL2_LDST_TO_REF |\
                                            R_ANAL2_TYPE_REF,
};

// BB and OP 
R_API ut64 r_anal2_map_anal2_to_anal_op_type(ut64 ranal2_op_type);
R_API void r_anal2_bb_to_op(RAnal *anal, RAnalInfos *state, RAnalBlock *bb, RAnalOp *op);
R_API int r_anal2_is_op_type_eop(ut64 x);

R_API ut32 r_anal2_map_anal2_to_anal_bb_type (ut64 ranal2_op_type);

/* by default performs recursive descent, but is anal->analysis_algorithm
 * is present, then that will be the algorithm used for analyzing the code 
 * or data
 */
R_API RList * r_anal2_analyze( RAnal *anal, RAnalInfos *state, ut64 addr);
R_API RList * r_anal2_recursive_descent( RAnal *anal, RAnalInfos *state, ut64 addr);

// BB and OP handling
R_API void r_anal2_update_bb_cfg_head_tail( RAnalBlock *start, RAnalBlock * head, RAnalBlock * tail );
R_API RAnalOp * r_anal2_get_op(RAnal *anal, RAnalInfos *state, ut64 addr);
R_API RAnalBlock * r_anal2_get_bb(RAnal *anal, RAnalInfos *state, ut64 addr);
R_API void r_anal2_clone_op_switch_to_bb (RAnalBlock *bb, RAnalOp *op);



// used to perform comparisons on BB to determine if BB are in same body
R_API int r_anal2_bb_head_comparator(RAnalBlock *a, RAnalBlock *b);
// compare two BB to see if they are equal
R_API int r_anal2_bb_address_comparator(RAnalBlock *a, RAnalBlock *b);



/* 
 * RAnalInfos maintains state during analysis.
 * there are standard values current_fcn, current_op, current_bb, addr, 
 * data buffer, etc. but there is also a void * for user defined structures
 * that can be updated during the callbacks.
 */
R_API RAnalInfos * r_anal2_state_new (ut64 start, ut8* buffer, ut64 len);
R_API void r_anal2_state_insert_bb (RAnalInfos* state, RAnalBlock *bb);
R_API int r_anal2_state_need_rehash (RAnalInfos* state, RAnalBlock *bb);
R_API RAnalBlock * r_anal2_state_search_bb (RAnalInfos* state, ut64 addr);
R_API void r_anal2_state_free (RAnalInfos * state);
R_API ut64 r_anal2_state_get_len (RAnalInfos *state, ut64 addr);
R_API const ut8 * r_anal2_state_get_buf_by_addr (RAnalInfos *state, ut64 addr);
R_API int r_anal2_state_addr_is_valid (RAnalInfos *state, ut64 addr);
R_API void r_anal2_state_merge_bb_list (RAnalInfos *state, RList* bbs);




// Type definitions to strings
#define R_ANAL2_TYPE_REF_NULL_STR "null"
#define R_ANAL2_TYPE_UNK_REF_STR "unk_ref"
#define R_ANAL2_TYPE_REF_STR "ref"
#define R_ANAL2_TYPE_SIGNED_STR "signed"
#define R_ANAL2_TYPE_PRIM_STR "prim"
#define R_ANAL2_TYPE_CONST_STR "const"
#define R_ANAL2_TYPE_STATIC_STR "static"
#define R_ANAL2_TYPE_VOLATILE_STR "volatile"
#define R_ANAL2_TYPE_PUBLIC_STR "public"
#define R_ANAL2_TYPE_BOOL_STR "bool"
#define R_ANAL2_TYPE_BYTE_STR "byte"
#define R_ANAL2_TYPE_SHORT_STR "short"
#define R_ANAL2_TYPE_INT32_STR "int32"
#define R_ANAL2_TYPE_INT64_STR "int64"
#define R_ANAL2_TYPE_FLOAT_STR "float"
#define R_ANAL2_TYPE_DOUBLE_STR "double"


// Conditional Strings
#define R_ANAL2_COND_EQ_STR "=="
#define R_ANAL2_COND_NE_STR "!="
#define R_ANAL2_COND_GE_STR ">="
#define R_ANAL2_COND_GT_STR ">"
#define R_ANAL2_COND_LE_STR "<="
#define R_ANAL2_COND_LT_STR "<"
#define R_ANAL2_COND_AL_STR "==="
#define R_ANAL2_COND_NV_STR "is_zero"
#define R_ANAL2_COND_NULL_STR "is_null"

// Operation types
#define R_ANAL2_ILL_OP_STR    "op_illegal"
#define R_ANAL2_NULL_OP_STR   "op_null"
#define R_ANAL2_NOP_OP_STR    "op_nop"
#define R_ANAL2_STORE_OP_STR  "op_store"
#define R_ANAL2_LOAD_OP_STR   "op_"
#define R_ANAL2_REG_OP_STR    "op_reg"
#define R_ANAL2_OBJ_OP_STR    "op_obj"
#define R_ANAL2_STACK_OP_STR  "op_stack"
#define R_ANAL2_BIN_OP_STR    "op_bin"
#define R_ANAL2_CODE_OP_STR   "op_code"
#define R_ANAL2_DATA_OP_STR   "op_data"
#define R_ANAL2_UNK_OP_STR  "op_unk"
#define R_ANAL2_REP_OP_STR  "op_rep"
#define R_ANAL2_COND_OP_STR "op_cond"

// Code operation Strings
#define R_ANAL2_CODEOP_JMP_STR "jmp"
#define R_ANAL2_CODEOP_CALL_STR "call"
#define R_ANAL2_CODEOP_RET_STR "ret"
#define R_ANAL2_CODEOP_TRAP_STR "trap"
#define R_ANAL2_CODEOP_SWI_STR "swi"
#define R_ANAL2_CODEOP_IO_STR "io"
#define R_ANAL2_CODEOP_LEAVE_STR "leave"
#define R_ANAL2_CODEOP_SWITCH_STR "switch"
#define R_ANAL2_CODEOP_CJMP_STR_STR "cjmp"
#define R_ANAL2_CODEOP_EOB_STR_STR "eob"


// Return Type Strings
#define R_ANAL2_RET_TYPE_REF_NULL_STR "ref_null"
#define R_ANAL2_RET_TYPE_REF_STR "ref"
#define R_ANAL2_RET_TYPE_PRIM_STR "prim"
#define R_ANAL2_RET_TYPE_CONST_STR "const"
#define R_ANAL2_RET_TYPE_STATIC_STR "static"

// Binary operation Strings
#define R_ANAL2_BINOP_XCHG_STR "xchg"
#define R_ANAL2_BINOP_CMP_STR  "cmp"
#define R_ANAL2_BINOP_ADD_STR  "add"
#define R_ANAL2_BINOP_SUB_STR  "sub"
#define R_ANAL2_BINOP_MUL_STR  "mul"
#define R_ANAL2_BINOP_DIV_STR  "div"
#define R_ANAL2_BINOP_SHR_STR  "shr"
#define R_ANAL2_BINOP_SHL_STR  "shl"
#define R_ANAL2_BINOP_SAL_STR  "sal"
#define R_ANAL2_BINOP_SAR_STR  "sar"
#define R_ANAL2_BINOP_OR_STR   "or"
#define R_ANAL2_BINOP_AND_STR  "and"
#define R_ANAL2_BINOP_XOR_STR  "xor"
#define R_ANAL2_BINOP_NOT_STR  "not"
#define R_ANAL2_BINOP_MOD_STR  "mod"
#define R_ANAL2_BINOP_ROR_STR  "ror"
#define R_ANAL2_BINOP_ROL_STR  "rol"

// Object operations strings
#define R_ANAL2_OBJOP_CAST_STR "cast"
#define R_ANAL2_OBJOP_CHECK_STR "typecheck"
#define R_ANAL2_OBJOP_NEW_STR "new"
#define R_ANAL2_OBJOP_DEL_STR "del"
#define R_ANAL2_OBJOP_SIZE_STR "size"

// Load and Store Operations Info Strings
#define R_ANAL2_LDST_FROM_REF_STR "from_ref"
#define R_ANAL2_LDST_FROM_MEM_STR "from mem"
#define R_ANAL2_LDST_FROM_REG_STR "from_reg"
#define R_ANAL2_LDST_FROM_STACK_STR "from_stack"
#define R_ANAL2_LDST_FROM_CONST_STR "from_const"
#define R_ANAL2_LDST_FROM_VAR_STR "from_var"
#define R_ANAL2_LDST_INDIRECT_REF_STR "indirect_ref"
#define R_ANAL2_LDST_INDIRECT_MEM_STR "indirect mem"
#define R_ANAL2_LDST_INDIRECT_REG_STR "indirect_reg"
#define R_ANAL2_LDST_INDIRECT_STACK_STR "indirect_stack"
#define R_ANAL2_LDST_INDIRECT_IDX_STR "indirect_idx"
#define R_ANAL2_LDST_INDIRECT_VAR_STR "indirect_var"
#define R_ANAL2_LDST_TO_REF_STR "to_ref"
#define R_ANAL2_LDST_TO_MEM_STR "to mem"
#define R_ANAL2_LDST_TO_REG_STR "to_reg"
#define R_ANAL2_LDST_TO_STACK_STR "to_stack"
#define R_ANAL2_LDST_TO_VAR_STR "to_var"

#define R_ANAL2_LDST_OP_PUSH_STR "push"
#define R_ANAL2_LDST_OP_POP_STR "pop"
#define R_ANAL2_LDST_OP_TYPE_MOV_STR "mov"
#define R_ANAL2_LDST_OP_TYPE_EFF_ADDR_STR "eff_addr"


// Compound Operation Strings
#define R_ANAL2_LDST_LOAD_FROM_CONST_REF_TO_STACK_STR "load from_const ref to_stack"
#define R_ANAL2_LDST_LOAD_FROM_CONST_TO_STACK_STR "load from_const to_stack"
#define R_ANAL2_LDST_LOAD_FROM_CONST_INDIRECT_TO_STACK_STR "load from_const indirect to_stack"
#define R_ANAL2_LDST_LOAD_FROM_VAR_INDIRECT_TO_STACK_STR "load from_var indirect_idx to stack"
#define R_ANAL2_LDST_LOAD_FROM_VAR_INDIRECT_TO_STACK_REF_STR "load from_var indirect_idx to_stack ref"
#define R_ANAL2_LDST_LOAD_FROM_VAR_TO_STACK_STR "load from_var to_stack"
#define R_ANAL2_LDST_LOAD_FROM_VAR_TO_STACK_REF_STR "load from_var to_stack ref"
#define R_ANAL2_LDST_LOAD_FROM_REF_INDIRECT_TO_STACK_STR "load from_ref indirect_idx to_stack"
#define R_ANAL2_LDST_LOAD_FROM_REF_INDIRECT_TO_STACK_REF_STR "load from_ref indirect_idx to_stack ref"
#define R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_VAR_STR "store from_stack indirect_idx to_var"
#define R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_VAR_REF_STR "store from_stack indirect_idx to_var ref"
#define R_ANAL2_LDST_STORE_FROM_STACK_TO_VAR_STR "store from_stack to_var"
#define R_ANAL2_LDST_STORE_FROM_STACK_TO_VAR_REF_STR "store from_stack to_var ref"
#define R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_REF_STR "store from_stack indirect_idx to_ref"}
#define R_ANAL2_LDST_STORE_FROM_STACK_INDIRECT_TO_REF_REF_STR "store from_stack indirect_idx to_ref ref"}
#define R_ANAL2_LDST_LOAD_FROM_REF_TO_STACK_STR "load from_ref to_stack"
#define R_ANAL2_LDST_LOAD_FROM_PRIM_VAR_TO_STACK_STR "load from_var to_stack"
#define R_ANAL2_LDST_LOAD_GET_STATIC_STR "load from_ref to_stack ref"
#define R_ANAL2_LDST_STORE_PUT_STATIC_STR "store from_stack to_ref"
#define R_ANAL2_LDST_LOAD_GET_FIELD_STR "load from_ref to_stack ref"
#define R_ANAL2_LDST_STORE_PUT_FIELD_STR "load from_ref to_stack ref"




#endif


