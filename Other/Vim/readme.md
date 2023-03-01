

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



