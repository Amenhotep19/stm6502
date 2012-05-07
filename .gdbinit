target extended-remote :4242
file stm6502.elf

define flash
file stm6502.elf
load
end

define restart
run
end
