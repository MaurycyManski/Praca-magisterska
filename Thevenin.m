%%
R = 4300; %rezystor dodatkowy
C = 10^-5;
Pot = 50000;
N = 101; %liczba probek


X = linspace(0,100,N);
Rpot = zeros(1,N);
Rpot = (X/100*Pot .* (Pot - X/100 * Pot))./(X/100*Pot + Pot - X/100*Pot);

figure
plot(X,Rpot)
title('Rezystancja zastêpcza Thevenina')
xlabel('Droga potencjometru [%]')
ylabel('Rezystancja zastêpcza Thevenina [?]')
grid on

figure
plot(X,(Rpot+R)*C)
title('Sta³a czasowa RC')
xlabel('Droga potencjometru [%]')
ylabel('Sta³a czasowa RC [s]')
grid on

figure
plot(X,1./(2*pi*(Rpot+R)*C))
title('Filtr dolnoprzepustowy')
xlabel('Droga potencjometru [%]')
ylabel('Czestotliwoœæ odciêcia -3dB [Hz]')
grid on