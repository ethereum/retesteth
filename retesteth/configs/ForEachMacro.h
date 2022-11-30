#pragma once
//https://stackoverflow.com/questions/14732803/preprocessor-variadic-for-each-macro-compatible-with-msvc10

#define internal_MY_EXPAND(x) x
#define internal_FOR_EACH_1(what, x, ...) what(x)
#define internal_FOR_EACH_2(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_1(what, __VA_ARGS__))
#define internal_FOR_EACH_3(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_2(what, __VA_ARGS__))
#define internal_FOR_EACH_4(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_3(what, __VA_ARGS__))
#define internal_FOR_EACH_5(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_4(what, __VA_ARGS__))
#define internal_FOR_EACH_6(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_5(what, __VA_ARGS__))
#define internal_FOR_EACH_7(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_6(what, __VA_ARGS__))
#define internal_FOR_EACH_8(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_7(what, __VA_ARGS__))
#define internal_FOR_EACH_9(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_8(what, __VA_ARGS__))
#define internal_FOR_EACH_10(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_9(what, __VA_ARGS__))
#define internal_FOR_EACH_11(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_10(what, __VA_ARGS__))
#define internal_FOR_EACH_12(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_11(what, __VA_ARGS__))
#define internal_FOR_EACH_13(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_12(what, __VA_ARGS__))
#define internal_FOR_EACH_14(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_13(what, __VA_ARGS__))
#define internal_FOR_EACH_15(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_14(what, __VA_ARGS__))
#define internal_FOR_EACH_16(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_15(what, __VA_ARGS__))
#define internal_FOR_EACH_17(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_16(what, __VA_ARGS__))
#define internal_FOR_EACH_18(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_17(what, __VA_ARGS__))
#define internal_FOR_EACH_19(what, x, ...) what(x);internal_MY_EXPAND(internal_FOR_EACH_18(what, __VA_ARGS__))
#define internal_FOR_EACH_NARG(...) internal_FOR_EACH_NARG_(__VA_ARGS__, internal_FOR_EACH_RSEQ_N())
#define internal_FOR_EACH_NARG_(...) internal_MY_EXPAND(internal_FOR_EACH_ARG_N(__VA_ARGS__))
#define internal_FOR_EACH_ARG_N(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19, N, ...) N
#define internal_FOR_EACH_RSEQ_N() 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
#define internal_MY_CONCATENATE(x,y) x##y
#define internal_FOR_EACH_(N, what, ...) internal_MY_EXPAND(internal_MY_CONCATENATE(internal_FOR_EACH_, N)(what, __VA_ARGS__))
#define FOR_EACH(what, ...) internal_FOR_EACH_(internal_FOR_EACH_NARG(__VA_ARGS__), what, __VA_ARGS__)
