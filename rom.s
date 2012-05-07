.text
.global ROM_F800
.global ROM_F800_END

ROM_F800:
.incbin "supermon.bin"
ROM_F800_END:
.previous
