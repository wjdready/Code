proc create_ip {} {
    # 打包 ip, -root_dir 设置打包生成的地址, 以当前工程作为 ip 地址有助于调试
    ipx::package_project -import_files -force -root_dir ../ip_core/led

    # 设置 ip 属性和信息
    set_property vendor              {xilinx.com}            [ipx::current_core]
    set_property library             {user}                  [ipx::current_core]
    set_property taxonomy            {{/demo}}               [ipx::current_core]
    set_property vendor_display_name {shino}                 [ipx::current_core]
    set_property company_url         {xilinx.com}            [ipx::current_core]
    set_property supported_families  {
        virtex7    Production \
        qvirtex7   Production \
        kintex7    Production \
        kintex7l   Production \
        qkintex7   Production \
        qkintex7l  Production \
        artix7     Production \
        artix7l    Production \
        aartix7    Production \
        qartix7    Production \
        zynq       Production \
        qzynq      Production \
        azynq      Production \
        zynquplus  Production
    } [ipx::current_core]

    ipx::save_core [ipx::current_core]
    close_project
}

create_ip
