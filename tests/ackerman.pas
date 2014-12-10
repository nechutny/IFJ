var
    a:integer;
function Ack(m:integer; n:integer):integer;
begin
if m=0 then
begin
    Ack:=n+1
end
else
begin
    if n = 0 then
    begin
        Ack := Ack((m-1),1)
    end
    else
    begin
        Ack:=Ack(m-1,Ack(m,n-1))
    end
end
end;
 
begin
a :=Ack(1,2);
Write(a);
end.
