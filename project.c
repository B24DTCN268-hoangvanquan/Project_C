#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMP 100

typedef struct {
  char empId[20];
  char name[50];
  char position[15];
  double baseSalary;
  int workDay;
} Employee;

// nguyên mẫu hàm
void addEmployee();
void updateEmployee();
void deleteEmployee();
void displayEmployees();
void showMenu();
void searchEmployee();
void sortEmployees();

// hàm main
int main() {
  int choice;
  do {
    showMenu();
    scanf(" %d", &choice);
    // getchar();
    while (getchar() != '\n')
      ;
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
      break;
    case 5:
      searchEmployee();
      break;
    case 6:
      sortEmployees();
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
  printf("8. Xem bảng công\n");
  printf("9. Thoát\n");
  printf("=========================================\n");
  printf("Chọn chức năng: ");
}

// mảng lưu trữ nhân viên
Employee employees[MAX_EMP];
int empCount = 0;

//  tìm chỉ số nhân viên theo empId
int findEmployeeIndex(char empId[]) {
  for (int i = 0; i < empCount; i++) {
    if (strcmp(employees[i].empId, empId) == 0)
      return i;
  }
  return -1;
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

  printf("\nXóa nhân viên\n");

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

  for (int i = idx; i < empCount - 1; i++) {
    employees[i] = employees[i + 1];
  }
  empCount--;

  printf("Xóa nhân viên %s thành công.\n", id);
}

// hiển thị danh sách nhân viên
void displayEmployees() {
  if (empCount == 0) {
    printf("\nDanh sách nhân viên hiện đang trống.\n");
    return;
  }

  printf("\nDanh sách nhân viên:\n");
  printf("---------------------------------------------------------------------"
         "----------\n");
  printf("| %-10s | %-20s | %-15s | %-12s | %-10s |\n", "EmpId", "Tên",
         "Chức vụ", "Lương cơ bản", "Ngày công");
  printf("---------------------------------------------------------------------"
         "----------\n");

  for (int i = 0; i < empCount; i++) {
    printf("| %-10s | %-20s | %-15s | %-12.2f | %-10d |\n", employees[i].empId,
           employees[i].name, employees[i].position, employees[i].baseSalary,
           employees[i].workDay);
  }

  printf("---------------------------------------------------------------------"
         "----------\n");
}

// tìm kiếm nhân viên theo empId hoặc tên
void searchEmployee() {
  char keyword[50];
  printf("\nTìm kiếm nhân viên\n");
  printf("Nhập empId hoặc tên cần tìm: ");
  fgets(keyword, sizeof(keyword), stdin);
  keyword[strcspn(keyword, "\n")] = '\0';

  int found = 0;
  printf("------------------------------------------------------------\n");
  printf("| %-10s | %-20s | %-15s | %-10s |\n", "EmpId", "Tên", "Chức vụ",
         "Lương cơ bản");
  printf("------------------------------------------------------------\n");

  for (int i = 0; i < empCount; i++) {
    if (strstr(employees[i].empId, keyword) != NULL ||
        strstr(employees[i].name, keyword) != NULL) {
      printf("| %-10s | %-20s | %-15s | %-10.2f |\n", employees[i].empId,
             employees[i].name, employees[i].position, employees[i].baseSalary);
      found = 1;
    }
  }

  if (!found) {
    printf("Không tìm thấy nhân viên nào phù hợp!\n");
  }
  printf("------------------------------------------------------------\n");
}

// sắp xếp danh sách nhân viên theo lương cơ bản
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
