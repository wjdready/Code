adapter
      adapter command group (command valid any time)
  adapter assert |deassert [srst|trst [assert|deassert srst|trst]]
        Controls SRST and TRST lines.
  adapter deassert |assert [srst|trst [deassert|assert srst|trst]]
        Controls SRST and TRST lines.
  adapter driver driver_name
        Select a debug adapter driver (configuration command)
  adapter gpio [

            do|tdi|tms|tck|trst|swdio|swdio_dir|swclk|srst|led[gpio_number]
            [-chip chip_number] [-active-high|-active-low]
            [-push-pull|-open-drain|-open-source]










            e|-pull-up|-pull-down][-init-inactive|-init-active|-init-input]
            ]
        gpio adapter command group (configuration command)
  adapter list
        List all built-in debug adapter drivers (command valid any time)
  adapter name
        Returns the name of the currently selected adapter (driver)
        (command valid any time)
  adapter serial serial_string
        Set the serial number of the adapter (configuration command)
  adapter speed [khz]
        With an argument, change to the specified maximum jtag speed.  For
        JTAG, 0 KHz signifies adaptive clocking. With or without argument,
        display current setting. (command valid any time)
  adapter srst
        srst adapter command group (command valid any time)
    adapter srst delay [milliseconds]
          delay after deasserting SRST in ms (command valid any time)
    adapter srst pulse_width [milliseconds]
          SRST assertion pulse width in ms (command valid any time)
  adapter transports transport ...
        Declare transports the adapter supports. (configuration command)
  adapter usb
        usb adapter command group (command valid any time)
    adapter usb location [<bus>-port[.port]...]
          display or set the USB bus location of the USB device
          (configuration command)
add_help_text command_name helptext_string
      Add new command help text; Command can be multiple tokens. (command
      valid any time)
add_script_search_dir <directory>
      dir to search for config files and scripts (command valid any time)
add_usage_text command_name usage_string
      Add new command usage text; command can be multiple tokens. (command
      valid any time)
arm
      ARM command group (command valid any time)
  arm core_state ['arm'|'thumb']
        display/change ARM core state
  arm disassemble address [count ['thumb']]
        disassemble instructions
  arm mcr cpnum op1 CRn CRm op2 value
        write coprocessor register
  arm mrc cpnum op1 CRn CRm op2
        read coprocessor register
  arm reg
        display ARM core registers
  arm semihosting ['enable'|'disable']
        activate support for semihosting operations
  arm semihosting_basedir [dir]
        set the base directory for semihosting I/O operations
  arm semihosting_cmdline arguments
        command line arguments to be passed to program
  arm semihosting_fileio ['enable'|'disable']
        activate support for semihosting fileio operations
  arm semihosting_read_user_param
        read parameters in semihosting-user-cmd-0x10X callbacks
  arm semihosting_redirect (disable | tcp <port> ['debug'|'stdio'|'all'])
        redirect semihosting IO
  arm semihosting_resexit ['enable'|'disable']
        activate support for semihosting resumable exit
bindto [name]
      Specify address by name on which to listen for incoming TCP/IP
      connections (configuration command)
bp [<address> [<asid>] <length> ['hw'|'hw_ctx']]
      list or set hardware or software breakpoint
capture command
      Capture progress output and return as tcl return value. If the
      progress output was empty, return tcl return value. (command valid
      any time)
command
      core command group (introspection) (command valid any time)
  command mode [command_name ...]
        Returns the command modes allowed by a command: 'any', 'config', or
        'exec'. If no command is specified, returns the current command
        mode. Returns 'unknown' if an unknown command is given. Command can
        be multiple tokens. (command valid any time)
cti
      CTI commands (configuration command)
  cti create name '-chain-position' name [options ...]
        Creates a new CTI object (command valid any time)
  cti names
        Lists all registered CTI objects by name (command valid any time)
