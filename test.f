\ traceon
: multiples 11 1 do dup i * . i 5 = if i . then loop drop cr ;
7 multiples
." Hello!" cr
: test s" Works?" . . ;
test
\ traceon
s" Works?" type cr
