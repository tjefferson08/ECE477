/*
 * SPI testing utility (using spidev driver)
 *
 * Copyright (c) 2007  MontaVista Software, Inc.
 * Copyright (c) 2007  Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I/path/to/cross-kernel/include
 */

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define MAX_TX_SIZE (64)
#define TRANSFER_SIZE (20)

static void pabort(const char *s)
{
	perror(s);
	abort();
}

static const char *device = "/dev/spidev0.1";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 62500;
static uint16_t delay;
static uint8_t sendData[MAX_TX_SIZE];


int transferData(int fd, char *data) {
	int ret;
	char rx[20] = {0, };
	uint8_t txData[20] = {0, };
	for (ret=0;ret<TRANSFER_SIZE;ret++) {
	  txData[ret] = data[ret];
	}

	struct spi_ioc_transfer tr = {
	  .tx_buf = (unsigned long)txData,
	  .rx_buf = (unsigned long)rx,
	  .len = ARRAY_SIZE(txData),
	  .delay_usecs = delay,
	  .speed_hz = speed,
	  .bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

	printf("\nTX BUF******************** \n");
	for (ret = 0; ret < ARRAY_SIZE(txData); ret++) {
		if (!(ret % 6))
			puts("");
		printf("tx: %.2X \n", txData[ret]);
	}
	printf("rx: %d\n", rx[0]);
	// check rx packets for diffent commands
	if (rx[0] == 1) return 1;
	else return 0;
}

int transferCmd(int fd, char *data) {
  	int ret;
	char rx[1] = {0, };
	char txData[1] = {data[0]};
      

	struct spi_ioc_transfer tr = {
	  .tx_buf = (unsigned long)txData,
	  .rx_buf = (unsigned long)rx,
	  .len = ARRAY_SIZE(txData),
	  .delay_usecs = delay,
	  .speed_hz = speed,
	  .bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");

	printf("\nTX BUF******************** \n");
	for (ret = 0; ret < ARRAY_SIZE(txData); ret++) {
		if (!(ret % 6))
			puts("");
		printf("tx: %.2X \n", txData[ret]);
	}
	printf("rx: %d\n", rx[0]);
	if (rx[0] == 2) return 1;
	else return 1;
  
}

int main(int argc, char **argv)
{
  int hb = 1;
  int metaType = 0;  
  int charCount = 0;     

  if (argc == 5) {
    hb = 0;
  }

      int ret = 0;
      int fd;

      fd = open(device, O_RDWR);
      if (fd < 0)
	pabort("can't open device");

      /*
       * spi mode
       */
      ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
      if (ret == -1)
	pabort("can't set spi mode");

      ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
      if (ret == -1)
	pabort("can't get spi mode");

      /*
       * bits per word
       */
      ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
      if (ret == -1)
	pabort("can't set bits per word");

      ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
      if (ret == -1)
	pabort("can't get bits per word");

      /*
       * max speed hz
       */
      ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
      if (ret == -1)
	pabort("can't set max speed hz");

      ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
      if (ret == -1)
	pabort("can't get max speed hz");

      printf("spi mode: %d\n", mode);
      printf("bits per word: %d\n", bits);
      printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);

      uint8_t hbData[1];
      uint8_t mData[1];
      hbData[0] = 0xAA;
      mData[0] = 0xBB;
      int temp;
      int count = 0;
	  int nextSong = 0;
      if (hb) {
		nextSong = transferCmd(fd, hbData);		
      }
      else {
       	temp = transferCmd(fd, mData);
		//Send song title
		temp = transferData(fd, argv[1]);
		//Send artist
        printf("\nfinished song!\n");
		temp = transferData(fd, argv[2]);
        printf("\nfinished artist!\n");
		//Send album
		temp = transferData(fd, argv[3]);
        printf("\nfinished album!\n");
		//Send year
		temp = transferData(fd, argv[4]);
        printf("\nfinished year!\n");
      }
      /*       	while (1) { 
		count++;
		temp = transfer(fd, txData, 1);
		printf("%d: temp is: %d\n",count, temp);
		if (temp) {
		printf("found a special command\n");
		break;
		}
		else {
		printf("NOTHING\n");
		}
		} */
      close(fd);
      return nextSong;
}