dap
      DAP commands (configuration command)
  dap create name '-chain-position' name
        Creates a new DAP instance (command valid any time)
  dap info [ap_num | 'root']
        display ROM table for specified MEM-AP (default MEM-AP of current
        target) or the ADIv6 root ROM table of current target's DAP
  dap init
        Initialize all registered DAP instances (command valid any time)
  dap names
        Lists all registered DAP instances by name (command valid any time)
debug_level number
      Sets the verbosity level of debugging output. 0 shows errors only; 1
      adds warnings; 2 (default) adds other info; 3 adds debugging; 4 adds
      extra verbose debugging. (command valid any time)
dump_image filename address size
echo [-n] string
      Logs a message at "user" priority. Option "-n" suppresses trailing
      newline (command valid any time)
exit
      exit telnet session
expr
       (command valid any time)
fast_load
      loads active fast load image to current target - mainly for profiling
      purposes
fast_load_image filename address ['bin'|'ihex'|'elf'|'s19'] [min_address
          [max_length]]
      Load image into server memory for later use by fast_load; primarily
      for profiling (command valid any time)
find <file>
      print full path to file according to OpenOCD search rules (command
      valid any time)
flash
      NOR flash command group (command valid any time)
  flash bank bank_id driver_name base_address size_bytes chip_width_bytes
            bus_width_bytes target [driver_options ...]
        Define a new bank with the given name, using the specified NOR
        flash driver. (configuration command)
  flash banks
        Display table with information about flash banks. (command valid
        any time)
  flash erase_address ['pad'] ['unlock'] address length
        Erase flash sectors starting at address and continuing for length
        bytes.  If 'pad' is specified, data outside that range may also be
        erased: the start address may be decreased, and length increased,
        so that all of the first and last sectors are erased. If 'unlock'
        is specified, then the flash is unprotected before erasing.
  flash erase_check bank_id
        Check erase state of all blocks in a flash bank.
  flash erase_sector bank_id first_sector_num (last_sector_num|'last')
        Erase a range of sectors in a flash bank.
  flash fillb address value n
        Fill n bytes with 8-bit value, starting at word address.  (No
        autoerase.)
  flash filld address value n
        Fill n double-words with 64-bit value, starting at word address.
        (No autoerase.)
  flash fillh address value n
        Fill n halfwords with 16-bit value, starting at word address.  (No
        autoerase.)
  flash fillw address value n
        Fill n words with 32-bit value, starting at word address.  (No
        autoerase.)
  flash info bank_id ['sectors']
        Print information about a flash bank.
  flash init
        Initialize flash devices. (configuration command)
  flash list
        Returns a list of details about the flash banks. (command valid any
        time)
  flash mdb address [count]
        Display bytes from flash.
  flash mdh address [count]
        Display half-words from flash.
  flash mdw address [count]
        Display words from flash.
  flash padded_value bank_id value
        Set default flash padded value
  flash probe bank_id
        Identify a flash bank.
  flash protect bank_id first_block [last_block|'last'] ('on'|'off')
        Turn protection on or off for a range of protection blocks or
        sectors in a given flash bank. See 'flash info' output for a list
        of blocks.
  flash read_bank bank_id filename [offset [length]]
        Read binary data from flash bank to file. Allow optional offset
        from beginning of the bank (defaults to zero).
  flash verify_bank bank_id filename [offset]
        Compare the contents of a file with the contents of the flash bank.
        Allow optional offset from beginning of the bank (defaults to
        zero).
  flash verify_image filename [offset [file_type]]
        Verify an image against flash. Allow optional offset from beginning
        of bank (defaults to zero)
  flash write_bank bank_id filename [offset]
        Write binary data from file to flash bank. Allow optional offset
        from beginning of the bank (defaults to zero).
  flash write_image [erase] [unlock] filename [offset [file_type]]
        Write an image to flash.  Optionally first unprotect and/or erase
        the region to be used. Allow optional offset from beginning of bank
        (defaults to zero)
