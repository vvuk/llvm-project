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
#include "clang/Driver/Compilation.h"
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


IRIX::IRIX(const Driver &D, const llvm::Triple &Triple, const ArgList &Args)
    : ToolChain(D, Triple, Args)
{
  getProgramPaths().push_back(getDriver().getInstalledDir());
  if (getDriver().getInstalledDir() != D.Dir)
    getProgramPaths().push_back(D.Dir);

  StringRef cpu;
  StringRef abi;
  tools::mips::getMipsCPUAndABI(Args, Triple, cpu, abi);

  mABI = abi.str();

  std::string SysRoot = computeSysRoot();

  getProgramPaths().push_back(getDriver().getInstalledDir());
  if (getDriver().getInstalledDir() != getDriver().Dir)
    getProgramPaths().push_back(getDriver().Dir);

  // Find out the library suffix based on the ABI.
  LibSuffix = tools::mips::getMipsABILibSuffix(Args, Triple);

  // IRIX has .../mips3 .../mips4 too
  getFilePaths().push_back(SysRoot + "/usr/lib" + LibSuffix + "/" + cpu.str());
  getFilePaths().push_back(SysRoot + "/usr/lib" + LibSuffix);
  // I don't think this is needed
  //getFilePaths().push_back(SysRoot + "/lib" + LibSuffix);

  // this is a little wonky.  The ToolChain::ToolChain constructor calls the virtual
  // getRuntimePath() before we've had a chance to init LibSuffix (or, possibly, the
  // string, so that's not cool).  Push the real one in front.  Might be the same.
  if (auto RuntimePath = getRuntimePath())
    getLibraryPaths().insert(getLibraryPaths().begin(), *RuntimePath);

  // Similar to the logic for GCC above, if we currently running Clang inside
  // of the requested system root, add its parent library paths to
  // those searched.
  // FIXME: It's not clear whether we should use the driver's installed
  // directory ('Dir' below) or the ResourceDir.
  if (StringRef(D.Dir).startswith(SysRoot)) {
    addPathIfExists(D, D.Dir + "/../lib" + LibSuffix, getFilePaths());
  }
}

Tool *IRIX::buildLinker() const {
  return new tools::irix::Linker(*this);
}

std::string IRIX::getDynamicLinker(const llvm::opt::ArgList &Args) const {
  return "/usr/lib" + LibSuffix + "/libc.so.1";
}

