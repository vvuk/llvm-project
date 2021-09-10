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
using namespace clang::driver::toolchains;
using namespace clang;
using namespace llvm::opt;

using tools::addPathIfExists;

static StringRef getOSLibDir(const llvm::Triple &Triple, const ArgList &Args) {
  if (tools::mips::hasMipsAbiArg(Args, "n32")) {
    return "lib32";
  } else {
    return "lib64";
  }
}

IRIX::IRIX(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
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

  const std::string OSLibDir = std::string(getOSLibDir(Triple, Args));
  const std::string MultiarchTriple = getMultiarchTriple(D, Triple, SysRoot);
  Paths.push_back(SysRoot + "/usr/" + OSLibDir + "/" + CPUName.data());
  Paths.push_back(SysRoot + "/usr/" + OSLibDir);
  Paths.push_back(SysRoot + "/" + OSLibDir);

#ifdef ENABLE_LINKER_BUILD_ID
  ExtraOpts.push_back("--build-id");
#endif

//#define BINUTILS_LD_BUILD
#ifndef BINUTILS_LD_BUILD
  // Without this, lld will fail to link, since it's looking for
  // code defined inside rld
  ExtraOpts.push_back("--allow-shlib-undefined");
  ExtraOpts.push_back("-lm");

  // Hack for shared libraries, otherwise they take the wrong base address
  // and rld isn't happy
  // This doesn't work on Binutils LD
  ExtraOpts.push_back("-image-base=0x10000");
#endif

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
}

bool IRIX::HasNativeLLVMSupport() const { return true; }

Tool *IRIX::buildLinker() const { return new tools::gnutools::Linker(*this); }

Tool *IRIX::buildAssembler() const {
  return new tools::gnutools::Assembler(*this);
}

std::string IRIX::getDynamicLinker(const ArgList &Args) const {
  if (tools::mips::hasMipsAbiArg(Args, "n32")) {
    return "/usr/lib32/libc.so.1";
  } else {
    return "/usr/lib64/libc.so.1";
  }
}

void IRIX::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                     ArgStringList &CC1Args) const {
  const Driver &D = getDriver();
  std::string SysRoot = computeSysRoot();

  addSystemInclude(DriverArgs, CC1Args, GCCInstallation.getInstallPath() + "/include-fixed");

  AddMultilibIncludeArgs(DriverArgs, CC1Args);

  if (DriverArgs.hasArg(clang::driver::options::OPT_nostdinc))
    return;

  if (!DriverArgs.hasArg(options::OPT_nostdlibinc)) {
    //addSystemInclude(DriverArgs, CC1Args, SysRoot + "/usr/include-fixed");
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

  // Lacking those, try to detect the correct set of system includes for the
  // target triple.

  addExternCSystemInclude(DriverArgs, CC1Args, SysRoot + "/usr/include");

  //addExternCSystemInclude(DriverArgs, CC1Args, SysRoot + "/usr/include-fixed");
  
}

void IRIX::addExtraOpts(llvm::opt::ArgStringList &CmdArgs) const {
  for (const auto &Opt : ExtraOpts)
    CmdArgs.push_back(Opt.c_str());
}
