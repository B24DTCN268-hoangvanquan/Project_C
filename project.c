#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAX_EMP 100

typedef struct {
  char empId[20];
  char name[50];
  char position[15];
  double baseSalary;
  int workDay;
} Employee;
typedef struct {
  int logId;
  char empId[20];
  char date[20];
  char status[10];
} TimeSheet;

Employee employees[MAX_EMP];
int empCount = 0;

TimeSheet timesheets[1000]; // bảng chấm công
int timesheetCount = 0;

// Nguyên mẫu hàm nhân viên
void addEmployee();
void updateEmployee();
void deleteEmployee();
void displayEmployees();
void searchEmployee();
void sortEmployees();

// Nguyên mẫu hàm chấm công
void timeKeeping();
void showTimesheet();

// Nguyên mẫu hàm tiện ích
int findEmployeeIndex(char empId[]);
int isValidDateFormat(const char *date);
int isDuplicateCheckIn(char empId[], char date[]);
void showMenu();

// hàm main
int main() {
  int choice;
  do {
    showMenu();
    scanf(" %d", &choice);
    getchar();
    switch (choice) {
    case 1:
      addEmployee();
      break;
    case 2:
      updateEmployee();
      break;
    case 3:
      deleteEmployee();
      break;
    case 4:
      displayEmployees();
      printf("Nhấn Enter để tiếp tục ...");
      getchar();
      break;
    case 5:
      searchEmployee();
      break;
    case 6:
      sortEmployees();
      break;
    case 7:
      timeKeeping();
      break;
    case 8:
      showTimesheet();
      break;
    case 9:
      printf("Thoát chương trình...\n");
      break;
    default:
      printf("Lựa chọn không hợp lệ!\n");
    }
  } while (choice != 9);

  return 0;
}

// menu
void showMenu() {
  printf("\n===== QUẢN LÝ NHÂN VIÊN & CHẤM CÔNG =====\n");
  printf("1. Thêm nhân viên\n");
  printf("2. Cập nhật hồ sơ\n");
  printf("3. Xóa nhân viên\n");
  printf("4. Hiển thị danh sách\n");
  printf("5. Tìm kiếm nhân viên\n");
  printf("6. Sắp xếp danh sách\n");
  printf("7. Chấm công ngày\n");
  printf("8. Xem bảng chấm công\n");
  printf("9. Thoát\n");
  printf("=========================================\n");
  printf("Chọn chức năng: ");
}

//  tìm chỉ số nhân viên theo empId
int findEmployeeIndex(char empId[]) {
  for (int i = 0; i < empCount; i++) {
    if (strcmp(employees[i].empId, empId) == 0)
      return i;
  }
  return -1;
}
int isValidDateFormat(const char *date) {
  // Định dạng phải có dạng dd/mm/yyyy
  if (strlen(date) != 10)
    return 0;
  if (date[2] != '/' || date[5] != '/')
    return 0;
  // Kiểm tra các ký tự còn lại là số
  for (int i = 0; i < 10; i++) {
    if (i == 2 || i == 5)
      continue;
    if (date[i] < '0' || date[i] > '9')
      return 0;
  }
  return 1;
}
int isDuplicateCheckIn(char empId[], char date[]) {
  for (int i = 0; i < timesheetCount; i++) {
    if (strcmp(timesheets[i].empId, empId) == 0 &&
        strcmp(timesheets[i].date, date) == 0) {
      return 1; // đã tồn tại
    }
  }
  return 0;
}

