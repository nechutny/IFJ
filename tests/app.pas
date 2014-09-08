program fooo;

var	cislo : integer = 5;
	desetine : real;

function funkce(arg1 : real) : integer;
begin
	writeln(arg1);
	return 2;
end;

begin
	cislo = 7;
	cislo = funkce(1.25);
	if cislo == 2
	then begin
		cislo = 3;
		destine = 1.25;
	end;

	repeat until keypressed();
	
end.
