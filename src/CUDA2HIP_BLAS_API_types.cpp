/*
Copyright (c) 2015 - present Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "CUDA2HIP.h"

// Map of all functions
const std::map<llvm::StringRef, hipCounter> CUDA_BLAS_TYPE_NAME_MAP {
  // Blas defines
  {"CUBLAS_VER_MAJOR",               {"HIPBLAS_VER_MAJOR",               "",                                      CONV_DEFINE, API_BLAS, 2, UNSUPPORTED}},
  {"CUBLAS_VER_MINOR",               {"HIPBLAS_VER_MINOR",               "",                                      CONV_DEFINE, API_BLAS, 2, UNSUPPORTED}},
  {"CUBLAS_VER_PATCH",               {"HIPBLAS_VER_PATCH",               "",                                      CONV_DEFINE, API_BLAS, 2, UNSUPPORTED}},
  {"CUBLAS_VER_BUILD",               {"HIPBLAS_VER_BUILD",               "",                                      CONV_DEFINE, API_BLAS, 2, UNSUPPORTED}},
  {"CUBLAS_VERSION",                 {"HIPBLAS_VERSION",                 "",                                      CONV_DEFINE, API_BLAS, 2, UNSUPPORTED}},

  // Blas operations
  {"cublasOperation_t",              {"hipblasOperation_t",              "rocblas_operation",                     CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_OP_N",                    {"HIPBLAS_OP_N",                    "rocblas_operation_none",                CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_OP_T",                    {"HIPBLAS_OP_T",                    "rocblas_operation_transpose",           CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_OP_C",                    {"HIPBLAS_OP_C",                    "rocblas_operation_conjugate_transpose", CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_OP_HERMITAN",             {"HIPBLAS_OP_C",                    "rocblas_operation_conjugate_transpose", CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_OP_CONJG",                {"HIPBLAS_OP_CONJG",                "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},

  // Blas statuses
  {"cublasStatus",                   {"hipblasStatus_t",                 "rocblas_status",                        CONV_TYPE, API_BLAS, 2}},
  {"cublasStatus_t",                 {"hipblasStatus_t",                 "rocblas_status",                        CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_STATUS_SUCCESS",          {"HIPBLAS_STATUS_SUCCESS",          "rocblas_status_success",                CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_NOT_INITIALIZED",  {"HIPBLAS_STATUS_NOT_INITIALIZED",  "rocblas_status_invalid_handle",         CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_ALLOC_FAILED",     {"HIPBLAS_STATUS_ALLOC_FAILED",     "rocblas_status_not_implemented",        CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_INVALID_VALUE",    {"HIPBLAS_STATUS_INVALID_VALUE",    "rocblas_status_invalid_pointer",        CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_MAPPING_ERROR",    {"HIPBLAS_STATUS_MAPPING_ERROR",    "rocblas_status_invalid_size",           CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_EXECUTION_FAILED", {"HIPBLAS_STATUS_EXECUTION_FAILED", "rocblas_status_memory_error",           CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_INTERNAL_ERROR",   {"HIPBLAS_STATUS_INTERNAL_ERROR",   "rocblas_status_internal_error",         CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_NOT_SUPPORTED",    {"HIPBLAS_STATUS_NOT_SUPPORTED",    "rocblas_status_perf_degraded",          CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_ARCH_MISMATCH",    {"HIPBLAS_STATUS_ARCH_MISMATCH",    "rocblas_status_size_query_mismatch",    CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_STATUS_LICENSE_ERROR",    {"HIPBLAS_STATUS_UNKNOWN",          "rocblas_status_not_implemented",        CONV_NUMERIC_LITERAL, API_BLAS, 2, ROC_UNSUPPORTED}},

  // Blas Fill Modes
  {"cublasFillMode_t",               {"hipblasFillMode_t",               "rocblas_fill",                          CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_FILL_MODE_LOWER",         {"HIPBLAS_FILL_MODE_LOWER",         "rocblas_fill_lower",                    CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_FILL_MODE_UPPER",         {"HIPBLAS_FILL_MODE_UPPER",         "rocblas_fill_upper",                    CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_FILL_MODE_FULL",          {"HIPBLAS_FILL_MODE_FULL",          "rocblas_fill_full",                     CONV_NUMERIC_LITERAL, API_BLAS, 2}},

  // Blas Diag Types
  {"cublasDiagType_t",               {"hipblasDiagType_t",               "rocblas_diagonal",                      CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_DIAG_NON_UNIT",           {"HIPBLAS_DIAG_NON_UNIT",           "rocblas_diagonal_non_unit",             CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_DIAG_UNIT",               {"HIPBLAS_DIAG_UNIT",               "rocblas_diagonal_unit",                 CONV_NUMERIC_LITERAL, API_BLAS, 2}},

  // Blas Side Modes
  {"cublasSideMode_t",               {"hipblasSideMode_t",               "rocblas_side",                          CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_SIDE_LEFT",               {"HIPBLAS_SIDE_LEFT",               "rocblas_side_left",                     CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_SIDE_RIGHT",              {"HIPBLAS_SIDE_RIGHT",              "rocblas_side_right",                    CONV_NUMERIC_LITERAL, API_BLAS, 2}},

  // Blas Pointer Modes
  {"cublasPointerMode_t",            {"hipblasPointerMode_t",            "rocblas_pointer_mode",                  CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_POINTER_MODE_HOST",       {"HIPBLAS_POINTER_MODE_HOST",       "rocblas_pointer_mode_host",             CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_POINTER_MODE_DEVICE",     {"HIPBLAS_POINTER_MODE_DEVICE",     "rocblas_pointer_mode_device",           CONV_NUMERIC_LITERAL, API_BLAS, 2}},

  // Blas Atomics Modes
  {"cublasAtomicsMode_t",            {"hipblasAtomicsMode_t",            "rocblas_atomics_mode",                  CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_ATOMICS_NOT_ALLOWED",     {"HIPBLAS_ATOMICS_NOT_ALLOWED",     "rocblas_atomics_not_allowed",           CONV_NUMERIC_LITERAL, API_BLAS, 2}},
  {"CUBLAS_ATOMICS_ALLOWED",         {"HIPBLAS_ATOMICS_ALLOWED",         "rocblas_atomics_allowed",               CONV_NUMERIC_LITERAL, API_BLAS, 2}},

  // Blas Data Type
  {"cublasDataType_t",               {"hipblasDatatype_t",               "rocblas_datatype",                      CONV_TYPE, API_BLAS, 2}},

  // Blas Math mode/tensor operation
  {"cublasMath_t",                                     {"hipblasMath_t",                                     "",  CONV_TYPE, API_BLAS, 2, UNSUPPORTED}},
  {"CUBLAS_DEFAULT_MATH",                              {"HIPBLAS_DEFAULT_MATH",                              "",  CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 0
  {"CUBLAS_TENSOR_OP_MATH",                            {"HIPBLAS_TENSOR_OP_MATH",                            "",  CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED | CUDA_DEPRECATED}}, // 1
  {"CUBLAS_PEDANTIC_MATH",                             {"HIPBLAS_PEDANTIC_MATH",                             "",  CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 2
  {"CUBLAS_TF32_TENSOR_OP_MATH",                       {"HIPBLAS_TF32_TENSOR_OP_MATH",                       "",  CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 3
  {"CUBLAS_MATH_DISALLOW_REDUCED_PRECISION_REDUCTION", {"HIPBLAS_MATH_DISALLOW_REDUCED_PRECISION_REDUCTION", "",  CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 16

  // Blass different GEMM algorithms
  {"cublasGemmAlgo_t",               {"hipblasGemmAlgo_t",               "rocblas_gemm_algo",                     CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_GEMM_DFALT",              {"HIPBLAS_GEMM_DEFAULT",            "rocblas_gemm_algo_standard",            CONV_NUMERIC_LITERAL, API_BLAS, 2}},  //  -1 // 160 // 0b0000000000
  {"CUBLAS_GEMM_DEFAULT",            {"HIPBLAS_GEMM_DEFAULT",            "rocblas_gemm_algo_standard",            CONV_NUMERIC_LITERAL, API_BLAS, 2}},  //  -1 // 160 // 0b0000000000
  {"CUBLAS_GEMM_ALGO0",              {"HIPBLAS_GEMM_ALGO0",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   0
  {"CUBLAS_GEMM_ALGO1",              {"HIPBLAS_GEMM_ALGO1",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   1
  {"CUBLAS_GEMM_ALGO2",              {"HIPBLAS_GEMM_ALGO2",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   2
  {"CUBLAS_GEMM_ALGO3",              {"HIPBLAS_GEMM_ALGO3",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   3
  {"CUBLAS_GEMM_ALGO4",              {"HIPBLAS_GEMM_ALGO4",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   4
  {"CUBLAS_GEMM_ALGO5",              {"HIPBLAS_GEMM_ALGO5",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   5
  {"CUBLAS_GEMM_ALGO6",              {"HIPBLAS_GEMM_ALGO6",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   6
  {"CUBLAS_GEMM_ALGO7",              {"HIPBLAS_GEMM_ALGO7",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   7
  {"CUBLAS_GEMM_ALGO8",              {"HIPBLAS_GEMM_ALGO8",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   8
  {"CUBLAS_GEMM_ALGO9",              {"HIPBLAS_GEMM_ALGO9",              "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //   9
  {"CUBLAS_GEMM_ALGO10",             {"HIPBLAS_GEMM_ALGO10",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  10
  {"CUBLAS_GEMM_ALGO11",             {"HIPBLAS_GEMM_ALGO11",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  11
  {"CUBLAS_GEMM_ALGO12",             {"HIPBLAS_GEMM_ALGO12",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  12
  {"CUBLAS_GEMM_ALGO13",             {"HIPBLAS_GEMM_ALGO13",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  13
  {"CUBLAS_GEMM_ALGO14",             {"HIPBLAS_GEMM_ALGO14",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  14
  {"CUBLAS_GEMM_ALGO15",             {"HIPBLAS_GEMM_ALGO15",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  15
  {"CUBLAS_GEMM_ALGO16",             {"HIPBLAS_GEMM_ALGO16",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  16
  {"CUBLAS_GEMM_ALGO17",             {"HIPBLAS_GEMM_ALGO17",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  17
  {"CUBLAS_GEMM_ALGO18",             {"HIPBLAS_GEMM_ALGO18",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  18
  {"CUBLAS_GEMM_ALGO19",             {"HIPBLAS_GEMM_ALGO19",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  19
  {"CUBLAS_GEMM_ALGO20",             {"HIPBLAS_GEMM_ALGO20",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  20
  {"CUBLAS_GEMM_ALGO21",             {"HIPBLAS_GEMM_ALGO21",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  21
  {"CUBLAS_GEMM_ALGO22",             {"HIPBLAS_GEMM_ALGO22",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  22
  {"CUBLAS_GEMM_ALGO23",             {"HIPBLAS_GEMM_ALGO23",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  23
  {"CUBLAS_GEMM_DEFAULT_TENSOR_OP",  {"HIPBLAS_GEMM_DEFAULT_TENSOR_OP",  "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  99
  {"CUBLAS_GEMM_DFALT_TENSOR_OP",    {"HIPBLAS_GEMM_DFALT_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  //  99
  {"CUBLAS_GEMM_ALGO0_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO0_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 100
  {"CUBLAS_GEMM_ALGO1_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO1_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 101
  {"CUBLAS_GEMM_ALGO2_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO2_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 102
  {"CUBLAS_GEMM_ALGO3_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO3_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 103
  {"CUBLAS_GEMM_ALGO4_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO4_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 104
  {"CUBLAS_GEMM_ALGO5_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO5_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 105
  {"CUBLAS_GEMM_ALGO6_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO6_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 106
  {"CUBLAS_GEMM_ALGO7_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO7_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 107
  {"CUBLAS_GEMM_ALGO8_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO8_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 108
  {"CUBLAS_GEMM_ALGO9_TENSOR_OP",    {"HIPBLAS_GEMM_ALGO9_TENSOR_OP",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 109
  {"CUBLAS_GEMM_ALGO10_TENSOR_OP",   {"HIPBLAS_GEMM_ALGO10_TENSOR_OP",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 110
  {"CUBLAS_GEMM_ALGO11_TENSOR_OP",   {"HIPBLAS_GEMM_ALGO11_TENSOR_OP",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 111
  {"CUBLAS_GEMM_ALGO12_TENSOR_OP",   {"HIPBLAS_GEMM_ALGO12_TENSOR_OP",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 112
  {"CUBLAS_GEMM_ALGO13_TENSOR_OP",   {"HIPBLAS_GEMM_ALGO13_TENSOR_OP",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 113
  {"CUBLAS_GEMM_ALGO14_TENSOR_OP",   {"HIPBLAS_GEMM_ALGO14_TENSOR_OP",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 114
  {"CUBLAS_GEMM_ALGO15_TENSOR_OP",   {"HIPBLAS_GEMM_ALGO15_TENSOR_OP",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}},  // 115

  // TODO: rename hipblasDatatype_t to hipDataType_t and move from hipBLAS to HIP
  {"cudaDataType_t",                 {"hipblasDatatype_t",               "rocblas_datatype_",                     CONV_TYPE, API_BLAS, 3}},
  {"cudaDataType",                   {"hipblasDatatype_t",               "rocblas_datatype",                      CONV_TYPE, API_BLAS, 3}},
  {"CUDA_R_16F",                     {"HIPBLAS_R_16F",                   "rocblas_datatype_f16_r",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  2 // 150
  {"CUDA_C_16F",                     {"HIPBLAS_C_16F",                   "rocblas_datatype_f16_c",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  6 // 153
  {"CUDA_R_32F",                     {"HIPBLAS_R_32F",                   "rocblas_datatype_f32_r",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  0 // 151
  {"CUDA_C_32F",                     {"HIPBLAS_C_32F",                   "rocblas_datatype_f32_c",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  4 // 154
  {"CUDA_R_64F",                     {"HIPBLAS_R_64F",                   "rocblas_datatype_f64_r",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  1 // 152
  {"CUDA_C_64F",                     {"HIPBLAS_C_64F",                   "rocblas_datatype_f64_c",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  5 // 155
  {"CUDA_R_8I",                      {"HIPBLAS_R_8I",                    "rocblas_datatype_i8_r",                 CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  3 // 160
  {"CUDA_C_8I",                      {"HIPBLAS_C_8I",                    "rocblas_datatype_i8_c",                 CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  7 // 164
  {"CUDA_R_8U",                      {"HIPBLAS_R_8U",                    "rocblas_datatype_u8_r",                 CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  8 // 161
  {"CUDA_C_8U",                      {"HIPBLAS_C_8U",                    "rocblas_datatype_u8_c",                 CONV_NUMERIC_LITERAL, API_BLAS, 3}}, //  9 // 165
  {"CUDA_R_32I",                     {"HIPBLAS_R_32I",                   "rocblas_datatype_i32_r",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, // 10 // 162
  {"CUDA_C_32I",                     {"HIPBLAS_C_32I",                   "rocblas_datatype_i32_c",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, // 11 // 166
  {"CUDA_R_32U",                     {"HIPBLAS_R_32U",                   "rocblas_datatype_u32_r",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, // 12 // 163
  {"CUDA_C_32U",                     {"HIPBLAS_C_32U",                   "rocblas_datatype_u32_c",                CONV_NUMERIC_LITERAL, API_BLAS, 3}}, // 13 // 167
  {"CUDA_R_16BF",                    {"HIPBLAS_R_16B",                   "rocblas_datatype_bf16_r",               CONV_NUMERIC_LITERAL, API_BLAS, 3}}, // 14 // 168
  {"CUDA_C_16BF",                    {"HIPBLAS_C_16B",                   "rocblas_datatype_bf16_c",               CONV_NUMERIC_LITERAL, API_BLAS, 3}}, // 15 // 169
  {"CUDA_R_4I",                      {"HIPBLAS_R_4I",                    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 16
  {"CUDA_C_4I",                      {"HIPBLAS_C_4I",                    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 17
  {"CUDA_R_4U",                      {"HIPBLAS_R_4U",                    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 18
  {"CUDA_C_4U",                      {"HIPBLAS_C_4U",                    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 19
  {"CUDA_R_16I",                     {"HIPBLAS_R_16I",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 20
  {"CUDA_C_16I",                     {"HIPBLAS_C_16I",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 21
  {"CUDA_R_16U",                     {"HIPBLAS_R_16U",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 22
  {"CUDA_C_16U",                     {"HIPBLAS_C_16U",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 23
  {"CUDA_R_64I",                     {"HIPBLAS_R_64I",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 24
  {"CUDA_C_64I",                     {"HIPBLAS_C_64I",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 25
  {"CUDA_R_64U",                     {"HIPBLAS_R_64U",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 26
  {"CUDA_C_64U",                     {"HIPBLAS_C_64U",                   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 27
  {"CUDA_R_8F_E4M3",                 {"HIPBLAS_R_8F_E4M3",               "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 28
  {"CUDA_R_8F_E5M2",                 {"HIPBLAS_R_8F_E5M2",               "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 3, UNSUPPORTED}}, // 29

  {"cublasHandle_t",                 {"hipblasHandle_t",                 "rocblas_handle",                        CONV_TYPE, API_BLAS, 2}},
  // TODO: dereferencing: typedef struct cublasContext *cublasHandle_t;
  {"cublasContext",                  {"hipblasContext",                  "_rocblas_handle",                       CONV_TYPE, API_BLAS, 2, HIP_UNSUPPORTED}},

  // NOTE: renamed UNSUPPORTED hipblasComputeType_t to the HIP supported hipblasDatatype_t (workaround)
  // TODO: change the type to the correct one after fixing https://github.com/ROCmSoftwarePlatform/hipBLAS/issues/529
  {"cublasComputeType_t",            {"hipblasDatatype_t",               "",                                      CONV_TYPE, API_BLAS, 2}},
  {"CUBLAS_COMPUTE_16F",             {"HIPBLAS_COMPUTE_16F",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 64
  {"CUBLAS_COMPUTE_16F_PEDANTIC",    {"HIPBLAS_COMPUTE_16F_PEDANTIC",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 65
  {"CUBLAS_COMPUTE_32F",             {"HIPBLAS_COMPUTE_32F",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 68
  {"CUBLAS_COMPUTE_32F_PEDANTIC",    {"HIPBLAS_COMPUTE_32F_PEDANTIC",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 69
  {"CUBLAS_COMPUTE_32F_FAST_16F",    {"HIPBLAS_COMPUTE_32F_FAST_16F",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 74
  {"CUBLAS_COMPUTE_32F_FAST_16BF",   {"HIPBLAS_COMPUTE_32F_FAST_16BF",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 75
  {"CUBLAS_COMPUTE_32F_FAST_TF32",   {"HIPBLAS_COMPUTE_32F_FAST_TF32",   "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 77
  {"CUBLAS_COMPUTE_64F",             {"HIPBLAS_COMPUTE_64F",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 70
  {"CUBLAS_COMPUTE_64F_PEDANTIC",    {"HIPBLAS_COMPUTE_64F_PEDANTIC",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 71
  {"CUBLAS_COMPUTE_32I",             {"HIPBLAS_COMPUTE_32I",             "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 72
  {"CUBLAS_COMPUTE_32I_PEDANTIC",    {"HIPBLAS_COMPUTE_32I_PEDANTIC",    "",                                      CONV_NUMERIC_LITERAL, API_BLAS, 2, UNSUPPORTED}}, // 73
};

const std::map<llvm::StringRef, cudaAPIversions> CUDA_BLAS_TYPE_NAME_VER_MAP {
  {"CUBLAS_VER_MAJOR",                                 {CUDA_101, CUDA_0, CUDA_0}},
  {"CUBLAS_VER_MINOR",                                 {CUDA_101, CUDA_0, CUDA_0}},
  {"CUBLAS_VER_PATCH",                                 {CUDA_101, CUDA_0, CUDA_0}},
  {"CUBLAS_VER_BUILD",                                 {CUDA_102, CUDA_0, CUDA_0}},
  {"CUBLAS_VERSION",                                   {CUDA_101, CUDA_0, CUDA_0}},
  {"CUBLAS_OP_CONJG",                                  {CUDA_101, CUDA_0, CUDA_0}},
  {"CUBLAS_OP_HERMITAN",                               {CUDA_101, CUDA_0, CUDA_0}},
  {"CUBLAS_FILL_MODE_FULL",                            {CUDA_101, CUDA_0, CUDA_0}},
  {"cublasDataType_t",                                 {CUDA_75,  CUDA_0, CUDA_0}},
  {"cublasMath_t",                                     {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_DEFAULT_MATH",                              {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_TENSOR_OP_MATH",                            {CUDA_90,  CUDA_110, CUDA_0}},
  {"CUBLAS_PEDANTIC_MATH",                             {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_TF32_TENSOR_OP_MATH",                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_MATH_DISALLOW_REDUCED_PRECISION_REDUCTION", {CUDA_110, CUDA_0, CUDA_0}},
  {"cublasGemmAlgo_t",                                 {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_DFALT",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_DEFAULT",                              {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO0",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO1",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO2",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO3",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO4",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO5",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO6",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO7",                                {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO15_TENSOR_OP",                     {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO14_TENSOR_OP",                     {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO13_TENSOR_OP",                     {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO12_TENSOR_OP",                     {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO11_TENSOR_OP",                     {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO10_TENSOR_OP",                     {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO9_TENSOR_OP",                      {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO8_TENSOR_OP",                      {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO7_TENSOR_OP",                      {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO6_TENSOR_OP",                      {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO5_TENSOR_OP",                      {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO4_TENSOR_OP",                      {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO3_TENSOR_OP",                      {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO2_TENSOR_OP",                      {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO1_TENSOR_OP",                      {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO0_TENSOR_OP",                      {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_DFALT_TENSOR_OP",                      {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_DEFAULT_TENSOR_OP",                    {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO23",                               {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO22",                               {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO21",                               {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO20",                               {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO19",                               {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO18",                               {CUDA_92,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO17",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO16",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO15",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO14",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO13",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO12",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO11",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO10",                               {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO9",                                {CUDA_90,  CUDA_0, CUDA_0}},
  {"CUBLAS_GEMM_ALGO8",                                {CUDA_90,  CUDA_0, CUDA_0}},
  {"cudaDataType_t",                                   {CUDA_80,  CUDA_0, CUDA_0}},
  {"cudaDataType",                                     {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_R_16F",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_C_16F",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_R_32F",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_C_32F",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_R_64F",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_C_64F",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_R_8I",                                        {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_C_8I",                                        {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_R_8U",                                        {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_C_8U",                                        {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_R_32I",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_C_32I",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_R_32U",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"CUDA_C_32U",                                       {CUDA_80,  CUDA_0, CUDA_0}},
  {"cublasComputeType_t",                              {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_16F",                               {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_16F_PEDANTIC",                      {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_32F",                               {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_32F_PEDANTIC",                      {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_32F_FAST_16F",                      {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_32F_FAST_16BF",                     {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_32F_FAST_TF32",                     {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_64F",                               {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_64F_PEDANTIC",                      {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_32I",                               {CUDA_110, CUDA_0, CUDA_0}},
  {"CUBLAS_COMPUTE_32I_PEDANTIC",                      {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_R_4I",                                        {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_C_4I",                                        {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_R_4U",                                        {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_C_4U",                                        {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_R_16I",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_C_16I",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_R_16U",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_C_16U",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_R_64I",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_C_64I",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_R_64U",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_C_64U",                                       {CUDA_110, CUDA_0, CUDA_0}},
  {"CUDA_R_8F_E4M3",                                   {CUDA_118, CUDA_0, CUDA_0}},
  {"CUDA_R_8F_E5M2",                                   {CUDA_118, CUDA_0, CUDA_0}},
};

const std::map<llvm::StringRef, hipAPIversions> HIP_BLAS_TYPE_NAME_VER_MAP {
  {"hipblasOperation_t",                               {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_OP_N",                                     {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_OP_T",                                     {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_OP_C",                                     {HIP_1082, HIP_0,    HIP_0   }},
  {"hipblasGemmAlgo_t",                                {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_GEMM_DEFAULT",                             {HIP_1082, HIP_0,    HIP_0   }},
  {"hipblasStatus_t",                                  {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_SUCCESS",                           {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_NOT_INITIALIZED",                   {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_ALLOC_FAILED",                      {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_INVALID_VALUE",                     {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_MAPPING_ERROR",                     {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_EXECUTION_FAILED",                  {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_INTERNAL_ERROR",                    {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_NOT_SUPPORTED",                     {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_STATUS_ARCH_MISMATCH",                     {HIP_1082, HIP_0,    HIP_0   }},
  {"hipblasFillMode_t",                                {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_FILL_MODE_LOWER",                          {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_FILL_MODE_UPPER",                          {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_FILL_MODE_FULL",                           {HIP_1082, HIP_0,    HIP_0   }},
  {"hipblasDiagType_t",                                {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_DIAG_NON_UNIT",                            {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_DIAG_UNIT",                                {HIP_1082, HIP_0,    HIP_0   }},
  {"hipblasSideMode_t",                                {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_SIDE_LEFT",                                {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_SIDE_RIGHT",                               {HIP_1082, HIP_0,    HIP_0   }},
  {"hipblasPointerMode_t",                             {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_POINTER_MODE_HOST",                        {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_POINTER_MODE_DEVICE",                      {HIP_1082, HIP_0,    HIP_0   }},
  {"hipblasAtomicsMode_t",                             {HIP_3100, HIP_0,    HIP_0   }},
  {"HIPBLAS_ATOMICS_NOT_ALLOWED",                      {HIP_3100, HIP_0,    HIP_0   }},
  {"HIPBLAS_ATOMICS_ALLOWED",                          {HIP_3100, HIP_0,    HIP_0   }},
  {"hipblasDatatype_t",                                {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_16F",                                    {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_16F",                                    {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_32F",                                    {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_32F",                                    {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_64F",                                    {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_64F",                                    {HIP_1082, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_8I",                                     {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_8I",                                     {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_8U",                                     {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_8U",                                     {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_32I",                                    {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_32I",                                    {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_32U",                                    {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_32U",                                    {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_R_16B",                                    {HIP_3000, HIP_0,    HIP_0   }},
  {"HIPBLAS_C_16B",                                    {HIP_3000, HIP_0,    HIP_0   }},
  {"hipblasHandle_t",                                  {HIP_3000, HIP_0,    HIP_0   }},
};
