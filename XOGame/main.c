
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char turn;
unsigned char board [3][3];
int r,c;
void init (){
	turn =0;
	for (int i=0;i<3;i++)
	for (int j=0;j<3;j++)
	board[i][j]='.';
	r=c=0;
	
}
unsigned char Left(){
	if (c==0)return 1;
	c--;
	return 0;
}
unsigned char Right(){
	if (c==2)return 1;
	c++;
	return 0;
}
unsigned char Up(){
	if (r==0)return 1;
	r--;
	return 0;
}
unsigned char Down(){
	if (r==2)return 1;
	r++;
	return 0;
}

// 0 lsa ma5lstsh , 1 el awl faz , 2 el tany faz, 3 draw
unsigned char win;
int gameState(){
	if(board[0][0] == board[0][1] && board[0][0]==board[0][2] && board[0][0] == 'x' ){
		win=0;
		return 1;
	}
	if(board[1][0] == board[1][1] && board[1][0]==board[1][2] && board[1][0] == 'x' ){
		win=1;
		return 1;
	}
	if(board[2][0] == board[2][1] && board[2][0]==board[2][2] && board[2][0] == 'x' ){
		win=2;
		return 1;	
	}
	if(board[0][0] == board[0][1] && board[0][0]==board[0][2] && board[0][0] == 'o' ){
		win=0;
		return 2;
	}
	if(board[1][0] == board[1][1] && board[1][0]==board[1][2] && board[1][0] == 'o' ){
		win=1;
		return 2;
	}
	if(board[2][0] == board[2][1] && board[2][0]==board[2][2] && board[2][0] == 'o' ){
		win=2;
		return 2;
	}
	if(board[0][0] == board[1][0] && board[0][0]==board[2][0] && board[0][0] == 'x' ){
		win=3+0;
		return 1;
	}
	if(board[0][1] == board[1][1] && board[0][1]==board[2][1] && board[0][1] == 'x' ){
		win=3+1;
		return 1;
	}
	if(board[0][2] == board[1][2] && board[0][2]==board[2][2] && board[0][2] == 'x' ){
		win=3+2;
		return 1;
	}
	if(board[0][0] == board[1][1] && board[2][2]==board[1][1] && board[0][0] == 'x' ){
		win=6;
		return 1;
	}
	if(board[0][0] == board[1][1] && board[2][2]==board[1][1] && board[0][0] == 'o' ){
		win=6;
		return 2;
	}
	if(board[0][0] == board[1][0] && board[0][0]==board[2][0] && board[0][0] == 'o' ){
		win=3+0;
		return 2;
	}
	if(board[0][1] == board[1][1] && board[0][1]==board[2][1] && board[0][1] == 'o' ){
		win=3+1;
		return 2;
	}
	if(board[0][2] == board[1][2] && board[0][2]==board[2][2] && board[0][2] == 'o' ){
		win=3+2;
		return 2;
	}
	if(board[0][2] == board[1][1] && board[2][0]==board[1][1] && board[1][1] == 'x' ){
		win=7;
		return 1;
	}
	if(board[0][2] == board[1][1] && board[2][0]==board[1][1] && board[1][1] == 'o' ){
		win=7;
		return 2;
	}
	
	for(int i = 0 ; i <3 ; i++)
	for(int j = 0 ; j<3 ; j++)
	if(board[i][j] == '.') return 0;
	
	return 3;
}

unsigned char add (){
	if (board[r][c]!='.')return 1;
	board[r][c]=(turn?'o':'x');
	turn=1-turn;
	return 0;
}


unsigned char ended=0;

