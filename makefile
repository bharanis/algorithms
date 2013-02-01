DIRS = common bst heap sorting

all:  
	for d in $(DIRS); do (cd $$d; make all); done

clean: 
	for d in $(DIRS); do (cd $$d; make clean); done
