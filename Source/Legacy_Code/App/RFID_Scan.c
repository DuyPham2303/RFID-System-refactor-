#include "rc522.h"
#include "RFID_Scan.h"
#include "Systick.h"
#include "AppTypes.h"

uint8_t RfidUserData_Index;

unsigned char block = 2;
unsigned char buff[4];
uint8_t total_uid = 5;
uint8_t tagtype[2];
int id = 0;
unsigned char list_uid[5] = {0xFA, 0x94, 0x34, 0xE9, 0x0E};
unsigned char uid[4];
unsigned char block_address[16] = {0};
unsigned char size;
unsigned char read_buff[16];
char *license_list[] = {"none",
						"59F2-2322",
						"89F2-2413",
						"23A1-2129",
						"12K1-2312",
						"59H2-8922"
						"67h3-1291",
						"12h8-1231"};
static bool verify_data(const char *buff)
{
	for (RfidUserData_Index = 0; RfidUserData_Index < count; RfidUserData_Index++)
	{
		if (strcmp(buff, list_RfidUserData[RfidUserData_Index].license_plate) == 0)
		{
			return true;
		}
	}
	return false;
}
static bool verify_uid(unsigned char *uid)
{
	while (total_uid)
	{
		static int index = 0;
		if (uid[index] == list_uid[index])
		{
			return true;
		}
		index++;
		total_uid--;
	}
	return false;
}

void RFID_Scan()
{
	static unsigned char sectorKey[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
	// kiem tra loai the
	while (MFRC522_Request(PICC_REQIDL, tagtype) != MI_OK)
		;

	// lay uid
	while (MFRC522_Anticoll(uid) != MI_OK)
		;

	// chon the
	size = MFRC522_SelectTag(uid);

	// xac thuc the

	while (MFRC522_Read(block, block_address) != MI_OK)
		;

	while (MFRC522_Auth(PICC_AUTHENT1A, block, sectorKey, uid) != MI_OK)
		;

	while (MFRC522_Read(block, block_address) != MI_OK)
		;

	static unsigned char data[] = "Pham Cao Duy";

	while (MFRC522_Write(block, data) != MI_OK)
		;

	do
	{
		while (MFRC522_Read(block, block_address) != MI_OK)
			;
		block++;
	} while (block < 4);

	block = 0;
	MFRC522_Halt();
	delay_ms(500);

	char *pRev = NULL;
	while (MFRC522_Read(4, (unsigned char *)pRev) != MI_OK)
		;

	uint8_t len = strlen(pRev);
	unsigned char *buff = (unsigned char *)malloc(sizeof(unsigned char) * len + 1);
	strcpy((char *)buff, pRev);

	// RFID card hop le
	if (verify_uid(uid))
	{
		if (verify_data((char *)buff))
		{
			ModeStatusCfg.apptype = RUN_MODE;
		}
		else
		{
			ModeStatusCfg.apptype = ERROR_MODE;
			ModeStatusCfg.errortype = RFID_SCAN_EMPTY;
		}
		ModeStatusCfg.IsLcdModeUpdate = true;
	}
	// RFID khong hop le
	else
	{
		ModeStatusCfg.apptype = ERROR_MODE;
		ModeStatusCfg.errortype = UID_NOTFOUND;
	}

	free(pRev);
}

void RFID_ModuleInit()
{
	MFRC522_Init();
}