int isValidDate(const char *date) {
  if (!isValidDateFormat(date))
    return 0;

  int dd, mm, yyyy;
  sscanf(date, "%d/%d/%d", &dd, &mm, &yyyy);

  if (mm < 1 || mm > 12 || dd < 1 || dd > 31)
    return 0;

  // Kiểm tra số ngày hợp lệ theo tháng
  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) {
    daysInMonth[1] = 29; // năm nhuận
  }
  if (dd > daysInMonth[mm - 1])
    return 0;

  // Kiểm tra không vượt quá ngày hiện tại
  time_t t = time(NULL);
  struct tm *now = localtime(&t);
  int curr_dd = now->tm_mday;
  int curr_mm = now->tm_mon + 1;
  int curr_yyyy = now->tm_year + 1900;

  if (yyyy > curr_yyyy)
    return 0;
  if (yyyy == curr_yyyy && mm > curr_mm)
    return 0;
  if (yyyy == curr_yyyy && mm == curr_mm && dd > curr_dd)
    return 0;

  return 1;
}

// thêm nhân viên
void addEmployee() {
  if (empCount >= MAX_EMP) {
    printf("Danh sách nhân viên đã đầy!\n");
    return;
  }

  Employee e;
  printf("\nThêm nhân viên mới\n");

  // Nhập empId
  do {
    printf("Nhập mã nhân viên (empId): ");
    fgets(e.empId, sizeof(e.empId), stdin);
    e.empId[strcspn(e.empId, "\n")] = '\0';

    if (strlen(e.empId) == 0) {
      printf("empId không được để trống!\n");
      continue;
    }
    if (findEmployeeIndex(e.empId) != -1) {
      printf("empId đã tồn tại! Nhập lại.\n");
      continue;
    }
    break;
  } while (1);

  // Nhập tên nhân viên
  do {
    printf("Nhập tên nhân viên: ");
    fgets(e.name, sizeof(e.name), stdin);
    e.name[strcspn(e.name, "\n")] = '\0';
    if (strlen(e.name) == 0)
      printf("Tên không được để trống!\n");
  } while (strlen(e.name) == 0);

  // Nhập chức vụ
  do {
    printf("Nhập chức vụ: ");
    fgets(e.position, sizeof(e.position), stdin);
    e.position[strcspn(e.position, "\n")] = '\0';
    if (strlen(e.position) == 0)
      printf("Chức vụ không được để trống!\n");
  } while (strlen(e.position) == 0);

  // Nhập lương cơ bản
  do {
    printf("Nhập lương cơ bản: ");
    scanf("%lf", &e.baseSalary);
    getchar();
    if (e.baseSalary <= 0)
      printf("Lương phải lớn hơn 0!\n");
  } while (e.baseSalary <= 0);

  // Nhập ngày công
  int choice;
  do {
    printf("Ngày công:\n");
    printf("1. Chưa có ngày công (lưu = 0)\n");
    printf("2. Nhập số ngày công\n");
    printf("Chọn: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
      e.workDay = 0;
      break;
    } else if (choice == 2) {
      do {
        printf("Nhập số ngày công: ");
        scanf("%d", &e.workDay);
        getchar();
        if (e.workDay <= 0) {
          printf("Ngày công phải lớn hơn 0!\n");
          continue;
        }
        break;
      } while (1);
      break;
    } else {
      printf("Lựa chọn không hợp lệ!\n");
    }
  } while (1);

  employees[empCount++] = e;
  printf("Thêm nhân viên thành công!\n");
}

