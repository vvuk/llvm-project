//===--- IRIX.cpp - IRIX ToolChain Implementations --------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "IRIX.h"
#include "CommonArgs.h"
#include "Arch/Mips.h"
#include "clang/Config/config.h"
#include "clang/Driver/Driver.h"
#include "clang/Driver/Options.h"
#include "llvm/Support/Path.h"
#include "llvm/Support/VirtualFileSystem.h"

using namespace clang::driver;
using namespace clang::driver::tools;
using namespace clang::driver::toolchains;
using namespace clang;
using namespace llvm::opt;

using tools::addPathIfExists;

static StringRef getOSLibDir(StringRef abi) {
  if (abi == "n32")
    return "lib32";
  if (abi == "n64")
    return "lib64";
  if (abi == "o32")
    return "lib";
  llvm_unreachable("Invalid abi for getOSLibDir");
}

IRIXGCC::IRIXGCC(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
    : Generic_ELF(D, Triple, Args) {
  GCCInstallation.init(Triple, Args);
  Multilibs = GCCInstallation.getMultilibs();
  SelectedMultilib = GCCInstallation.getMultilib();
  std::string SysRoot = computeSysRoot();
  ToolChain::path_list &PPaths = getProgramPaths();

  Generic_GCC::PushPPaths(PPaths);

  // The selection of paths to try here is designed to match the patterns which
  // the GCC driver itself uses, as this is part of the GCC-compatible driver.
  // This was determined by running GCC in a fake filesystem, creating all
  // possible permutations of these directories, and seeing which ones it added
  // to the link paths.
  path_list &Paths = getFilePaths();

  StringRef CPUName;
  StringRef ABIName;
  tools::mips::getMipsCPUAndABI(Args, Triple, CPUName, ABIName);

  mABI = ABIName;

  const std::string OSLibDir = std::string(getOSLibDir(ABIName));
  const std::string MultiarchTriple = getMultiarchTriple(D, Triple, SysRoot);
  Paths.push_back(SysRoot + "/usr/" + OSLibDir + "/" + CPUName.data());
  Paths.push_back(SysRoot + "/usr/" + OSLibDir);
  Paths.push_back(SysRoot + "/" + OSLibDir);

  // Similar to the logic for GCC above, if we currently running Clang inside
  // of the requested system root, add its parent library paths to
  // those searched.
  // FIXME: It's not clear whether we should use the driver's installed
  // directory ('Dir' below) or the ResourceDir.
  if (StringRef(D.Dir).startswith(SysRoot)) {
    addPathIfExists(D, D.Dir + "/../" + OSLibDir, Paths);
  }

  Generic_GCC::AddMultiarchPaths(D, SysRoot, OSLibDir, Paths);
  Generic_GCC::AddMultilibPaths(D, SysRoot, OSLibDir, MultiarchTriple, Paths);

  mUseGNULinker = getenv("CLANG_USE_GNU_LINKER") != nullptr;
  mNoGCC = getenv("CLANG_NO_GCC") != nullptr;
  mIsLLD = false;
  mRuntimeLibType = ToolChain::GetRuntimeLibType(Args);
  ToolChain::GetLinkerPath(&mIsLLD);
}

bool IRIXGCC::HasNativeLLVMSupport() const { return true; }

Tool *IRIXGCC::buildLinker() const {
  if (mUseGNULinker)
    return new tools::gnutools::Linker(*this);
  return new tools::irix::Linker(*this);
}

Tool *IRIXGCC::buildAssembler() const {
  return new tools::gnutools::Assembler(*this);
}

std::string IRIXGCC::getDynamicLinker(const llvm::opt::ArgList &Args) const {
  if (mABI == "n32")
    return "/usr/lib32/libc.so.1";
  if (mABI == "n64")
    return "/usr/lib64/libc.so.1";
  if (mABI == "o32")
    return "/usr/lib/libc.so.1";
  llvm_unreachable("Bad ABI in getDynamicLinker");
}

void IRIXGCC::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                     ArgStringList &CC1Args) const {
  const Driver &D = getDriver();
  std::string SysRoot = computeSysRoot();

  if (DriverArgs.hasArg(clang::driver::options::OPT_nostdinc))
    return;

  std::string ExtraStuffPath = SysRoot + "/usr/lib/clang/" + getTriple().str();

  AddMultilibIncludeArgs(DriverArgs, CC1Args);

  if (!DriverArgs.hasArg(options::OPT_nostdlibinc)) {
    if (!mNoGCC)
      addSystemInclude(DriverArgs, CC1Args, GCCInstallation.getInstallPath() + "/include-fixed");
    else
      addSystemInclude(DriverArgs, CC1Args, ExtraStuffPath + "/include-fixed");
    addSystemInclude(DriverArgs, CC1Args, SysRoot + "/usr/include");
  }

  if (!DriverArgs.hasArg(options::OPT_nobuiltininc)) {
    SmallString<128> P(D.ResourceDir);
    llvm::sys::path::append(P, "include");
    addSystemInclude(DriverArgs, CC1Args, P);
  }

  if (DriverArgs.hasArg(options::OPT_nostdlibinc))
    return;

  // Check for configure-time C include directories.
  StringRef CIncludeDirs(C_INCLUDE_DIRS);
  if (CIncludeDirs != "") {
    SmallVector<StringRef, 5> Dirs;
    CIncludeDirs.split(Dirs, ":");
    for (StringRef Dir : Dirs) {
      StringRef Prefix =
          llvm::sys::path::is_absolute(Dir) ? "" : StringRef(SysRoot);
      addExternCSystemInclude(DriverArgs, CC1Args, Prefix + Dir);
    }
    return;
  }

  // TODO IRIX do we need ExternCSystemInclude for /usr/include?
  //addExternCSystemInclude(DriverArgs, CC1Args, SysRoot + "/usr/include");
}

