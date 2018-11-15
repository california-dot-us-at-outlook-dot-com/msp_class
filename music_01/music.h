/*
 * music.h
 *
 *  Created on: 2018年10月30日
 *      Author: Cal
 */

#ifndef MUSIC_H_
#define MUSIC_H_
#define CPU_F ((double)8000000)   //外部高频晶振8MHZ
//#define CPU_F ((double)32768)   //外部低频晶振32.768KHZ
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))





#endif /* MUSIC_H_ */