gdb_breakpoint_override ('hard'|'soft'|'disable')
      Display or specify type of breakpoint to be used by gdb 'break'
      commands. (command valid any time)
gdb_flash_program ('enable'|'disable')
      enable or disable flash program (configuration command)
gdb_memory_map ('enable'|'disable')
      enable or disable memory map (configuration command)
gdb_port [port_num]
      Normally gdb listens to a TCP/IP port. Each subsequent GDB server
      listens for the next port number after the base port number
      specified. No arguments reports GDB port. "pipe" means listen to
      stdin output to stdout, an integer is base port number, "disabled"
      disables port. Any other string is are interpreted as named pipe to
      listen to. Output pipe is the same name as input pipe, but with 'o'
      appended. (configuration command)
gdb_report_data_abort ('enable'|'disable')
      enable or disable reporting data aborts (configuration command)
gdb_report_register_access_error ('enable'|'disable')
      enable or disable reporting register access errors (configuration
      command)
gdb_save_tdesc
      Save the target description file
gdb_sync
      next stepi will return immediately allowing GDB to fetch register
      state without affecting target state (command valid any time)
gdb_target_description ('enable'|'disable')
      enable or disable target description (configuration command)
get_reg list
      Get register values from the target
halt [milliseconds]
      request target to halt, then wait up to the specified number of
      milliseconds (default 5000) for it to complete
help [command_name]
      Show full command help; command can be multiple tokens. (command
      valid any time)
hla_command <command>
      execute a custom adapter-specific command
hla_device_desc description_string
      set the device description of the adapter (configuration command)
hla_layout layout_name
      set the layout of the adapter (configuration command)
hla_stlink_backend usb | tcp [port]
      select which ST-Link backend to use (configuration command)
hla_vid_pid (vid pid)*
      the vendor and product ID of the adapter (configuration command)
init
      Initializes configured targets and servers.  Changes command mode
      from CONFIG to EXEC.  Unless 'noinit' is called, this command is
      called automatically at the end of startup. (command valid any time)
itm
      itm command group (command valid any time)
  itm port <port> (0|1|on|off)
        Enable or disable ITM stimulus port (command valid any time)
  itm ports (0|1|on|off)
        Enable or disable all ITM stimulus ports (command valid any time)
jsp_port [port_num]
      Specify port on which to listen for incoming JSP telnet connections.
      (command valid any time)
load_image filename address ['bin'|'ihex'|'elf'|'s19'] [min_address]
          [max_length]
log_output [file_name | "default"]
      redirect logging to a file (default: stderr) (command valid any time)
mdb ['phys'] address [count]
      display memory bytes
mdd ['phys'] address [count]
      display memory double-words
mdh ['phys'] address [count]
      display memory half-words
mdw ['phys'] address [count]
      display memory words
measure_clk
      Runs a test to measure the JTAG clk. Useful with RCLK / RTCK.
      (command valid any time)
mmw address setbits clearbits
      Modify word in memory. new_val = (old_val & ~clearbits) | setbits;
      (command valid any time)
mrb address
      Returns value of byte in memory. (command valid any time)
mrh address
      Returns value of halfword in memory. (command valid any time)
mrw address
      Returns value of word in memory. (command valid any time)
ms
      Returns ever increasing milliseconds. Used to calculate differences
      in time. (command valid any time)
mwb ['phys'] address value [count]
      write memory byte
mwd ['phys'] address value [count]
      write memory double-word
mwh ['phys'] address value [count]
      write memory half-word
mww ['phys'] address value [count]
      write memory word
nand
      NAND flash command group (command valid any time)
  nand device bank_id driver target [driver_options ...]
        defines a new NAND bank (configuration command)
  nand drivers
        lists available NAND drivers (command valid any time)
  nand init
        initialize NAND devices (configuration command)
noinit
      Prevent 'init' from being called at startup. (configuration command)