void IRIXGCC::addClangTargetOptions(const ArgList &DriverArgs,
                                   ArgStringList &CC1Args,
                                   Action::OffloadKind) const {
  const bool UseInitArrayDefault = false;
  const bool UseEmulatedTLS = true;
  const bool UseMipsPc64RelDefault = false;

  // TODO IRIX -- figure out if this is actually needed to set to false; if it is,
  // and we still want to allow overriding it, then we need to do some work here because
  // the mips-pc64-rel arg can only be passed a maxmimum of one times.
  bool pc64rel = DriverArgs.hasFlag(options::OPT_mmips_pc64_rel,
    options::OPT_mno_mips_pc64_rel, UseMipsPc64RelDefault);
  CC1Args.push_back("-mllvm");
  if (pc64rel)
    CC1Args.push_back("-mmips-pc64-rel=true");
  else
    CC1Args.push_back("-mmips-pc64-rel=false");

  if (!DriverArgs.hasFlag(options::OPT_fuse_init_array,
                          options::OPT_fno_use_init_array, UseInitArrayDefault))
    CC1Args.push_back("-fno-use-init-array");
  if (DriverArgs.hasFlag(options::OPT_femulated_tls,
                          options::OPT_fno_emulated_tls, UseEmulatedTLS))
    CC1Args.push_back("-femulated-tls");
}

void IRIXGCC::addExtraOpts(llvm::opt::ArgStringList &Args) const {
#ifdef ENABLE_LINKER_BUILD_ID
  Args.push_back("--build-id");
#endif

  // TODO -- need to guard this with notstdlib (again need to use irix::Linker::ConstructJob)
  Args.push_back("-lc");

  Args.push_back("--as-needed");
  Args.push_back("-lm");
  Args.push_back("-lgen"); // needed for a bunch of std C stuff, if it gets used
  Args.push_back("--no-as-needed");

  // Hack for shared libraries, otherwise they get generated with a base of 0,
  // and rld doesn't like that.
  // This doesn't work on Binutils LD
  // TODO -- only do this if --shared, but we don't have access to args.
  // fix when we use irix::Linker::ConstructJob below.
  Args.push_back("-image-base=0x10000");

  Args.push_back("-init=__gcc_init");
  Args.push_back("-fini=__gcc_fini");

  // These are used by rld
  Args.push_back("--export-dynamic-symbol=__Argc");
  Args.push_back("--export-dynamic-symbol=__Argv");
  Args.push_back("--export-dynamic-symbol=__rld_obj_head");
}
