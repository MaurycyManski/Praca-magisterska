%%
N = 10000; % liczba pr�bek

X = linspace(2.5,4.2,N);
Y = zeros(1,N);
Z = zeros(1,N);

for c = 1:N
    if X(c)< 3.5
        Y(c) = 100*(X(c)-0.2)/ X(c);
        Z(c) = 0.2 * 0.5;
    else 
        Y(c) = 100*(3.3/ X(c));
        Z(c) = (X(c) - 3.3) * 0.5;
    end
end
figure
plot(X,Y)
title('Sprawno�� stabilizatora')
xlabel('Napi�cie na akumulatorze [V]')
ylabel('Sprawno�� stabilizatora [%]')
grid on

figure
plot(X,Z)
title('Moc wydzielana na stabilizatorze przy pr�dzie 0,5A')
xlabel('Napi�cie na akumulatorze [V]')
ylabel('Moc wydzielana na stabilizatorze [W]')
grid on