// cập nhật nhân viên
void updateEmployee() {
  char id[20];
  printf("\nCập nhật hồ sơ nhân viên\n");

  do {
    printf("Nhập empId cần sửa: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    if (strlen(id) == 0) {
      printf("empId không được để trống!\n");
      continue;
    }
    break;
  } while (1);

  int idx = findEmployeeIndex(id);
  if (idx == -1) {
    printf("Không tìm thấy nhân viên!\n");
    return;
  }

  // cập nhật tên
  do {
    printf("Nhập tên mới: ");
    fgets(employees[idx].name, sizeof(employees[idx].name), stdin);
    employees[idx].name[strcspn(employees[idx].name, "\n")] = '\0';

    if (strlen(employees[idx].name) == 0) {
      printf("Tên không được để trống!\n");
      continue;
    }
    break;
  } while (1);

  // cập nhật chức vụ
  do {
    printf("Nhập chức vụ mới: ");
    fgets(employees[idx].position, sizeof(employees[idx].position), stdin);
    employees[idx].position[strcspn(employees[idx].position, "\n")] = '\0';

    if (strlen(employees[idx].position) == 0) {
      printf("Chức vụ không được để trống!\n");
      continue;
    }
    break;
  } while (1);

  // cập nhật lương
  do {
    printf("Nhập lương cơ bản mới: ");
    if (scanf("%lf", &employees[idx].baseSalary) != 1) {
      printf("Lương phải là số!\n");
      while (getchar() != '\n')
        ;
      continue;
    }
    getchar();
    if (employees[idx].baseSalary <= 0) {
      printf("Lương phải lớn hơn 0!\n");
      continue;
    }
    break;
  } while (1);

  // cập nhật ngày công
  int choice;
  do {
    printf("Ngày công:\n");
    printf("1. Giữ nguyên (không thay đổi)\n");
    printf("2. Đặt lại = 0 (chưa có ngày công)\n");
    printf("3. Nhập số ngày công mới (>0)\n");
    printf("Chọn: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
      // giữ nguyên workDay
      break;
    } else if (choice == 2) {
      employees[idx].workDay = 0;
      break;
    } else if (choice == 3) {
      do {
        printf("Nhập số ngày công mới: ");
        scanf("%d", &employees[idx].workDay);
        getchar();
        if (employees[idx].workDay <= 0) {
          printf("Ngày công phải lớn hơn 0!\n");
          continue;
        }
        break;
      } while (1);
      break;
    } else {
      printf("Lựa chọn không hợp lệ!\n");
    }
  } while (1);

  printf("Cập nhật thành công!\n");
}

