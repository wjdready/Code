set(COMPONENT_SRCS "func.cpp"
                   "main.cpp")
set(COMPONENT_ADD_INCLUDEDIRS ".")

# 设置依赖的库组件
# set(COMPONENT_PRIV_REQUIRES nvs_flash console)

register_component()
