SCRIPT = 30p01a.lkr
OBJECTS =  mastri2c.o init.o i2ccomm.o
OUTPUT = top.s19

all : $(OUTPUT)

$(OUTPUT) : $(OBJECTS)
	#../gplink/gplink --map -s $(SCRIPT) -o $(OUTPUT) $(OBJECTS)
	../gplink/gplink -m -I ../lkr -s $(SCRIPT)  -o $(OUTPUT) $(OBJECTS)

%.o : %.asm
	../gpasm/gpasm -p mc30p01a -c $<

clean:
	rm -f *.o *.lst *.map *.s19 *.cod *.cof

mastri2c.o : mastri2c.asm mastri2c.inc i2ccomm1.inc flags.inc
init.o : init.asm
i2ccomm.o : i2ccomm.asm i2ccomm.inc i2ccomm1.inc flags.inc


