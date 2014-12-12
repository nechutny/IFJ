var cislo : integer;
    cislo2 : integer;
    desetine : real;
    text : string;

function funkce1( foo : integer) : integer; forward;

function begin( arg1 : real; arg2 : integer ) : integer;
begin
    funkce2 := 123
end;

function funkce1( foo : integer) : integer;
var xyz : real;
begin
    funkce1 := 1;
    foo := 123
end;

begin
    cislo := funkce1(3)

end.
