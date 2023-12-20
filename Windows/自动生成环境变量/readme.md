
该程序自动读取自身目录下的 config.ini，输出 batch脚本 xx.bat 和 shell 脚本 xx

之后在需要时运行 xx 即可导出 config.ini 中设置的环境变量

config.ini 例子

```ini
[python38]
C:\ProgramFiles\Python\Python38
C:\ProgramFiles\Python\Python38\Scripts

[flutter]  
C:\ProgramFiles\flutter\bin
```

源码 `main.rs`

```rust
use regex::Regex;
use std::collections::HashMap;
use std::env;
use std::fs::File;
use std::io::{BufRead, BufReader, Write};

fn output_batch(map: &HashMap<String, Vec<String>>) {
    let mut contant = String::from("@echo off\n\n@set ADD_PATH=\n\n");
    let mut menu = String::from("\n\n@REM ===== Menu ====\n@if \"%1\" == \"\" (\n");
    let mut index = 0;

    for (key, value) in map {
        let format = format!(
            "{}if \"%1\" == \"{index}\" (\n",
            if index == 0 { "@" } else { "else " }
        );
        contant.push_str(format.as_str());

        for (i, path) in value.iter().enumerate() {
            if i == 0 {
                contant.push_str("    @set ADD_PATH=");
            }
            contant.push_str(path);
            if i != value.len() - 1 {
                contant.push_str(";");
            }
        }

        contant.push_str("\n");

        let format = format!("    @echo {index}.{key}\n");
        menu.push_str(format.as_str());

        contant.push_str(") ");
        index += 1;
        println!("{} {:?}", key, value);
    }

    menu.push_str(")\n\n");
    contant.push_str(menu.as_str());

    contant.push_str("@if not \"%ADD_PATH%\" == \"\" (\n");
    contant.push_str("    @echo %ADD_PATH%\n");
    contant.push_str("    @set \"PATH=%PATH%;%ADD_PATH%\"\n");
    contant.push_str(")\n");

    let filepath = env::current_exe().unwrap().parent().unwrap().join("xx.bat");
    let mut file = File::create(filepath).unwrap();
    file.write(contant.as_bytes()).unwrap();
}

fn output_shell(map: &HashMap<String, Vec<String>>) {
    let mut contant = String::from("#!/bin/bash\n\nADD_PATH=\n\n");
    let mut menu = String::from("\n\n# ===== Menu ====\nif [ \"$1\" == \"\" ]; then\n");
    let mut index = 0;

    for (key, value) in map {
        let format = format!(
            "{}if [ \"$1\" == \"{index}\" ]; then\n",
            if index == 0 { "" } else { "el" }
        );
        contant.push_str(format.as_str());

        for (i, path) in value.iter().enumerate() {
            let path = path.replace("\\", "/").replace(":", "");
            let format: String =
                format!("{}/{}", if i == 0 { "    ADD_PATH=\"" } else { "" }, path);
            contant.push_str(format.as_str());

            if i != value.len() - 1 {
                contant.push_str(":");
            }
        }

        contant.push_str("\"\n");

        let format = format!("    echo {index}.{key}\n");
        menu.push_str(format.as_str());

        contant.push_str("");
        index += 1;
    }

    contant.push_str("fi");
    menu.push_str("fi\n\n");

    contant.push_str(menu.as_str());

    contant.push_str("if [ ! \"$ADD_PATH\" == \"\" ]; then\n");
    contant.push_str("    echo \"$ADD_PATH\"\n");
    contant.push_str("    export PATH=$PATH:$ADD_PATH\n");
    contant.push_str("fi\n");

    let filepath = env::current_exe().unwrap().parent().unwrap().join("xx");
    let mut file = File::create(filepath).unwrap();
    file.write(contant.as_bytes()).unwrap();
}

fn main() -> Result<(), std::io::Error> {
    let re = Regex::new(r"^\[\s*(?<name>[a-zA-Z0-9_]+)\s*\]\s*$").unwrap();

    let filepath = env::current_exe()?.parent().unwrap().join("config.ini");
    println!("Read {:?}", { &filepath });
    let file = File::open(filepath)?;
    let mut reader = BufReader::new(file);

    let mut line = String::new();

    let mut map: HashMap<String, Vec<String>> = HashMap::new();

    let mut key = String::new();
    let mut values: Vec<String> = Vec::new();

    loop {
        let bytes_read = reader.read_line(&mut line)?;
        if bytes_read == 0 {
            break;
        }

        let s = line.trim();
        if s.len() < 1 {
            line.clear();
            continue;
        }

        match re.captures(s) {
            Some(caps) => {
                if key.len() > 0 && values.len() > 0 {
                    map.insert(key, values.clone());
                }

                key = caps["name"].to_string();
                values.clear();
                // println!("{}", &caps["name"]);
            }
            None => {
                // println!("Not Match {}", s);
                values.push(s.to_string());
            }
        }

        line.clear();
    }

    if key.len() > 0 && values.len() > 0 {
        map.insert(key, values.clone());
    }

    output_batch(&map);
    output_shell(&map);

    Ok(())
}
```

