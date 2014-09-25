program foo;

var a : integer;
vysl : integer;

function factorial(n : integer ) : integer;
	var temp_result : integer;
	decremented_n : integer;
	begin
		if n < 2 then
		begin
			write('Konec factorialu'#10'');
			factorial := 1
		end
		else
		begin
			decremented_n := n - 1;
			temp_result := factorial(decremented_n);
			factorial := n*temp_result
		end

		write('n je:', n,''#10'');
		write('decremented je:', decremented_n,''#10'');
		write('temp_result je:', temp_result,''#10'');
		write('factorial je:', factorial,''#10'', ''#10'');
	end;

begin
	write('Zadejte cislo pro vypocet faktorialu :');
	readln(a);
	if a < 0 then
	
	begin
		write('Faktorial nelze spocitat '#10'')
	end
	else
	begin
		vysl := factorial(a);
		write('Vysledek je:', vysl,''#10'')
	end
end.
