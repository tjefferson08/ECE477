all: group project

group:
        @echo "Travis Jefferson - jefferst@purdue.edu"
        @echo "Matt McMunigle - mmcmunig@purdue.edu"
		@echo "Cournet Laubach - claubach@purdue.edu"
        @echo "Sophie Pouliquen - spouliqu@purdue.edu"

project:
        rm -rf bin
		mkdir bin
clean:
        rm -rf bin

.PHONY: all group project clean