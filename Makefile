monoplan: Module_IHM/main_gtk.c Module_formule_cellules/cell.c Module_formule_cellules/Stack/stack.c Module_liste/liste.c
	gcc -o monoplan Module_IHM/main_gtk.c Module_formule_cellules/cell.c Module_formule_cellules/Stack/stack.c Module_liste/liste.c -Wall -g `pkg-config --cflags --libs gtk+-3.0` -lm

run: monoplan
	./monoplan

clean:
	rm -f monoplan