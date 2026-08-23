#include "SdCard_Read.h"
#include "AppTypes.h"
#ifdef DISKO_H
FATFS fs; // file system
FIL file; // File
// FILlist_LicensePlate fno;
FRESULT fresult; // result
UINT br, bw;	 // File read/write count

/**** capacity related *****/
FATFS *pfs;
DWORD fre_clust;
uint32_t total, free_space;
static int buffSize(char *pbuff);
static RfidUserDtypeCfg_s *read_data(char *path);
static void check_free_space();
static void SDcard_write();
static void SDcard_read();
static void SDcard_updateFile();

#endif

int count = 0;
RfidUserDtypeCfg_s *list_RfidUserData = NULL;
#define FILE_SRC ".//Bsw//database//db.csv"

/* các hàm thao tác file cục bộ */
static Status Load_UserDB(const char *path);
static Status WriteFile(const char *path);
static RfidUserDtypeCfg_s *parse_line(char *line);

void SDcard_ModuleInit()
{
#ifdef DISKO_H
	MX_FATFS_Init();
	fresult = f_mount(&fs, "/", 1);
	while (fresult != FR_OK)
		; // kiem tra ket noi the nho thanh cong hay khong

	check_free_space();

	SDcard_write();
#endif

	if (Load_UserDB(FILE_SRC) == E_OK)
	{
		ModeStatusCfg.apptype = INIT_MODE;
	}
	else
	{
		return;
	}
}

static Status Load_UserDB(const char *path)
{
	if (WriteFile(path) == E_OK)
	{
		FILE *file = fopen(path, "r");
		if (file == NULL)
		{
			return NOT_OK;
		}
		char line[100];
		// đọc và không xử lý dòng tiêu đề
		fgets(line, sizeof(line), file);
		// cấp phát vùng nhớ động
		list_RfidUserData = (RfidUserDtypeCfg_s *)malloc(sizeof(RfidUserDtypeCfg_s));

		// kiểm tra vùng nhớ hợp lệ
		if (list_RfidUserData == NULL)
		{
			return NOT_OK;
		}

		// đọc từng dòng của filem
		while (fgets(line, sizeof(line), file) != NULL)
		{
			// tách ra thông tin và trả kết quả về kết quả tạm thời
			RfidUserDtypeCfg_s *temp = parse_line(line);
			// Tăng số lượng UserData
			count++;
			// điều chỉnh tăng kích thước vùng nhớ mỗi khi cập nhật thông tin user mới
			list_RfidUserData = (RfidUserDtypeCfg_s *)realloc(list_RfidUserData, sizeof(RfidUserDtypeCfg_s) * count);
			// copy vào vùng nhớ đã cáp phát
			list_RfidUserData[count - 1] = *temp;
			// Xóa vùng nhớ tạm mà không giải phóng vùng nhớ cấp phát cho các thành viên bên trong
			free(temp);
		}
		fclose(file);
		return E_OK;
	}
	return NOT_OK;
}
static Status WriteFile(const char *path)
{
	FILE *file = fopen(path, "w");
	if (file == NULL)
	{
		return NOT_OK;
	}
	fprintf(file, "ten,tuoi,so dien thoai,dia chi\n");
	fprintf(file, "Pham Cao Duy,25,0972665872,91 Pham Van Hai P3 Q Tan Binh\n");
	fprintf(file, "Trinh Tran Phuong Tuan,24,0908234588,TP Tay Ninh \n");
	fprintf(file, "Trinh Le Hoang,28,0376572677,12 Nguyen Xien Q12\n");
	fprintf(file, "Nguyen Tan Tung,21,038764589,45 Tran Binh Trong\n");
	fprintf(file, "Le Quang Nhat,26,0978278121,17 Binh Phuoc\n");
	fprintf(file, "Nguyen Huu Hung,22,0978565342,17 Q Binh Thanh\n");
	fprintf(file, "Pham Cao Duy,18,090395678,220/8 Nguyen Phuc Nguuyen P9 Q3\n");
	fprintf(file, "Dinh Anh Tuan,24,0903478211,TP Tay Ninh\n");
	fprintf(file, "Nguyen Ho Duy,17,0906733209,22/5 Binh Thuan\n");
	fprintf(file, "Pham Cao Duy,20,0376572231,18 Nguyen Thi Minh Khai Q1 TPHCM\n");
	fprintf(file, "Nguyen Thi Thanh Thuy,27,038764987,21 Nguyen Thien Thuat P12 QTan Phu\n");
	fprintf(file, "Nguyen Ho Duy,27,038764912,22 Nguyen Thien Thuat P13 Q Tan Binh\n");
	fprintf(file, "Nguyen Thi Thanh Thu,27,038764997,23 Nguyen Thien Thuat P14  Q Binh Thanh\n");
	fprintf(file, "Pham Cao Duy,27,038764910,24 Nguyen Thien Thuat P9 Q 10\n");
	fclose(file);
	return E_OK;
}
static RfidUserDtypeCfg_s *parse_line(char *line)
{
	// cấp phát 1 vùn nhớ tạm để lưu trữ thông tin của user hiện tại
	RfidUserDtypeCfg_s *temp = (RfidUserDtypeCfg_s *)malloc(sizeof(RfidUserDtypeCfg_s));
	char *token; // con trỏ lưu trữ giá trị tạm thời

	// Parse name
	token = strtok(line, ","); // tách từng ký tự cho đến khi gặp dấu ","
	temp->name = (char *)malloc(strlen(token) + 1);
	strcpy(temp->name, token); // sao chép vào vùng nhớ heap

	// Parse age
	token = strtok(NULL, ",");
	temp->license_plate = (char *)malloc(strlen(token) + 1);
	strcpy(temp->license_plate, token); // sao chép vào vùng nhớ heap

	return temp; // trả về thông tin của user hiện tại
}