ocd_find file
      find full path to file (command valid any time)
pld
      programmable logic device commands (command valid any time)
  pld device driver_name [driver_args ... ]
        configure a PLD device (configuration command)
  pld init
        initialize PLD devices (configuration command)
poll ['on'|'off']
      poll target state; or reconfigure background polling
poll_period
      set the servers polling period (command valid any time)
power_restore
      Overridable procedure run when power restore is detected. Runs 'reset
      init' by default. (command valid any time)
profile seconds filename [start end]
      profiling samples the CPU PC
program <filename> [address] [pre-verify] [verify] [reset] [exit]
      write an image to flash, address is only required for binary images.
      verify, reset, exit are optional (command valid any time)
ps
      list all tasks
rbp 'all' | address
      remove breakpoint
read_memory address width count ['phys']
      Read Tcl list of 8/16/32/64 bit numbers from target memory
reg [(register_number|register_name) [(value|'force')]]
      display (reread from target with "force") or set a register; with no
      arguments, displays all registers and their values
reset [run|halt|init]
      Reset all targets into the specified mode. Default reset mode is run,
      if not given.
reset_config [none|trst_only|srst_only|trst_and_srst]
          [srst_pulls_trst|trst_pulls_srst|combined|separate]
          [srst_gates_jtag|srst_nogate] [trst_push_pull|trst_open_drain]
          [srst_push_pull|srst_open_drain]
          [connect_deassert_srst|connect_assert_srst]
      configure adapter reset behavior (command valid any time)
reset_nag ['enable'|'disable']
      Nag after each reset about options that could have been enabled to
      improve performance. (command valid any time)
resume [address]
      resume target execution from current PC or address
rtt
      RTT (command valid any time)
  rtt channellist
        list available channels
  rtt channels
        list available channels
  rtt polling_interval [interval]
        show or set polling interval in ms
  rtt server
        RTT server (command valid any time)
    rtt server start <port> <channel>
          Start a RTT server (command valid any time)
    rtt server stop <port>
          Stop a RTT server (command valid any time)
  rtt setup <address> <size> <ID>
        setup RTT (command valid any time)
  rtt start
        start RTT
  rtt stop
        stop RTT
rwp address
      remove watchpoint
script <file>
      filename of OpenOCD script (tcl) to run (command valid any time)
set_reg dict
      Set target register values
shutdown
      shut the server down (command valid any time)
sleep milliseconds ['busy']
      Sleep for specified number of milliseconds.  "busy" will busy wait
      instead (avoid this). (command valid any time)
soft_reset_halt
      halt the target and do a soft reset
srst_deasserted
      Overridable procedure run when srst deassert is detected. Runs 'reset
      init' by default. (command valid any time)
step [address]
      step one instruction from current PC or address
stm32h7x
      stm32h7x flash command group (command valid any time)
  stm32h7x lock bank_id
        Lock entire flash device.
  stm32h7x mass_erase bank_id
        Erase entire flash device.
  stm32h7x option_read bank_id reg_offset
        Read and display device option bytes.
  stm32h7x option_write bank_id reg_offset value [mask]
        Write device option bit fields with provided value.
  stm32h7x unlock bank_id
        Unlock entire protected flash device.
