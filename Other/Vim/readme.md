

.vimrc 

```python
set nu
syntax enable
syntax on
set tabstop=4   
set softtabstop=4 
set shiftwidth=4  
set autoindent    
set ruler        
set cursorline  
set wildmenu   
set mouse=a   

inoremap jj <Esc>

map <Space> <leader>
map <leader>n :bnext<cr>
map <leader>p :bprevious<cr>
map <leader>d :bdelete<cr>

""""""""""""""theme""""""""""""""""""""
" colorscheme molokai 
colorscheme gruvbox
set background=dark "Setting dark/light mode


""""""""""""""Plug"""""""""""""""""""""
call plug#begin('~/.vim/plugged')

Plug 'jiangmiao/auto-pairs'

Plug 'scrooloose/nerdtree'

call plug#end() 
```

常用 Vim 快捷键和技巧:

| 模式   | 快捷键   | 备注                         |
| ------ | -------- | ---------------------------- |
| Normal | S        | 删除当前行, 然后进入插入模式 |
| Normal | cc       | 删除当前行, 然后进入插入模式 |
| Normal | ddO      | 删除当前行, 然后进入插入模式 |
| Normal | A\<C-u\> | 删除当前行, 然后进入插入模式 |
| Normal | D        | 删除光标后的所有内容         |