// quản lý nhân viên (sa thải nhân viên)
void deleteEmployee() {
  char id[20];
  char confirm;

  printf("\nXóa nhân viên\n");

  // Nhập mã nhân viên
  do {
    printf("Nhập mã nhân viên cần xóa: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = '\0';

    if (strlen(id) == 0) {
      printf("Mã nhân viên không được để trống.\n");
      continue;
    }
    break;
  } while (1);

  int idx = findEmployeeIndex(id);
  if (idx == -1) {
    printf("Không tìm thấy nhân viên có mã %s.\n", id);
    return;
  }

  // Xác nhận trước khi xóa
  printf("Bạn có chắc chắn muốn xóa nhân viên %s (%s)? (y/n): ",
         employees[idx].name, id);
  scanf(" %c", &confirm);
  getchar(); // loại bỏ newline
  if (confirm != 'y' && confirm != 'Y') {
    printf("Hủy xóa nhân viên.\n");
    return;
  }

  // Xóa nhân viên
  for (int i = idx; i < empCount - 1; i++) {
    employees[i] = employees[i + 1];
  }
  empCount--;

  printf("Xóa nhân viên mã(ID) %s thành công.\n", id);
}

// hiển thị danh sách nhân viên
void displayEmployees() {
  if (empCount == 0) {
    printf("\nDanh sách nhân viên hiện đang trống.\n");
    return;
  }

  const int pageSize = 5; // số nhân viên mỗi trang
  int totalPages = (empCount + pageSize - 1) / pageSize; // tính số trang
  int page;

  do {
    printf("Có tổng cộng %d trang.\n", totalPages);
    printf("Nhập số trang muốn xem (1 - %d): ", totalPages);
    if (scanf("%d", &page) != 1) {
      while (getchar() != '\n')
        ; // xóa input lỗi
      page = 0;
    }
    getchar(); // xóa ký tự newline
    if (page < 1 || page > totalPages) {
      printf("Số trang không hợp lệ! Vui lòng nhập lại.\n");
    }
  } while (page < 1 || page > totalPages);

  printf("\n===== DANH SÁCH NHÂN VIÊN - TRANG %d/%d =====\n", page, totalPages);
  printf("+-----------+----------------------+-----------------+---------------"
         "+---------------+\n");
  printf("| Mã NV     | Tên NV               | Chức vụ         | Lương cơ bản  "
         "| Số ngày công  |\n");
  printf("+-----------+----------------------+-----------------+---------------"
         "+---------------+\n");

  int start = (page - 1) * pageSize;
  int end = start + pageSize;
  if (end > empCount)
    end = empCount;

  for (int i = start; i < end; i++) {
    printf("| %-9s | %-20s | %-15s | %13.2f | %13d |\n", employees[i].empId,
           employees[i].name, employees[i].position, employees[i].baseSalary,
           employees[i].workDay);
  }

  printf("+-----------+----------------------+-----------------+---------------"
         "+---------------+\n");
}

// tìm kiếm nhân viên theo empId hoặc tên
void searchEmployee() {
  char keyword[50];
  printf("\n===== TÌM KIẾM NHÂN VIÊN =====\n");
  printf("Nhập empId hoặc tên cần tìm: ");
  fgets(keyword, sizeof(keyword), stdin);
  keyword[strcspn(keyword, "\n")] = '\0';

  int found = 0;
  printf("+-----------+----------------------+-----------------+---------------"
         "+\n");
  printf("| Mã NV     | Tên NV               | Chức vụ         | Lương cơ bản  "
         "|\n");
  printf("+-----------+----------------------+-----------------+---------------"
         "+\n");

  for (int i = 0; i < empCount; i++) {
    if (strstr(employees[i].empId, keyword) != NULL ||
        strstr(employees[i].name, keyword) != NULL) {
      printf("| %-9s | %-20s  | %-15s | %13.2f |\n", employees[i].empId,
             employees[i].name, employees[i].position, employees[i].baseSalary);
      found = 1;
    }
  }

  if (!found) {
    printf("| %-9s | %-20s | %-15s | %13s |\n", "-", "-", "-", "-");
    printf("Không tìm thấy nhân viên nào phù hợp!\n");
  }

  printf("+-----------+----------------------+-----------------+---------------"
         "+\n");
}

// hàm sắp xếp danh sách nhân viên theo lương cơ bản
void sortEmployees() {
  if (empCount == 0) {
    printf("\nDanh sách nhân viên hiện tại đang trống!\n");
    return;
  }

  int choice;
  printf("\nSắp xếp danh sách nhân viên theo lương cơ bản\n");
  printf("1. Tăng dần\n");
  printf("2. Giảm dần\n");
  printf("Chọn cách sắp xếp: ");
  scanf("%d", &choice);
  getchar();

  if (choice == 1) {
    // Sắp xếp tăng dần
    for (int i = 0; i < empCount - 1; i++) {
      for (int j = i + 1; j < empCount; j++) {
        if (employees[i].baseSalary > employees[j].baseSalary) {
          Employee temp = employees[i];
          employees[i] = employees[j];
          employees[j] = temp;
        }
      }
    }
    printf("Đã sắp xếp theo lương tăng dần!\n");
  } else if (choice == 2) {
    // Sắp xếp giảm dần
    for (int i = 0; i < empCount - 1; i++) {
      for (int j = i + 1; j < empCount; j++) {
        if (employees[i].baseSalary < employees[j].baseSalary) {
          Employee temp = employees[i];
          employees[i] = employees[j];
          employees[j] = temp;
        }
      }
    }
    printf("Đã sắp xếp theo lương giảm dần!\n");
  } else {
    printf("Lựa chọn không hợp lệ!\n");
    return;
  }

  displayEmployees();
}

// hàm chấm công ngày
void timeKeeping() {
  char empId[20], date[20];

  printf("\n===== CHẤM CÔNG NGÀY =====\n");

  // Nhập mã NV
  do {
    printf("Nhập mã nhân viên: ");
    fgets(empId, sizeof(empId), stdin);
    empId[strcspn(empId, "\n")] = '\0';

    if (strlen(empId) == 0) {
      printf("Mã NV không được để trống!\n");
      continue;
    }
    break;
  } while (1);

  // Tìm nhân viên
  int idx = findEmployeeIndex(empId);
  if (idx == -1) {
    printf("Không tìm thấy nhân viên có mã %s.\n", empId);
    return;
  }

  // Nhập ngày chấm công
  int dd, mm, yyyy;
  do {
    printf("Nhập ngày chấm công (dd/mm/yyyy): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0';

    if (strlen(date) == 0) {
      printf("Ngày không được để trống!\n");
      continue;
    }
    if (!isValidDateFormat(date)) {
      printf("Định dạng ngày không hợp lệ! (dd/mm/yyyy)\n");
      continue;
    }

    // Tách ngày/tháng/năm
    sscanf(date, "%d/%d/%d", &dd, &mm, &yyyy);

    // Kiểm tra tháng hợp lệ
    if (mm < 1 || mm > 12) {
      printf("Tháng không hợp lệ!\n");
      continue;
    }

    // Số ngày trong tháng
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((yyyy % 4 == 0 && yyyy % 100 != 0) || (yyyy % 400 == 0)) {
      daysInMonth[1] = 29; // năm nhuận
    }
    if (dd < 1 || dd > daysInMonth[mm - 1]) {
      printf("Ngày không hợp lệ!\n");
      continue;
    }

    // Lấy ngày hiện tại
    time_t t = time(NULL);
    struct tm *now = localtime(&t);
    int curr_dd = now->tm_mday;
    int curr_mm = now->tm_mon + 1;
    int curr_yyyy = now->tm_year + 1900;

    // Kiểm tra không vượt quá ngày hiện tại
    if (yyyy > curr_yyyy || (yyyy == curr_yyyy && mm > curr_mm) ||
        (yyyy == curr_yyyy && mm == curr_mm && dd > curr_dd)) {
      printf("Ngày không được vượt quá ngày hiện tại!\n");
      continue;
    }

    break;
  } while (1);

  // Kiểm tra trùng lặp
  if (isDuplicateCheckIn(empId, date)) {
    printf("Nhân viên %s đã chấm công ngày này rồi.\n", empId);
    return;
  }

  // Tạo bản ghi chấm công
  TimeSheet ts;
  ts.logId = timesheetCount + 1;
  strcpy(ts.empId, empId);
  strcpy(ts.date, date);
  strcpy(ts.status, "Đi làm"); // trạng thái mặc định

  // Cập nhật ngày công nhân viên
  employees[idx].workDay++;

  // Lưu vào danh sách timesheet
  timesheets[timesheetCount++] = ts;

  printf("Chấm công ngày %s cho nhân viên %s thành công.\n", date, empId);
}

// hàm xem bảng chấm công
void showTimesheet() {
  char viewEmpId[20];
  printf("\n===== XEM BẢNG CHẤM CÔNG =====\n");
  printf("Nhập mã nhân viên cần xem: ");
  scanf("%s", viewEmpId);
  getchar();

  int idx = findEmployeeIndex(viewEmpId);
  if (idx == -1) {
    printf("Không tìm thấy nhân viên có mã %s.\n", viewEmpId);
    return;
  }

  printf("\n===== BẢNG CÔNG CỦA NHÂN VIÊN %s - %s =====\n",
         employees[idx].empId, employees[idx].name);

  printf("+-------+--------------+-------------+\n");
  printf("| LogID | Ngày chấm    | Trạng thái  |\n");
  printf("+-------+--------------+-------------+\n");

  int count = 0;
  for (int i = 0; i < timesheetCount; i++) {
    if (strcmp(timesheets[i].empId, viewEmpId) == 0) {
      printf("| %-5d | %-12s | %-11s   |\n", timesheets[i].logId,
             timesheets[i].date, timesheets[i].status);
      count++;
    }
  }

  if (count == 0) {
    printf("| %-5s | %-12s | %-11s |\n", "-", "-", "-");
    printf("Nhân viên này chưa có ngày công nào.\n");
  }

  printf("+-------+--------------+-------------+\n");
}
