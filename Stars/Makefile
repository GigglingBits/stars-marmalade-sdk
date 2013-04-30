####################################################################
# Setup
####################################################################
#TIMESTAMP	:= $(shell date +"%Y%m%d_%H%M%S" --utc)
TIMESTAMP	:= $(shell date -u +%Y%m%d_%H%M%S)


DISTPATH	:= dist
DISTNAME	:= $(NAME)_$(TIMESTAMP)

BACKUPPATH	:= backup
BACKUPNAME	:= backup_$(TIMESTAMP)


####################################################################
# generic targets
####################################################################
all: 	backup

doxygen: 
	cd doxygen
	doxygen

backup:	$(BACKUPPATH)/$(BACKUPNAME).tar.gz

$(BACKUPPATH)/$(BACKUPNAME).tar.gz:
	@mkdir -p $(BACKUPPATH)
	tar --exclude $(BACKUPPATH) --exclude ./build_game_vc10 --exclude ./doxygen/html --exclude ./MiniDump.dmp -cvzf $(BACKUPPATH)/$(BACKUPNAME).tar.gz .
	@echo $(BACKUPPATH)/$(BACKUPNAME).tar.gz

clean:
	rm -rf ./build_game_vc10
	rm -rf ./doxygen/html
	rm ./MiniDump.dmp
	#$(MAKE) -C doxygen clean




