//===--- IRIX.h - IRIX ToolChain Implementations ----------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_IRIX_H
#define LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_IRIX_H

#include "Gnu.h"
#include "clang/Driver/ToolChain.h"

namespace clang {
namespace driver {

namespace toolchains {

class LLVM_LIBRARY_VISIBILITY IRIX : public ToolChain {
public:
  IRIX(const Driver &D, const llvm::Triple &Triple,
       const llvm::opt::ArgList &Args);

  bool HasNativeLLVMSupport() const override { return true; }
  bool IsIntegratedAssemblerDefault() const override { return true; }
  bool IsMathErrnoDefault() const override { return false; }
  RuntimeLibType GetDefaultRuntimeLibType() const override {
    return ToolChain::RLT_CompilerRT;
  }
  UnwindLibType GetDefaultUnwindLibType() const override {
    return ToolChain::UNW_CompilerRT;
  }
  CXXStdlibType GetDefaultCXXStdlibType() const override {
    return ToolChain::CST_Libcxx;
  }
  bool IsUnwindTablesDefault(const llvm::opt::ArgList &Args) const override {
    return false;
  }
  bool isPICDefault() const override { return true; }
  bool isPIEDefault() const override { return false; }
  bool isPICDefaultForced() const override { return false; }
  llvm::DebuggerKind getDefaultDebuggerTuning() const override {
    return llvm::DebuggerKind::DBX;
  }

  SanitizerMask getSupportedSanitizers() const override { return SanitizerMask(); }
  SanitizerMask getDefaultSanitizers() const override { return SanitizerMask(); }

  void addClangTargetOptions(const llvm::opt::ArgList &DriverArgs,
                             llvm::opt::ArgStringList &CC1Args,
                             Action::OffloadKind DeviceOffloadKind) const override;
  void
  AddClangSystemIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                            llvm::opt::ArgStringList &CC1Args) const override;
  void
  AddClangCXXStdlibIncludeArgs(const llvm::opt::ArgList &DriverArgs,
                               llvm::opt::ArgStringList &CC1Args) const override;
  void AddCXXStdlibLibArgs(const llvm::opt::ArgList &DriverArgs,
                           llvm::opt::ArgStringList &CmdArgs) const override;

  const char *getDefaultLinker() const override {
    return "ld.lld";
  }

  std::string getDynamicLinker(const llvm::opt::ArgList &Args) const;

  StringRef GetABI() const { return mABI; }

protected:
  Tool *buildLinker() const override;

  std::string LibSuffix;
  std::string mABI;
};

} // end namespace toolchains

namespace tools {
namespace irix {

class LLVM_LIBRARY_VISIBILITY Linker : public Tool {
public:
  Linker(const ToolChain &TC) : Tool("irix::Linker", "linker", TC) {}

  bool hasIntegratedCPP() const override { return false; }
  bool isLinkJob() const override { return true; }

  void ConstructJob(Compilation &C, const JobAction &JA,
                    const InputInfo &Output, const InputInfoList &Inputs,
                    const llvm::opt::ArgList &TCArgs,
                    const char *LinkingOutput) const override;
};

} // end namespace irix
} // end namespace tools

} // end namespace driver
} // end namespace clang

#endif // LLVM_CLANG_LIB_DRIVER_TOOLCHAINS_IRIX_H
