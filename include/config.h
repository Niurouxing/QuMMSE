#pragma once
#include "QuBLAS.h"



using RxSymbol_t = apFixed<intBits<8>,fracBits<4>>;
using Nv_t = apFixed<intBits<9>,fracBits<9>>;
using H_t = apFixed<intBits<4>,fracBits<4>>;
using HtH_t = apFixed<intBits<7>,fracBits<10>>;
using HtY_t = apFixed<intBits<10>,fracBits<6>>;


using QgemulAdd_t = apFixed<intBits<9>,fracBits<9>>;
using QgemulMul_t = apFixed<intBits<8>,fracBits<9>>;


using QgemvAdd_t = apFixed<intBits<8>,fracBits<8>>;
using QgemvMul_t = apFixed<intBits<10>,fracBits<10>>;

using QgetrfDiv_t = apFixed<intBits<4>,fracBits<9>>;
using QgetrfSub_t = apFixed<intBits<9>,fracBits<6>>;
using QgetrfMul_t = apFixed<intBits<5>,fracBits<8>>;

using QgetrsDiv_t = apFixed<intBits<8>,fracBits<5>>;
using QgetrsSub_t = apFixed<intBits<7>,fracBits<6>>;
using QgetrsMul_t = apFixed<intBits<7>,fracBits<6>>;