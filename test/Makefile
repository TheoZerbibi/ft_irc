PIPE=pipo
OUTPUT=output
INPUT=input
TEST=ncirc.sh

all: $(OUTPUT)

$(OUTPUT): $(PIPE)
	./$(TEST) < $(INPUT) > $(OUTPUT)

$(PIPE):
	mkfifo $(PIPE)

clean:
	rm $(PIPE)

fclean:
	rm -f $(OUTPUT)
