srcdir = "."
blddir = "build"
APPNAME = "node-libvirt"
VERSION = "0.0.1"

def set_options(ctx):
  ctx.recurse('src')
  
def configure(ctx):
  ctx.recurse('src')
  
def build(ctx):
  ctx.recurse('src')
