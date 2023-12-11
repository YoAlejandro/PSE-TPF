#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

void delay_ms(unsigned int n);
void delay_5us();
void delay_10us();
void delay_10us_n(unsigned int n);
unsigned long myAtoi(char* str);