void IRIX::addClangTargetOptions(const ArgList &DriverArgs,
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

void IRIX::AddClangSystemIncludeArgs(const ArgList &DriverArgs,
                                     ArgStringList &CC1Args) const {
  const Driver &D = getDriver();

  if (DriverArgs.hasArg(clang::driver::options::OPT_nostdinc))
    return;

  std::string ExtraStuffPath = D.SysRoot + "/usr/lib/clang/" + getTriple().str();
  //AddMultilibIncludeArgs(DriverArgs, CC1Args);

  if (!DriverArgs.hasArg(options::OPT_nobuiltininc)) {
    SmallString<128> P(D.ResourceDir);
    llvm::sys::path::append(P, "include");
    addSystemInclude(DriverArgs, CC1Args, P);
  }

  if (DriverArgs.hasArg(options::OPT_nostdlibinc))
    return;

  // this is the fixincludes output -- actual replacements for the system headers
  addSystemInclude(DriverArgs, CC1Args, ExtraStuffPath + "/include-fixed");
  // these are headers that interpose themselves before/after the system ones
  addSystemInclude(DriverArgs, CC1Args, ExtraStuffPath + "/include");
  // these are the actual system headers
  addSystemInclude(DriverArgs, CC1Args, D.SysRoot + "/usr/include");

  // Check for configure-time C include directories.
  StringRef CIncludeDirs(C_INCLUDE_DIRS);
  if (CIncludeDirs != "") {
    SmallVector<StringRef, 5> Dirs;
    CIncludeDirs.split(Dirs, ":");
    for (StringRef Dir : Dirs) {
      StringRef Prefix =
          llvm::sys::path::is_absolute(Dir) ? "" : StringRef(D.SysRoot);
      addExternCSystemInclude(DriverArgs, CC1Args, Prefix + Dir);
    }
    return;
  }

  // TODO IRIX do we need ExternCSystemInclude for /usr/include?
  if (!D.SysRoot.empty()) {
    SmallString<128> P(D.SysRoot);
    llvm::sys::path::append(P, "usr", "include");
    addExternCSystemInclude(DriverArgs, CC1Args, P.str());
  }
}

void IRIX::AddClangCXXStdlibIncludeArgs(const ArgList &DriverArgs,
                                        ArgStringList &CC1Args) const {
  if (DriverArgs.hasArg(options::OPT_nostdlibinc) ||
      DriverArgs.hasArg(options::OPT_nostdincxx))
    return;

  switch (GetCXXStdlibType(DriverArgs)) {
  case ToolChain::CST_Libcxx: {
    SmallString<128> P(getDriver().Dir);
    llvm::sys::path::append(P, "..", "include", "c++", "v1");
    addSystemInclude(DriverArgs, CC1Args, P.str());
    break;
  }

  default:
    llvm_unreachable("invalid stdlib name");
  }
}

void IRIX::AddCXXStdlibLibArgs(const ArgList &DriverArgs,
                               ArgStringList &CmdArgs) const {
  switch (GetCXXStdlibType(DriverArgs)) {
  case ToolChain::CST_Libcxx:
    CmdArgs.push_back("-lc++");
    break;

  case ToolChain::CST_Libstdcxx:
    llvm_unreachable("invalid stdlib name");
  }
}

Optional<std::string> IRIX::getRuntimePath() const {
  const Driver &D = getDriver();
  SmallString<128> P;

  // First try the triple passed to driver as --target=<triple>.
  P.assign(D.ResourceDir);
  llvm::sys::path::append(P, "lib" + LibSuffix, D.getTargetTriple());
  if (getVFS().exists(P))
    return llvm::Optional<std::string>(std::string(P.str()));

  // Second try the normalized triple.
  P.assign(D.ResourceDir);
  llvm::sys::path::append(P, "lib" + LibSuffix, getTriple().str());
  if (getVFS().exists(P))
    return llvm::Optional<std::string>(std::string(P.str()));

  return None;
}

void irix::Linker::ConstructJob(Compilation &C, const JobAction &JA,
                                   const InputInfo &Output,
                                   const InputInfoList &Inputs,
                                   const ArgList &Args,
                                   const char *LinkingOutput) const {
  const toolchains::IRIX &ToolChain =
      static_cast<const toolchains::IRIX &>(getToolChain());
  const Driver &D = ToolChain.getDriver();
  const bool IsPIE =
      !Args.hasArg(options::OPT_shared) &&
      (Args.hasArg(options::OPT_pie) || ToolChain.isPIEDefault());

  ArgStringList CmdArgs;

  // Silence warning for "clang -g foo.o -o foo"
  Args.ClaimAllArgs(options::OPT_g_Group);
  // and "clang -emit-llvm foo.o -o foo"
  Args.ClaimAllArgs(options::OPT_emit_llvm);
  // and for "clang -w foo.o -o foo". Other warning options are already
  // handled somewhere else.
  Args.ClaimAllArgs(options::OPT_w);

  if (!D.SysRoot.empty())
    CmdArgs.push_back(Args.MakeArgString("--sysroot=" + D.SysRoot));

  if (IsPIE)
    CmdArgs.push_back("-pie");

  if (ToolChain.isNoExecStackDefault()) {
    CmdArgs.push_back("-z");
    CmdArgs.push_back("noexecstack");
  }

  // IRIX doesn't understand this, so let's not confuse matters
  CmdArgs.push_back("-z");
  CmdArgs.push_back("norelro");

  CmdArgs.push_back("--eh-frame-hdr");

  if (Args.hasArg(options::OPT_static)) {
    CmdArgs.push_back("-static");
  } else {
    if (Args.hasArg(options::OPT_rdynamic))
      CmdArgs.push_back("-export-dynamic");
    if (Args.hasArg(options::OPT_shared)) {
      CmdArgs.push_back("-shared");
    } else {
      CmdArgs.push_back("-dynamic-linker");
      CmdArgs.push_back(Args.MakeArgString(ToolChain.getDynamicLinker(Args)));
    }
  }

  CmdArgs.push_back("-m");
  if (ToolChain.GetABI() == "n32")
    CmdArgs.push_back("elf32btsmipn32_irix");
  else if (ToolChain.GetABI() == "n64")
    CmdArgs.push_back("elf64btsmip_irix");
  else
    llvm_unreachable("invalid ABI");

  if (Arg *A = Args.getLastArg(options::OPT_G)) {
    StringRef v = A->getValue();
    CmdArgs.push_back(Args.MakeArgString("-G" + v));
    A->claim();
  }

  if (!Output.isFilename())
    assert(Output.isNothing() && "Invalid output.");

  CmdArgs.push_back("-o");
  CmdArgs.push_back(Output.getFilename());

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles)) {
    // from IRIX
    if (!Args.hasArg(options::OPT_shared))
      CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crt1.o")));

    // from compiler-rt or gcc
    std::string crtbegin = ToolChain.getCompilerRT(Args, "crtbegin", ToolChain::FT_Object);
    if (ToolChain.getVFS().exists(crtbegin))
      CmdArgs.push_back(Args.MakeArgString(crtbegin));
    else if (!getenv("CLANG_NO_GCC_CRT"))
      CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("gcc-irix-crti.o")));
  }

  Args.AddAllArgs(CmdArgs, options::OPT_L);
  Args.AddAllArgs(CmdArgs, options::OPT_u);
  ToolChain.AddFilePathLibArgs(Args, CmdArgs);
  Args.AddAllArgs(CmdArgs, options::OPT_T_Group);
  Args.AddAllArgs(CmdArgs, options::OPT_e);
  Args.AddAllArgs(CmdArgs, options::OPT_s);
  Args.AddAllArgs(CmdArgs, options::OPT_t);
  Args.AddAllArgs(CmdArgs, options::OPT_Z_Flag);
  Args.AddAllArgs(CmdArgs, options::OPT_r);

  if (D.isUsingLTO()) {
    assert(!Inputs.empty() && "Must have at least one input.");
    addLTOOptions(ToolChain, Args, CmdArgs, Output, Inputs[0],
                  D.getLTOMode() == LTOK_Thin);
  }

  // these will both be false for irix, but do it here for some future world
  bool NeedsSanitizerDeps = addSanitizerRuntimes(ToolChain, Args, CmdArgs);
  bool NeedsXRayDeps = addXRayRuntime(ToolChain, Args, CmdArgs);

  AddLinkerInputs(ToolChain, Inputs, Args, CmdArgs, JA);

  bool didBuiltins = false;

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nodefaultlibs)) {
    if (D.CCCIsCXX() && ToolChain.ShouldLinkCXXStdlib(Args)) {
      bool OnlyLibstdcxxStatic = Args.hasArg(options::OPT_static_libstdcxx) &&
                                !Args.hasArg(options::OPT_static);
      CmdArgs.push_back("--push-state");
      CmdArgs.push_back("--as-needed");
      if (OnlyLibstdcxxStatic)
        CmdArgs.push_back("-Bstatic");
      ToolChain.AddCXXStdlibLibArgs(Args, CmdArgs);
      if (OnlyLibstdcxxStatic)
        CmdArgs.push_back("-Bdynamic");
      CmdArgs.push_back("-lm");
      CmdArgs.push_back("--pop-state");

      AddRunTimeLibs(ToolChain, D, CmdArgs, Args);
      didBuiltins = true;
    }

    if (NeedsSanitizerDeps)
      linkSanitizerRuntimeDeps(ToolChain, CmdArgs);
    if (NeedsXRayDeps)
      linkXRayRuntimeDeps(ToolChain, CmdArgs);

    // TODO should we just assume -lpthread, add it as_needed?  Without this
    // porting software is just going to be more annoying.
    if (Args.hasArg(options::OPT_pthread))
      CmdArgs.push_back("-lpthread");

    if (Args.hasArg(options::OPT_fsplit_stack))
      CmdArgs.push_back("--wrap=pthread_create");

    if (!Args.hasArg(options::OPT_nolibc))
      CmdArgs.push_back("-lc");
  }

  // can't use AddRunTimeLibs because it pulls in libunwind
  if (!didBuiltins)
    CmdArgs.push_back(ToolChain.getCompilerRTArgString(Args, "builtins"));

  if (!Args.hasArg(options::OPT_nostdlib, options::OPT_nostartfiles)) {
    // from compiler-rt or gcc
    std::string crtend = ToolChain.getCompilerRT(Args, "crtend", ToolChain::FT_Object);
    if (ToolChain.getVFS().exists(crtend))
      CmdArgs.push_back(Args.MakeArgString(crtend));
    else if (!getenv("CLANG_NO_GCC_CRT"))
      CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("gcc-irix-crtn.o")));

    // from IRIX
    CmdArgs.push_back(Args.MakeArgString(ToolChain.GetFilePath("crtn.o")));
  }

  // Always export these dynamically and preemptible; rld depends on them existing and having GOT
  // entries
  CmdArgs.push_back("--export-dynamic-symbol=__Argc");
  CmdArgs.push_back("--export-dynamic-symbol=__Argv");
  CmdArgs.push_back("--export-dynamic-symbol=__rld_obj_head");

  ToolChain.addProfileRTLibs(Args, CmdArgs);

  const char *Exec = Args.MakeArgString(getToolChain().GetLinkerPath());
  C.addCommand(std::make_unique<Command>(JA, *this,
                                         ResponseFileSupport::AtFileCurCP(),
                                         Exec, CmdArgs, Inputs, Output));
}