stm32h7x.cpu0
      target command group (command valid any time)
  stm32h7x.cpu0 arm
        ARM command group (command valid any time)
    stm32h7x.cpu0 arm core_state ['arm'|'thumb']
          display/change ARM core state
    stm32h7x.cpu0 arm disassemble address [count ['thumb']]
          disassemble instructions
    stm32h7x.cpu0 arm mcr cpnum op1 CRn CRm op2 value
          write coprocessor register
    stm32h7x.cpu0 arm mrc cpnum op1 CRn CRm op2
          read coprocessor register
    stm32h7x.cpu0 arm reg
          display ARM core registers
    stm32h7x.cpu0 arm semihosting ['enable'|'disable']
          activate support for semihosting operations
    stm32h7x.cpu0 arm semihosting_basedir [dir]
          set the base directory for semihosting I/O operations
    stm32h7x.cpu0 arm semihosting_cmdline arguments
          command line arguments to be passed to program
    stm32h7x.cpu0 arm semihosting_fileio ['enable'|'disable']
          activate support for semihosting fileio operations
    stm32h7x.cpu0 arm semihosting_read_user_param
          read parameters in semihosting-user-cmd-0x10X callbacks
    stm32h7x.cpu0 arm semihosting_redirect (disable | tcp <port>
              ['debug'|'stdio'|'all'])
          redirect semihosting IO
    stm32h7x.cpu0 arm semihosting_resexit ['enable'|'disable']
          activate support for semihosting resumable exit
  stm32h7x.cpu0 arp_examine ['allow-defer']
        used internally for reset processing
  stm32h7x.cpu0 arp_halt
        used internally for reset processing
  stm32h7x.cpu0 arp_halt_gdb
        used internally for reset processing to halt GDB
  stm32h7x.cpu0 arp_poll
        used internally for reset processing
  stm32h7x.cpu0 arp_reset
        used internally for reset processing
  stm32h7x.cpu0 arp_waitstate
        used internally for reset processing
  stm32h7x.cpu0 array2mem arrayname bitwidth address count
        Writes Tcl array of 8/16/32 bit numbers to target memory
  stm32h7x.cpu0 cget target_attribute
        returns the specified target attribute (command valid any time)
  stm32h7x.cpu0 configure [target_attribute ...]
        configure a new target for use (command valid any time)
  stm32h7x.cpu0 curstate
        displays the current state of this target
  stm32h7x.cpu0 eventlist
        displays a table of events defined for this target
  stm32h7x.cpu0 examine_deferred
        used internally for reset processing
  stm32h7x.cpu0 get_reg list
        Get register values from the target
  stm32h7x.cpu0 invoke-event event_name
        invoke handler for specified event
  stm32h7x.cpu0 itm
        itm command group (command valid any time)
    stm32h7x.cpu0 itm port <port> (0|1|on|off)
          Enable or disable ITM stimulus port (command valid any time)
    stm32h7x.cpu0 itm ports (0|1|on|off)
          Enable or disable all ITM stimulus ports (command valid any time)
  stm32h7x.cpu0 mdb address [count]
        Display target memory as 8-bit bytes
  stm32h7x.cpu0 mdd address [count]
        Display target memory as 64-bit words
  stm32h7x.cpu0 mdh address [count]
        Display target memory as 16-bit half-words
  stm32h7x.cpu0 mdw address [count]
        Display target memory as 32-bit words
  stm32h7x.cpu0 mem2array arrayname bitwidth address count
        Loads Tcl array of 8/16/32 bit numbers from target memory
  stm32h7x.cpu0 mwb address data [count]
        Write byte(s) to target memory
  stm32h7x.cpu0 mwd address data [count]
        Write 64-bit word(s) to target memory
  stm32h7x.cpu0 mwh address data [count]
        Write 16-bit half-word(s) to target memory
  stm32h7x.cpu0 mww address data [count]
        Write 32-bit word(s) to target memory
  stm32h7x.cpu0 read_memory address width count ['phys']
        Read Tcl list of 8/16/32/64 bit numbers from target memory
  stm32h7x.cpu0 rtt
        RTT target commands
    stm32h7x.cpu0 rtt channellist
          list available channels
    stm32h7x.cpu0 rtt channels
          list available channels
    stm32h7x.cpu0 rtt polling_interval [interval]
          show or set polling interval in ms
    stm32h7x.cpu0 rtt setup <address> <size> <ID>
          setup RTT (command valid any time)
    stm32h7x.cpu0 rtt start
          start RTT
    stm32h7x.cpu0 rtt stop
          stop RTT
  stm32h7x.cpu0 set_reg dict
        Set target register values
  stm32h7x.cpu0 tpiu
        tpiu command group
    stm32h7x.cpu0 tpiu config (disable | ((external | internal (<filename>
              | <:port> | -)) (sync <port width> | ((manchester | uart)
              <formatter enable>)) <TRACECLKIN freq> [<trace freq>]))
          Configure TPIU features, DEPRECATED, use 'tpiu create' (command
          valid any time)
  stm32h7x.cpu0 was_examined
        used internally for reset processing
  stm32h7x.cpu0 write_memory address width data ['phys']
        Write Tcl list of 8/16/32/64 bit numbers to target memory
