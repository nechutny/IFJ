program fooo;
var	cislo, cislo2 : integer = 5;
	desetine : real = 12.5;
	

function funkce1( arg1 : real, arg2 : integer ) : integer;
begin
	writeln( arg1, cislo, 15-2 );
	
	for bla := 1 to 15 do
		return 2;
end;
function funkce2() : integer;
begin
	writeln( -7-4 );
	while False do begin
		funkce2( );
		desetine := desetine - 15;
	end;
	funkce1( 1234,5, cislo);
end;
function funkce3( arg1 : integer ) : integer;
var var1 : integer = 10;
begin
	writeln( arg1 );
	if arg1 = 5 then begin
		arg1 := 5+var1;
		funkce2();
		repeat
			var1 := (arg1 +  5) * funkce1(1.2, funkce1(1.2, var1));
		until False;
	end else begin
		arg1 := -arg1*5;
	end;
	return 2;
	funkce3( arg1);
end;

begin
	cislo := 7;
	cislo := funkce1( 1.25, 6);
	if cislo = 2 then begin
		cislo := 3;
		destine := 1.25;
	end;
	text := 'foo fooo bar';

	cislo := text[0];

	case cislo of
		1 : funkce2( );
		4 : begin
			cislo2 := 15;
			funkce2();
		end;
		5 : funkce3(cislo2);
		else
			funkce2();
	end;
end.
