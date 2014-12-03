var	a : integer;
	b : integer;
	ret : integer;

function sort(num1 : integer, num2: integer): integer;
var result: integer;

begin
	if (num1 > num2) then
		result := num1;
	else
		result := num2;

	sort := result
end;

begin
	a := 400;
	b := 200;

	ret :=sort(a, b);

	write(ret)

end.