#ifdef DISKO_H

static int buffSize(char *pbuff)
{
	int size = 0;
	while (pbuff[size++] != '\0')
		;
	return size;
}
static void check_free_space()
{
	f_getfree("", &fre_clust, &pfs);
	total = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
	sprintf(buffer, "SD CARD Total Size: \t%d\n", total);
	memset(buffer, 0, sizeof(buffer));
	free_space = (uint32_t)(fre_clust * pfs->csize * 0.5);
	memset(buffer, 0, sizeof(buffer));
}
static void SDcard_write()
{
	/* Create second file with read write access and OPENING it */
	fresult = f_open(&file, "file2.txt", FA_CREATE_ALWAYS | FA_WRITE);

	/* Writing text */
	strcpy(buffer, "This is File2.txt, written using ...f_write... and it says Hello from Controllerstech\n");

	fresult = f_write(&file, buffer, buffSize(buffer), &bw);

	/*CLOSING file */
	f_close(&file);

	memset(buffer, 0, sizeof(buffer));
}
static void SDcard_read()
{
	fresult = f_open(&file, "file2.txt", FA_READ);
	if (fresult == FR_OK)
		f_read(&file, buffer, f_size(&file), &br);
	/*CLOSING filee */
	f_close(&file);
	memset(buffer, 0, sizeof(buffer));
}
static void SDcard_updateFile()
{
	/* OPENING the file with write access */
	fresult = f_open(&file, "filee2.txt", FA_OPEN_EXISTING | FA_READ | FA_WRITE);

	/* Move to offset to the end of the filee */
	fresult = f_lseek(&file, f_size(&file));

	if (fresult == FR_OK)
	{

		/* write the string to the filee */
		fresult = f_puts("This is updated data and it should be in the end", &file);

		f_close(&file);

		memset(buffer, 0, buffSize(buffer));

		/* OPENING to read the filee */
		fresult = f_open(&file, "filee2.txt", FA_READ);

		/* Read string from the filee */
		fresult = f_read(&file, buffer, f_size(&file), &br);

		// if read successfully -> skip this below line
		while (fresult != FR_OK)
			;
	}
	/*CLOSING filee */
	f_close(&file);
}

#endif