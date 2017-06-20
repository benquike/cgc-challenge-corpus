import os
import gdb

# class SavePrefixCommand (gdb.Command):
#     '''
#     Save the current breakpoints to a file.
#     This command takes a single argument, a file name.
#     The breakpoints can be restored using the 'source' command.
#     '''

#     def __init__(self):
#         super(SavePrefixCommand, self).__init__ ("save breakpoints",
#                                                  gdb.COMMAND_SUPPORT,
#                                                  gdb.COMPLETE_FILENAME)
    

#     def invoke (self, arg, from_tty):
#         with open (arg, 'w') as f:
#             for bp in gdb.breakpoints():
#                 print >> f, "break", bp.get_location (),
#                 if bp.get_thread () is not None:
#                     print >> f, " thread", bp.get_thread (),
#                 if bp.get_condition () is not None:
#                     print >> f, " if", bp.get_condition (),
#                 print >> f
#                 if not bp.is_enabled ():
#                     print >> f, "disable $bpnum"
#                 # Note: we don't save the ignore count; there doesn't
#                 # seem to be much point.
#                 commands = bp.get_commands ()
#                 if commands is not None:
#                     print >> f, "commands"
#                     # Note that COMMANDS has a trailing newline.
#                     print >> f, commands,
#                     print >> f, "end"
#                 print >> f

# SavePrefixCommand()

def write_memory(addr, val):
    '''
    Write val to addr using only one byte
    '''

    # print "writing " + str(val) +  " to " +hex(addr)
    cmd = "set {unsigned char} " + hex(addr) + " = " + str(val)

def read_memory(addr):
    '''
    Read one byte from addr
    '''
    cmd = 'x /1ub ' + hex(addr)
    ret = gdb.execute(cmd, to_string=True)
    # print "ret|" + ret 
    byte = int(ret.strip().split()[1])
    return byte
    
class LLVMGCDAFileStartBp(gdb.Breakpoint):
    def __init__(self):
        super(LLVMGCDAFileStartBp, self).__init__("lib/GCDAProfiling.c:201")

    def stop(self):
        write_buffer = gdb.parse_and_eval("write_buffer")
        filename = gdb.parse_and_eval("filename")
        gcda_filename = filename.string()

        if not os.path.exists(gcda_filename):
            return False

        f = open(gcda_filename, "rb")
        c = f.read()
        base_addr = long(write_buffer)
        for i in range(len(c)):
            write_memory(base_addr + i, ord(c[i]))

        return False

class LLVMGCDAFileEndBp(gdb.Breakpoint):
    def __init__(self):
        super(LLVMGCDAFileEndBp, self).__init__("llvm_gcda_end_file")
        # super(LLVMGCDAFileEndBp, self).__init__("lib/GCDAProfiling.c:331")

    def stop(self):
        filename = gdb.parse_and_eval("filename")
        write_buffer = gdb.parse_and_eval("write_buffer")
        cur_pos = gdb.parse_and_eval("cur_pos")

        # print "value of filename:" + filename.string()
        # print "value of write_buffer=====:" + write_buffer.address
        # print "value of cur_pos:" + str(cur_pos)
        gcda_filename = filename.string()
        try:
            os.makedirs(os.path.dirname(gcda_filename))
        except OSError:
            pass
        
        size = long(cur_pos)
        base_addr = long(write_buffer)
        # print "size:" + hex(size)
        # print "base_addr:" + str(base_addr)

        with open(gcda_filename, 'wb') as f:
            for i in range(size):
                b = read_memory(base_addr + i)
                f.write(chr(b))

        return False

LLVMGCDAFileStartBp()
LLVMGCDAFileEndBp()
