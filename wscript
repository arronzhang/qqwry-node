srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
  opt.tool_options("compiler_cxx")
  opt.tool_options("compiler_cc")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("compiler_cc")
  conf.check_tool("node_addon")
  conf.check(lib='iconv', libpath=['/usr/lib', '/usr/local/lib'], uselib_store='ICONV')
  conf.env.append_value('CCFLAGS', ['-O3'])
  conf.env.set_variant("default")

def build(bld):
  libqqwry = bld.new_task_gen("cc", "shlib")
  libqqwry.source = "libqqwry/libqqwry.c"
  libqqwry.includes = "libqqwry/"
  libqqwry.name = "libqqwry"
  libqqwry.target = "libqqwry"

  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.target = "qqwry"
  obj.source = "qqwry.cc"
  obj.uselib = "ICONV"
  obj.includes = "libqqwry"
  obj.add_objects = "libqqwry"
