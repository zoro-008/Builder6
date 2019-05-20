#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include "dask.h"

char *file_name="7200d";
U32 read_count=2000;
F64 sample_rate=10000;

main()
{
  I16 card, err, card_num;
  BOOLEAN halfReady, fStop;
  U32 count=0, count1=0;
  U32 i;
  U32 t=0;
  U16 sts=0;

  printf("Please input a card number: ");
  scanf(" %d", &card_num);
  //getch();
  if ((card=Register_Card (PCI_7200, card_num)) <0 ) {
        printf("Register_Card error=%d\n", card);
        exit(1);
  }
  err = DI_7200_Config(card, TRIG_INT_PACER, DI_NOWAITING, DI_TRIG_FALLING, IREQ_FALLING);
  if (err!=0) {
       printf("DI_7200_Config error=%d", err);
       exit(1);
  }
  err = DI_AsyncDblBufferMode(card, 1);
  if (err!=0) {
       printf("DI_DblBufferMode error=%d", err);
       exit(1);
  }
  err = DI_ContReadPortToFile(card, 0, file_name, read_count, sample_rate, ASYNCH_OP);
  if (err!=0) {
       printf("DI_ContReadPort error=%d", err);
       exit(1);
  }
  printf("\n\nPress any key to stop input operation.\n\n");
  do {
        do {
             DI_AsyncDblBufferHalfReady(card, &halfReady);
        } while (!halfReady);
        DI_AsyncDblBufferTransfer(card, NULL);  //transfer to file
        count += (read_count / 2);
		DI_AsyncDblBufferOverrun(card, 0, &sts);
		if(sts) {
			t++;
			DI_AsyncDblBufferOverrun(card, 1, &sts);
		}
		printf("\roverrun:%d, count: %d", t, count);
  } while(!kbhit());
  DI_AsyncClear(card, &count1);
  count += count1;
  Release_Card(card);
  printf("\n\n%d input data are stored in file '%s.dat'.\n", count, file_name);
  printf("\nPress ENTER to exit the program. "); getch();
}
