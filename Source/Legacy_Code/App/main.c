#include "./System/Testcase_Types.h"

/*chọn id của loại ngoại vi cần demo testcase*/
TC_IdType_e ty = TC_GPT_ID;

int main()
{
	/*đếm số lượng test case để khởi tạo bảng giám sát trạng thái xử lý*/
	uint8 total = TestCaseTotalCount(ty);

	/*khởi tạo bảng đồ lưu trử TC và trạng thái xử lý*/
	TestCaseMgr_s TC_Mgr[total]; // mảng chứa vừa đủ số lượng phần tử cần quản lý

	/*Gán dữ liệu mặc định cho từng phần tử*/
	TC_TableInitParam(TC_Mgr, total);
	/*lặp qua từng testcase để xử lý và cập nhật keyname và trạng thái kết quả*/
	for (uint8 tc = 0; tc < total; tc++)
	{
		TC_Mgr[tc].st = TC_Mgr[tc].RunFunc(); /*tự động gọi hàm và trả kết quả xử lý*/
	}

	return 0;
}
