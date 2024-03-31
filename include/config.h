#pragma once
#include "QuBLAS.h"



using TxSymbol_t = apFixed<intBits<9>,fracBits<9>>;
using RxSymbol_t = apFixed<intBits<9>,fracBits<9>>;
using Nv_t = apFixed<intBits<9>,fracBits<9>>;
using H_t = apFixed<intBits<9>,fracBits<9>>;
using HtH_t = apFixed<intBits<9>,fracBits<9>>;
using HtY_t = apFixed<intBits<9>,fracBits<9>>;


using QgemulAdd_t = apFixed<intBits<9>,fracBits<9>>;
using QgemulMul_t = apFixed<intBits<9>,fracBits<9>>;


using QgemvAdd_t = apFixed<intBits<9>,fracBits<9>>;
using QgemvMul_t = apFixed<intBits<9>,fracBits<9>>;

using QgetrfDiv_t = apFixed<intBits<9>,fracBits<9>>;
using QgetrfSub_t = apFixed<intBits<9>,fracBits<9>>;
using QgetrfMul_t = apFixed<intBits<9>,fracBits<9>>;

using QgetrsDiv_t = apFixed<intBits<9>,fracBits<9>>;
using QgetrsSub_t = apFixed<intBits<9>,fracBits<9>>;
using QgetrsMul_t = apFixed<intBits<9>,fracBits<9>>;