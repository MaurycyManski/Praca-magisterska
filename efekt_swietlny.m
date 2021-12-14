t = linspace(0,2*pi,1000);

x = (sawtooth(t+pi/2,1/2)+1)*50;
plot(t/(2*pi)*20,x,'g')
hold on
x = (sawtooth(t+pi,1/2)+1)*50;  
plot(t/(2*pi)*20,x,'r')
hold on
x = (sawtooth(t, 1/2)+1)*50;  
plot(t/(2*pi)*20,x,'b')

title('Sygna³y RGB')
xlabel('Czas [s]')
ylabel('Unormowana intensywnoœæ [%]')