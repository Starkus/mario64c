set tabstop=4
set softtabstop=4
set shiftwidth=4
set noexpandtab

autocmd vimenter * NERDTree
autocmd vimenter * wincmd w

set colorcolumn=110
highlight ColorColumn ctermbg=darkgray

let &path.="src,src/common,src/external"

set makeprg=make\ -C\ ./build\ -j9
nnoremap <F4> :make!<cr><cr>

nnoremap <F5> :!(cd src/ && ./mario64)<cr>
