var	a : integer;
	b : integer;
	ret : integer;
	foo : string;

function max(num1 : integer, num2: integer): integer;
var result: integer;

begin
	if (num1 > num2) then
		result := num1;
	else
		result := num2;

	max := result
end;

begin
	a := 400;
	b := 200;

	foo := "Dlouhy string
	na vice radek";

	ret := max(a, b);

	write(ret)

end.