stm32h7x.dap
      dap instance command group (command valid any time)
swd
      SWD command group (command valid any time)
  swd newdap
        declare a new SWD DAP (configuration command)
swo
      swo command group
  swo create name [-dap dap] [-ap-num num] [-baseaddr baseaddr]
        Creates a new TPIU or SWO object (command valid any time)
  swo init
        Initialize TPIU and SWO
  swo names
        Lists all registered TPIU and SWO objects by name (command valid
        any time)
target
      configure target (configuration command)
  target create name type '-chain-position' name [options ...]
        Creates and selects a new target (configuration command)
  target current
        Returns the currently selected target (command valid any time)
  target init
        initialize targets (configuration command)
  target names
        Returns the names of all targets as a list of strings (command
        valid any time)
  target smp targetname1 targetname2 ...
        gather several target in a smp list (command valid any time)
  target types
        Returns the available target types as a list of strings (command
        valid any time)
target_request
      target request command group (command valid any time)
  target_request debugmsgs ['enable'|'charmsg'|'disable']
        display and/or modify reception of debug messages from target
targets [target]
      change current default target (one parameter) or prints table of all
      targets (no parameters) (command valid any time)
tcl_notifications [on|off]
      Target Notification output
tcl_port [port_num]
      Specify port on which to listen for incoming Tcl syntax.  Read help
      on 'gdb_port'. (configuration command)
tcl_trace [on|off]
      Target trace output
telnet_port [port_num]
      Specify port on which to listen for incoming telnet connections.
      Read help on 'gdb_port'. (configuration command)
test_image filename [offset [type]]
test_mem_access size
      Test the target's memory access functions
tpiu
      tpiu command group
  tpiu config (disable | ((external | internal (<filename> | <:port> | -))
            (sync <port width> | ((manchester | uart) <formatter enable>))
            <TRACECLKIN freq> [<trace freq>]))
        Configure TPIU features, DEPRECATED, use 'tpiu create' (command
        valid any time)
  tpiu create name [-dap dap] [-ap-num num] [-baseaddr baseaddr]
        Creates a new TPIU or SWO object (command valid any time)
  tpiu init
        Initialize TPIU and SWO
  tpiu names
        Lists all registered TPIU and SWO objects by name (command valid
        any time)
trace
      trace command group
  trace history ['clear'|size]
        display trace history, clear history or set size
  trace point ['clear'|address]
        display trace points, clear list of trace points, or add new
        tracepoint at address
transport
      Transport command group (command valid any time)
  transport init
        Initialize this session's transport (command valid any time)
  transport list
        list all built-in transports (command valid any time)
  transport select [transport_name]
        Select this session's transport (command valid any time)
usage [command_name]
      Show basic command usage; command can be multiple tokens. (command
      valid any time)
verify_image filename [offset [type]]
verify_image_checksum filename [offset [type]]
version
      show program version (command valid any time)
virt2phys virtual_address
      translate a virtual address into a physical address (command valid
      any time)
wait_halt [milliseconds]
      wait up to the specified number of milliseconds (default 5000) for a
      previously requested halt
wp [address length [('r'|'w'|'a') value [mask]]]
      list (no params) or create watchpoints
write_memory address width data ['phys']
      Write Tcl list of 8/16/32/64 bit numbers to target memory