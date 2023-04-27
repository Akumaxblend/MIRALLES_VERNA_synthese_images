CC		= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lglfw -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers TD 02

# Fichiers exercice 01
OBJ_PROJET= projet.o
EXEC_PROJET= projet.out


# Regles compilation TD 02

all : projet
projet : $(OBJDIR)$(OBJ_PROJET)
	$(CC) $(CFLAGS) $(OBJDIR)$(OBJ_PROJET) -o $(BINDIR)$(EXEC_PROJET) $(LDFLAGS)


clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
