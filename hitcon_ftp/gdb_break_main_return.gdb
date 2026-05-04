set confirm off
b *0x4003
commands
printf "BREAK\n"
x/20gx $rbp-0x20
x/32gx $rbp+0x8
quit
end
run 1337
