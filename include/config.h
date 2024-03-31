#pragma once
#include "QuBLAS.h"

static constexpr int TxAntNum = 4;
static constexpr int RxAntNum = 8;
static constexpr int ModType = 4;
static constexpr double SNRdB = 30;


using TxSymbol_t = apFixed<intBits<12>,fracBits<12>>;
using RxSymbol_t = apFixed<intBits<12>,fracBits<12>>;
using Nv_t = apFixed<intBits<12>,fracBits<12>>;
using H_t = apFixed<intBits<12>,fracBits<12>>;
using HtH_t = apFixed<intBits<12>,fracBits<12>>;
using HtY_t = apFixed<intBits<12>,fracBits<12>>;


using QgemulAdd_t = apFixed<intBits<12>,fracBits<12>>;
using QgemulMul_t = apFixed<intBits<12>,fracBits<12>>;


using QgemvAdd_t = apFixed<intBits<12>,fracBits<12>>;
using QgemvMul_t = apFixed<intBits<12>,fracBits<12>>;

using QgetrfDiv_t = apFixed<intBits<12>,fracBits<12>>;
using QgetrfSub_t = apFixed<intBits<12>,fracBits<12>>;
using QgetrfMul_t = apFixed<intBits<12>,fracBits<12>>;

using QgetrsDiv_t = apFixed<intBits<12>,fracBits<12>>;
using QgetrsSub_t = apFixed<intBits<12>,fracBits<12>>;
using QgetrsMul_t = apFixed<intBits<12>,fracBits<12>>;