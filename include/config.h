#pragma once
#include "QuBLAS.h"

using RxSymbol_t = Qu<intBits<12>, fracBits<12>>;
using Nv_t = Qu<intBits<11>, fracBits<10>>;
using H_t = Qu<intBits<11>, fracBits<12>>;
using HtH_diag_t = Qu<intBits<12>, fracBits<11>>;
using HtH_off_diag_t = Qu<intBits<13>, fracBits<11>>;
using HtY_t = Qu<intBits<10>, fracBits<12>>;

using QgramulAdd_off_diag_1_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_off_diag_2_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_off_diag_3_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_off_diag_4_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_off_diag_5_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_off_diag_6_t = Qu<intBits<12>, fracBits<11>>;

using QgramulAddOffDiagList = TypeList<QgramulAdd_off_diag_1_t, QgramulAdd_off_diag_2_t, QgramulAdd_off_diag_3_t, QgramulAdd_off_diag_4_t, QgramulAdd_off_diag_5_t, QgramulAdd_off_diag_6_t>;

using QgemulOffDiagMul_t = Qu<intBits<12>, fracBits<12>>;

using QgramulAdd_diag_1_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_diag_2_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_diag_3_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_diag_4_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_diag_5_t = Qu<intBits<12>, fracBits<11>>;
using QgramulAdd_diag_6_t = Qu<intBits<12>, fracBits<11>>;

using QgramulAddDiagList = TypeList<QgramulAdd_diag_1_t, QgramulAdd_diag_2_t, QgramulAdd_diag_3_t, QgramulAdd_diag_4_t, QgramulAdd_diag_5_t, QgramulAdd_diag_6_t>;

using QgemulDiagMul_t = Qu<intBits<12>, fracBits<12>>;

using QgemvAdd_1_t = Qu<intBits<12>, fracBits<12>>;
using QgemvAdd_2_t = Qu<intBits<12>, fracBits<12>>;
using QgemvAdd_3_t = Qu<intBits<12>, fracBits<12>>;
using QgemvAdd_4_t = Qu<intBits<12>, fracBits<12>>;
using QgemvAdd_5_t = Qu<intBits<12>, fracBits<12>>;
using QgemvAdd_6_t = Qu<intBits<12>, fracBits<12>>;

using QgemvAddList = TypeList<QgemvAdd_1_t, QgemvAdd_2_t, QgemvAdd_3_t, QgemvAdd_4_t, QgemvAdd_5_t, QgemvAdd_6_t>;
using QgemvMul_t = Qu<intBits<12>, fracBits<12>>;