int main(void)
{
	/* Replace with your application code */
	
	DDRD = DDRD | 0b01110000; //PortA bits 0 to 4 input     (0:left, 1:right, 2:up, 3:down, 4:select)
	PORTD = PORTD | 0b10001111; //PortA bits 0 to 4 include pull-up resistors
	DDRC = 0xFF; //portC all output --> 8 (8 player 1 0--> 00)
	DDRB = 0xFF; // PortB all output --> 8 (8 Player 2 0--> 00)
	DDRA = DDRA & 0b10001111; //PortD pins 7, 3,2,1,0 -->5 (2 player 1, 2 Player 2, 1 Select)
	DDRL = 0xFF; // portL all output --> 8 (29) (8 Select)
	// c= 0 // B=1 // D=2; l=3
	init();
	DDRG = DDRG | 0b00000001; //PORTG pin0 output --> 1 (30) (Buzzer)
	while (1)
	{
		action();
		_delay_ms(1500);
	}

}
void action(){
	if (!ended){
	if(!(PIND & (1<<0))){
		if(Left()){
			PORTG |= 0x01;
			_delay_ms(1000);
			PORTG &= 0;
		}
	}
	if(!(PIND & (1<<1))){
		if(Right()){
			PORTG |= 0x01;
			_delay_ms(1000);
			PORTG &= 0;
		}
	}
	if(!(PIND & (1<<2))){
		if(Up()){
			PORTG |= 0x01;
			_delay_ms(1000);
			PORTG &= 0;
		}
	}
	if(!(PIND & (1<<3))){
		if(Down()){
			PORTG |= 0x01;
			_delay_ms(1000);
			PORTG &= 0;
		}
	}
	if(!(PIND & (1<<7))){
		if(add()){
			PORTG |= 0x01;
			_delay_ms(1000);
			PORTG &= 0;
		}
	}
	
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			if (board[i][j]=='x'){
				if (i==2 && j==2){
					PORTA |= 1<<(0);
				}
				else  PORTC |= 1<<(i*3+j);
			}
			else if (board[i][j]=='o'){
				if (i==2 && j==2){
					PORTA |= 1<<(1);
				}
				else  PORTB |= 1<<(i*3+j);
			}
			
		}
	}
	PORTL = 0;
	PORTA &= 0b11111011;
	if (r==2 && c==2){
		PORTA |= 1<<(2);
	}
	else  PORTL |= 1<<(r*3+c);
	
	if (turn){
		PORTA |= 1<<(3);
		PORTA &= ~(1<<(7));
	}
	
	else {
		PORTA |= 1<<(7);
		PORTA &= ~(1<<(3));
	}
	ended=gameState();
	if (ended>0){
		PORTB = 0;
		PORTC = 0;
		PORTA = 0;
		if (ended==1 || ended ==2){
			unsigned char ch =(ended==1?'x':'o');
			for (int i=0;i<3;i++)
			for (int j=0;j<3;j++)board[i][j]='.';
			if (win<3){
				board[win][0]=board[win][1]=board[win][2]=ch;
			}
			else if (win<6){
				board[0][win-3]=board[1][win-3]=board[2][win-3]=ch;
			}
			else if (win==6){
				board[0][0]=board[1][1]=board[2][2]=ch;
			}
			else {
				board[0][2]=board[1][1]=board[2][0]=ch;
			}
			
			while(1){
				for (int i=0;i<3;i++){
					for (int j=0;j<3;j++){
						if (board[i][j]=='x'){
							if (i==2 && j==2){
								PORTA |= 1<<(0);
							}
							else  PORTC |= 1<<(i*3+j);
						}
						else if (board[i][j]=='o'){
							if (i==2 && j==2){
								PORTA |= 1<<(1);
							}
							else  PORTB |= 1<<(i*3+j);
						}
						
					}
				}
				PORTG |= 0x01;
				_delay_ms(5000);
				PORTG = 0;
				PORTB = 0;
				PORTC = 0;
				PORTA = 0;
				_delay_ms(5000);
				
		}
		}
		else{
			while (1){
				PORTL = 0xFF;
				PORTA |= 1<<(2);
				_delay_ms(1000);
				PORTL = 0;
				PORTA = 0;
				_delay_ms(1000);
			}
		}
	}
	}
}


