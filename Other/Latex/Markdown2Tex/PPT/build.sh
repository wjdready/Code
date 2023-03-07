#!bash

set -e

function main {

    local file=$1
    local build_dir=$2

    [ "$file" == "" ] && echo "Usage: $0 <markdown file> [build_dir (build default)]" && exit -1
    [ ! -f "$file" ] && echo "No Such fiile $1" && exit -1

    [ "$build_dir" == "" ] && build_dir="build"
    [ ! -d "$build_dir" ] && mkdir -p $build_dir

    local template_file=$build_dir/template.tex
    [ ! -f "$template_file" ] && pandoc -D latex >$template_file

    local filename=$(basename $file)
    local target_texfile=$build_dir/${filename%.*}.tex

    local asset_dir=$(dirname $file)/${filename%.*}

    [ -d "$asset_dir" ] && cp -r $asset_dir $build_dir

    pandoc $file \
        --pdf-engine=xelatex \
        -t beamer  \
        -V monofont='JetBrainsMono-Light' \
        -V tableofcontents \
        --highlight-style=tango \
        --slide-level 2 \
        --template=$template_file \
        -o $target_texfile

    # 使用 use ctex
    sed -i 's/^\]{beamer}/\]{beamer}\n\\usepackage{tikz}\n\\usepackage{ctex}\n\\usepackage{atbegshi}/' $target_texfile
    # 设置代码字体大小
    sed -i 's/\(\\newenvironment{Shaded}{\\begin{snugshade}\)/\1\\footnotesize/' $target_texfile
    # 开启章序号
    sed -i 's/\(\\setcounter\)/% \1/' $target_texfile
    # 创建目录
    sed -i 's/\(\\frame{\\titlepage}\)/\1\n\\begin{frame}\\tableofcontents\\end{frame}/' $target_texfile

    # 插入目录页功能
    # \AtBeginSection[]
    # {
    # \begin{frame}
    #     \frametitle{Table of Contents}
    #     \tableofcontents[currentsection]
    # \end{frame}
    # }
    sed -i 's/\(\\begin{document}\)/\\AtBeginSection\[\]\n{\n\\begin{frame}\n    \\frametitle{Table of Contents}\n    \\tableofcontents\[currentsection\]\n\\end{frame}\n}\n\n\1/' $target_texfile

    # 使用 logo
    sed -i 's/\(\\begin{document}\)/\\logo{\\tikz\\node[opacity=.5] {\\includegraphics[height=1cm]{gogo\/logo}};}\n\n\1/' $target_texfile

    # 使用首行缩进
    # sed -i 's/\(\\usepackage{tikz}\)/\1\n\\usepackage{indentfirst}/' $target_texfile
    # sed -i 's/\(\\maketitle\)/\1\n\\setlength{\\parindent}{2em}/' $target_texfile

    # 替换
    sed -i 's/\\includegraphics{plantuml/\\input{plantuml/' $target_texfile
    sed -i 's/\(\\input.*\)/\\begin{figure}\n\\centering\n\\resizebox{5cm}{5cm}{\1}\n\\end{figure}\n/' $target_texfile

    # sed -i 's/\\includegraphics/\\includegraphics[height=5cm]/' tmp.tex

    # # 注释掉该段
    # perl -i -pe '
    #     if (/ifundefined{KOMAClassName}/) {
    #         $_ .= <> while !/KOMAoptions{parskip=half}}/;
    #         s/(.*\n)/% $1/gm;
    #     }' tmp.tex

    # xelatex -output-dir=$build_dir --aux-directory=$build_dir $target_texfile

    latexmk \
        -bibtex \
        -xelatex \
        -synctex=1 \
        -interaction=nonstopmode \
        -file-line-error \
        -aux-directory=$build_dir \
        -outdir=$build_dir \
        $target_texfile

    # -V mainfont='JetBrainsMono-Light' \
    # -V CJKmainfont='思源黑体'         \
    # -quiet      \
}

main $@
