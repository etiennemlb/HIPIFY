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

#include "ArgParse.h"

cl::OptionCategory ToolTemplateCategory("CUDA to HIP source translator options");

cl::opt<std::string> OutputFilename("o",
  cl::desc("Output filename"),
  cl::value_desc("filename"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> OutputDir("o-dir",
  cl::desc("Output directory"),
  cl::value_desc("directory"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> GeneratePerl("perl",
  cl::desc("Generate hipify-perl"),
  cl::value_desc("perl"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> GeneratePython("python",
  cl::desc("Generate hipify-python"),
  cl::value_desc("python"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> OutputHipifyPerlDir("o-hipify-perl-dir",
  cl::desc("Output directory for hipify-perl script"),
  cl::value_desc("directory"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> OutputPythonMapDir("o-python-map-dir",
  cl::desc("Output directory for Python map"),
  cl::value_desc("directory"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> TemporaryDir("temp-dir",
  cl::desc("Temporary directory"),
  cl::value_desc("directory"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> CudaPath("cuda-path",
  cl::desc("CUDA installation path"),
  cl::value_desc("directory"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> SaveTemps("save-temps",
  cl::desc("Save temporary files"),
  cl::value_desc("save-temps"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> Verbose("v",
  cl::desc("Show commands to run and use verbose output"),
  cl::value_desc("v"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> TranslateToRoc("roc",
  cl::desc("Translate to roc instead of hip where it is possible"),
  cl::value_desc("roc"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> Inplace("inplace",
  cl::desc("Modify input file inplace, replacing input with hipified output, save backup in .prehip file"),
  cl::value_desc("inplace"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> NoBackup("no-backup",
  cl::desc("Don't create a backup file for the hipified source"),
  cl::value_desc("no-backup"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> NoOutput("no-output",
  cl::desc("Don't write any translated output to stdout"),
  cl::value_desc("no-output"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> PrintStats("print-stats",
  cl::desc("Print translation statistics"),
  cl::value_desc("print-stats"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> PrintStatsCSV("print-stats-csv",
  cl::desc("Print translation statistics in CSV file"),
  cl::value_desc("print-stats-csv"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> OutputStatsFilename("o-stats",
  cl::desc("Output filename for statistics"),
  cl::value_desc("filename"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> Examine("examine",
  cl::desc("Combines -no-output and -print-stats options"),
  cl::value_desc("examine"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> DashDash("  ",
  cl::desc("Separator between hipify-clang and clang options;\ndon't specify if there are no clang options"),
  cl::ValueDisallowed,
  cl::cat(ToolTemplateCategory));

cl::list<std::string> IncludeDirs("I",
  cl::desc("Add directory to include search path"),
  cl::value_desc("directory"),
  cl::ZeroOrMore,
  cl::Prefix,
  cl::cat(ToolTemplateCategory));

cl::list<std::string> MacroNames("D",
  cl::desc("Define <macro> to <value> or 1 if <value> omitted"),
  cl::value_desc("macro>=<value"),
  cl::ZeroOrMore,
  cl::Prefix,
  cl::cat(ToolTemplateCategory));

cl::opt<bool> SkipExcludedPPConditionalBlocks("skip-excluded-preprocessor-conditional-blocks",
  cl::desc("Enable default preprocessor behaviour by skipping undefined conditional blocks"),
  cl::value_desc("skip-excluded-preprocessor-conditional-blocks"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> CudaGpuArch("cuda-gpu-arch",
  cl::desc("CUDA GPU architecture (e.g. sm_35);\nmay be specified more than once"),
  cl::value_desc("value"),
  cl::ZeroOrMore,
  cl::Prefix,
  cl::cat(ToolTemplateCategory));

cl::opt<bool> GenerateMarkdown("md",
  cl::desc("Generate documentation in Markdown format"),
  cl::value_desc("markdown"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> GenerateCSV("csv",
  cl::desc("Generate documentation in CSV format"),
  cl::value_desc("csv"),
  cl::cat(ToolTemplateCategory));

cl::opt<std::string> DocFormat("doc-format",
  cl::desc("Documentation format: 'full' (default), 'strict', or 'compact';\n'--md' or '--csv' option should be specified"),
  cl::value_desc("value"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> Experimental("experimental",
  cl::desc("HIP APIs that are experimentally supported"),
  cl::value_desc("experimental"),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> CudaKernelExecutionSyntax("cuda-kernel-execution-syntax",
  cl::desc("Keep CUDA kernel launch syntax (default)"),
  cl::value_desc("cuda-kernel-execution-syntax"),
  cl::init(true),
  cl::cat(ToolTemplateCategory));

cl::opt<bool> HipKernelExecutionSyntax("hip-kernel-execution-syntax",
  cl::desc("Transform CUDA kernel launch syntax to a regular HIP function call (overrides \"--cuda-kernel-execution-syntax\")"),
  cl::value_desc("hip-kernel-execution-syntax"),
  cl::cat(ToolTemplateCategory));

cl::extrahelp CommonHelp(ct::CommonOptionsParser::HelpMessage);

const std::vector<std::string> hipifyOptions {
  std::string(PrintStatsCSV.ArgStr),
  std::string(PrintStats.ArgStr),
  std::string(SkipExcludedPPConditionalBlocks.ArgStr),
  std::string(HipKernelExecutionSyntax.ArgStr),
  std::string(CudaKernelExecutionSyntax.ArgStr),
  std::string(GeneratePerl.ArgStr),
  std::string(GeneratePython.ArgStr),
  std::string(TranslateToRoc.ArgStr),
  std::string(GenerateMarkdown.ArgStr),
  std::string(GenerateCSV.ArgStr),
  std::string(NoBackup.ArgStr),
  std::string(NoOutput.ArgStr),
  std::string(Inplace.ArgStr),
  std::string(Examine.ArgStr),
  std::string(SaveTemps.ArgStr),
  std::string(DocFormat.ArgStr),
  std::string(Experimental.ArgStr),
};

const std::vector<std::string> hipifyOptionsWithTwoArgs {
  std::string(OutputDir.ArgStr),
  std::string(OutputHipifyPerlDir.ArgStr),
  std::string(OutputPythonMapDir.ArgStr),
  std::string(OutputStatsFilename.ArgStr),
  std::string(TemporaryDir.ArgStr),
};
