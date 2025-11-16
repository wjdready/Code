
cargo install flutter_rust_bridge_codegen && flutter_rust_bridge_codegen create my_app && cd my_app && flutter run

作为package, 可以通过 flutter_rust_bridge_codegen create mylib

然后修改 windows/CMakeLists.txt 

改成正确的相对路径
apply_cargokit(${PROJECT_NAME} ../rust hello_rust_ffi_plugin "")
