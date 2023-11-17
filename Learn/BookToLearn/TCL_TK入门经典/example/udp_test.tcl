package require udp

proc udp_send {text} {
    set sock [udp_open]
    fconfigure $sock -remote [list "192.168.1.102" "12345"]
    puts -nonewline $sock $text
    close $sock
}

entry .entry -width 50 -relief sunken -textvariable value
label .label -text "Message to send:"

button .btn_send -text "Send" -command {udp_send $value}

bind .entry <Return> {
    .btn_send flash
    .btn_send invoke
}

grid .label .entry -padx 1m -pady 1m
grid .btn_send - - -padx 1m -pady 1m
