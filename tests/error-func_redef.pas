var cislo : integer;
    cislo2 : integer;
    desetine : real;
    text : string;

function funkce2( foo : integer) : integer; forward;

function funkce2( arg1 : real; arg2 : integer ) : integer;
begin
    funkce2 := 123
end;

function funkce2( foo : integer) : integer;
var xyz : real;
begin
    funkce2 := 1;
    foo := 123
end;

begin
    cislo := funkce2(123.5,3)

end.
